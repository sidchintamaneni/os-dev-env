#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "syscall.h"

#define CHARDEV_PATH "/dev/chardev"

int chardev_init() {
	int chardev_fd = -1;
	int err;

	chardev_fd = open(CHARDEV_PATH, O_RDWR);
	if (chardev_fd < 0) {
		err = chardev_fd;
		goto err_chardev_fd;
	}

	if (ioctl(chardev_fd, CHARDEV_IOCTL_INIT, NULL)) {
		fprintf(stderr, "Current %ld, IOCTL" 
				"syscall failed", pthread_self());
		goto err_ioctl;
	}


err_ioctl:
	close(chardev_fd);
err_chardev_fd:
	return err;
}

int main() {
	
	chardev_init();
	return 0;
}
