#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <endian.h>

/* An colored "error:" message */
#define ERR "\e[31;1merror:\e[0m "

/* connect_to_server() - open the connection, returning a TCP socket
   This function opens the connection to 62.210.74.95:210 and returns an open
   socket for two-way communication, or a negative number on error. Close the
   returned socket with close(2). */
int connect_to_server(void)
{
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(tcp_socket < 0)
	{
		fprintf(stderr, ERR "cannot create socket: %m\n");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family		= AF_INET;

	/** The bytes of the port number and IP address must be specified in
	    "network" endian, which is big-endian, but the x86 is a little-
	    endian architecture. The endianness must be converted.
	    Because the port number is 16 bits and the IP address is 32 bits,
	    we choose htons() and htonl() (this is crucial). **/
	addr.sin_port		= htons(11111);
	addr.sin_addr.s_addr	= htonl(0x3ed24a5f); /* 62.210.74.95 */

	int x = connect(tcp_socket, (struct sockaddr *)&addr, sizeof addr);
	if(x != 0)
	{
		fprintf(stderr, ERR "connection failed: %m\n");
		close(tcp_socket);
		return -1;
	}

	return tcp_socket;
}

/* get_png_image() - get a full PNG image from the server, save to file
   This function tries to get a PNG image from the given TCP socket. It expects
   another file descriptor to an open file for writing, and writes the data to
   the file.

   @socket  Socket FD
   @file    FD of a file open for writing
   Returns a negative number on error and the size of the file on success. */
int get_png_image(int socket, int file)
{
	char buffer[1024];
	size_t downloaded_size = 0;	/* Total file size */
	ssize_t s_server;		/* Size returned by server */
	ssize_t s_file;			/* Size written to file */

	/* Get messages until now data is left */
	while(1)
	{
		printf("Waiting for data...\n");

		s_server = recv(socket, buffer, 1024, 0);
		if(s_server <= 0) break;

		printf("Received %d bytes from server\n", (int)s_server);

		downloaded_size += s_server;
		s_file = 0;

		/* Write the chunk to a file */
		while(s_file < s_server)
		{
			ssize_t x = write(file, buffer, s_server);
			if(x < 0)
			{
				fprintf(stderr, ERR "cannot save image: %m\n");
				return -1;
			}
			s_file += x;
		}

		printf("Written the chunk to file\n");

		/* TODO */
		break;
	}

	/* Return an error code if the retrieval failed at some point */
	if(s_server < 0)
	{
		fprintf(stderr, ERR "cannot retrieve data: %m\n");
		return -1;
	}

	return downloaded_size;
}

/* send_answer() - send the mean color calculated by the program */
void send_answer(int socket, int value)
{
	/* TODO: Do this properly */
	send(socket, &value, 3, 0);
}

/* get_status() - get a status code from the server
     -1  Error		0  Success	1  Fail
      2  TimeLimit	3  Goodbye
   @socket TCP socket to read from */
int get_status(int socket)
{
	const char *answers[] = {
		"SUCCESS\n", "FAIL\n", "TIMELIMIT\n", "GOODBYE\n", NULL
	};
	char buffer[32];
	int status;

	recv(socket, buffer, 1, 0);

	for(status = 0; answers[status]; status++)
	if(answers[status][0] == buffer[0])
	{
		recv(socket, buffer, strlen(answers[status]) - 1, 0);
		return status;
	}

	return status;
}

int main(void)
{
	/* First connect to server */
	int tcp_socket = connect_to_server();
	if(tcp_socket < 0) return 1;

	printf("Connection successful!\n");

	char name[20];

	for(int i = 1; i <= 10; i++)
	{
		snprintf(name, 20, "img%02d.png", i);

		/* Open a PNG file to download the image */
		int png = creat(name, 0644);
		if(png < 0)
		{
			fprintf(stderr,ERR "cannot create output image: %m\n");
			close(tcp_socket);
			return 1;
		}

		/* Get an image from the server */
		get_png_image(tcp_socket, png);

		/* Send a dummy value */
		send_answer(tcp_socket, 0);

		int status = get_status(tcp_socket);
		printf("Status code is %d\n", status);
	}

	close(tcp_socket);
	return 0;
}
