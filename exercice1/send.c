#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>

ssize_t send(int socket, const void *buffer, size_t len, int flags)
{
	write(1, buffer, len);
	return sendto(socket, buffer, len, flags, NULL, 0);
}
