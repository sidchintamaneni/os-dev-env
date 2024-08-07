#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/types.h>

#include "syscall.h"

#define CHARDEV_PATH "/dev/chardev"
#define THREAD_COUNT 2

#define CHARDEV_USER_CTXT 0xdeadbeef
#define CHARDEV_KERNEL_CTXT 0xdeaddead

struct chardev_init_args {
	__u32 init_type;
	__u64 chardev_mmap_memory_base;
};

void *chardev_init(void *args) {
	int chardev_fd = -1;
	int err;
	struct chardev_init_args chardev_user_args;
	void *chardev_mmap_ptr;

	chardev_user_args.init_type = CHARDEV_USER_CTXT;

	chardev_fd = open(CHARDEV_PATH, O_RDWR);
	if (chardev_fd < 0) {
		err = chardev_fd;
		goto err_chardev_fd;
	}

	if (ioctl(chardev_fd, CHARDEV_IOCTL_INIT, &chardev_user_args)) {
		fprintf(stderr, "Current %ld, IOCTL" 
				" syscall failed", pthread_self());
		goto err_ioctl;
	}

	// Returning from kernel context
	fprintf(stdout, "Current %ld, chardev_init function, " 
			"Returning from kernel context, init_type %x and "
			"pointer for memory created 0x%llx\n", pthread_self(), 
			chardev_user_args.init_type, chardev_user_args.chardev_mmap_memory_base);

	// Now need to mmap kernel memory to userspace
	chardev_mmap_ptr = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_SHARED,
				chardev_fd, chardev_user_args.chardev_mmap_memory_base);

	if (chardev_mmap_ptr == MAP_FAILED) {
		fprintf(stderr, "Current %ld, MMAP " 
				"Failed..\n", pthread_self());
		goto err_ioctl;
	}
	
	fprintf(stdout, "Current %ld, chardev_init function, " 
			"Returning from kernel context, init_type %x and "
			"after mmap memory %p and value %x\n", pthread_self(),
			chardev_user_args.init_type, chardev_mmap_ptr, *(__u32 *)chardev_mmap_ptr);
	// Check the string bytes stored in chardev_mmap_ptr

//	exit(1);
	munmap(chardev_mmap_ptr, 4096);

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
		sleep(0.5);
		if(err) {
			fprintf(stderr, "Thread Creation Failed\n");
		}
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(thr[i], NULL);
	}

	return 0;
}
