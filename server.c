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
	int fd, ac, rd, i;
	char buf[BUF_SIZE];
	struct sockaddr_un addr;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket error: %s", strerror(errno));
		return errno;
	}

	/* Initialization */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	unlink(SOCKET_NAME);

	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "bind error: %s", strerror(errno));
		return errno;
	}

	if (listen(fd, 5) == -1) {
		fprintf(stderr, "listen error: %s", strerror(errno));
		return errno;
	}

	for (i = 0; i < 1024; i++) {    
		if ((ac = accept(fd, NULL, NULL)) == -1) {
			fprintf(stderr, "listen error: %s", strerror(errno));
			return errno;
		}

		while ((rd = read(ac, buf, sizeof(buf))) > 0)
      			fprintf(stderr, "read %u bytes\n", rd);

		if (rd == -1) {
			fprintf(stderr, "listen error: %s", strerror(errno));
			return errno;
		} else if (rd == 0) {
			close(ac);
		}
	}

	return 0;
}

