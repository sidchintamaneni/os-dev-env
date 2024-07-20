#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "syscall.h"

#define CHARDEV_PATH "/dev/chardev"
#define THREAD_COUNT 2

void *chardev_init(void *args) {
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

	exit(1);
err_ioctl:
	close(chardev_fd);
err_chardev_fd:
	return NULL;
}

int main() {
	
	pthread_t thr[THREAD_COUNT];
	int err;

	for (int i = 0; i < THREAD_COUNT; i++) {
		err = pthread_create(&thr[i], NULL, chardev_init, NULL);
		sleep(0.2);
		if(err) {
			fprintf(stderr, "Thread Creation Failed\n");
		}
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(thr[i], NULL);
	}

	return 0;
}