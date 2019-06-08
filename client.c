#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "./perf.sock"
#define BUF_SIZE 1048576

int main ()
{
	int fd, wr, rd, i, sum;
	char buf[BUF_SIZE] = { 1 };
	struct sockaddr_un addr;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket error: %s", strerror(errno));
		return errno;
	}

	/* Initialization */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;

	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);
	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "connect error: %s", strerror(errno));
		return errno;
	}

	for (i = 0; i < 1024; i++) {
		if ((wr = write(fd, buf, BUF_SIZE)) != BUF_SIZE) {
			if (wr > 0) {
				fprintf(stderr, "listen error: %s", strerror(errno));
			} else {
				close(fd);
				fprintf(stderr, "write error: %s", strerror(errno));
				return errno;
			}
		}

		sum = 0;
		while (sum < BUF_SIZE && (rd = read(fd, buf, sizeof(buf))) > 0) {
			sum += rd;
			fprintf(stderr, "client: read %u bytes\n", rd);
		}

		if (rd == -1) {
			fprintf(stderr, "listen error: %s", strerror(errno));
			return errno;
		}
	}

	return 0;
}

