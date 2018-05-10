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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <png.h>

/* A colored "error:" message */
#define ERR "\e[31;1merror:\e[0m "

//---
//	Question 3 - Average color
//---

/* avg_color() - read a PNG file and calculate the average color

   @fp   FILE pointer to source image
   @avg  Pointer to an array of 3 uint8_t for the average colors (r, g, b)
   Returns nonzero on error. */

void avg_color_free(png_structp *png, png_infop *info, uint8_t *img)
{
	if(png) png_destroy_read_struct(png, *info ? info : NULL, NULL);
	if(img) free(img);
}

int avg_color(FILE *fp, uint8_t *avg)
{
	#define fail() { \
		avg_color_free(&png, &info, pixels); \
		return 1; \
	}

	/* The volatile is required by the longjmp()-based exception handling
	   if libpng */
	png_uint_32 width = 0, height = 0;
	volatile int r = 0, g = 0, b = 0;

	png_structp png = NULL;
	png_infop info = NULL;
	uint8_t *pixels = NULL;

	/* Allocate memory for the reading */

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) fail();

	info = png_create_info_struct(png);
	if(!info) fail();

	if(setjmp(png_jmpbuf(png))) fail();

	/* Setup libpng and read "headers" */

	png_init_io(png, fp);
	png_set_sig_bytes(png, 0);
	png_read_info(png, info);

	/* Get image dimensions; 24-bit RGB without interlacing is hardcoded */

	png_get_IHDR(png, info, &width, &height, NULL, NULL, NULL, NULL, NULL);

	/* Allocate memory to read the image */

	uint32_t bytes_per_row = png_get_rowbytes(png, info);
	pixels = malloc(height * bytes_per_row);
	if(!pixels) fail();

	uint8_t *rows[height];
	for(size_t y = 0; y < height; y++)
	{
		rows[y] = pixels + y * bytes_per_row;
	}

	/* Finally read the image! */

	png_read_image(png, rows);

	/* Compute the average components */

	for(size_t y = 0; y < height; y++)
	for(size_t x = 0; x < width; x++)
	{
		r += rows[y][3 * x];
		g += rows[y][3 * x + 1];
		b += rows[y][3 * x + 2];
	}

	avg[0] = r / (width * height);
	avg[1] = g / (width * height);
	avg[2] = b / (width * height);

	avg_color_free(&png, &info, pixels);
	return 0;
}

//---
//	Questions 2.4 - Network communication
//---

/* Status names sent by the server */
const char *status_names[] = {
	"SUCCESS\n", "FAIL\n", "TIMEOUT\n", "GOODBYE\n", NULL
};

/* connect_to_server() - open the connection, returning a TCP socket
   This function opens the connection to 62.210.74.95:11111 and returns an open
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
	    we choose htons() and htonl() (this is important !) **/
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

/* get_png_image() - get a full PNG image from the server, save to buffer
   This function tries to get a PNG image from the given TCP socket. It expects
   a pointer to an allocated buffer and fills the buffer.

   @socket   Socket FD
   @buffer   Allocated buffer
   @bufsize  Size of the buffer - must be enough to hold the image
   On success, returns the number of bytes written to the buffer. Otherwise,
   returns a negative number. */
int get_png_image(int socket, char *buffer, size_t bufsize)
{
	/* Size received from the server */
	ssize_t size = read(socket, buffer, bufsize);

	/* Print an error message if the retrieval failed */
	if(size < 0) fprintf(stderr, ERR "cannot retrieve data: %m\n");

	return (int)size;
}

/* send_answer() - send the mean color calculated by the program */
void send_answer(int socket, uint8_t *avg)
{
	ssize_t size = write(socket, avg, 3);

	if(size != 3)
	{
		fprintf(stderr, ERR "cannot send answer to server: %m\n");
		return;
	}
}

/* get_status() - get a status code from the server, returned as int
     -1  Error		0  Success	1  Fail
      2  TimeLimit	3  Goodbye
   @socket TCP socket to read from */
int get_status(int socket)
{
	char buffer[32];
	int status;
	size_t x;

	/* Read the first letter, deduce the size and read the rest */
	x = read(socket, buffer, 1);
	if(!x) return -1;

	for(status = 0; status_names[status]; status++)
	if(status_names[status][0] == buffer[0])
	{
		size_t length = strlen(status_names[status]) - 1;
		x = read(socket, buffer + 1, length);
		return (x == length) ? status : -1;
	}

	printf("Weird status: %c\n", buffer[0]);
	return status;
}

int main(void)
{
	/* Disable buffering on stdout */
	setvbuf(stdout, NULL, _IONBF, 0);

	/* Connect to server */

	int tcp_socket = connect_to_server();
	if(tcp_socket < 0) return 1;

	printf("Connection successful!\n");

	/* Receive images and return average colors */

	const char *png_header = "\x89" "PNG" "\r\n\x1a\n";

	char buffer[1024];
	int status = 0;
	ssize_t size;

	for(int i = 0; status != 4; i++)
	{
		printf("Solving puzzle %02d... ", i);

		/* Get an image from the server */
		size = get_png_image(tcp_socket, buffer, 1024);
		if(size < 0)
		{
			close(tcp_socket);
			return 1;
		}

		/* If it's not a PNG image, then we've got the password! o/ */
		if(size < 8 || strncmp(buffer, png_header, 8))
		{
			printf("got the password!\n");
			buffer[size] = 0;
			printf("%s", buffer);
			break;
		}

		/* Otherwise, find out the mean value */
		uint8_t avg[3];
		FILE *fp = fmemopen(buffer, size, "r");

		avg_color(fp, avg);
		printf("r%02x g%02x b%02x...", avg[0], avg[1], avg[2]);

		/* Send the value and wait for the status code */
		send_answer(tcp_socket, avg);

		status = get_status(tcp_socket);
		printf(" %s", status_names[status]);
	}

	close(tcp_socket);
	return 0;
}
