

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"

void* pthread_func(void *args) 
{
    int chardev_fd, err;
    ssize_t bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return NULL;
    }
	close(chardev_fd);
}

int main() 
{
	pthread_t tid;

    for (int i = 0; i < 100; i++)
        pthread_create(&tid, NULL, pthread_func, NULL);

    pthread_exit(NULL);
    return 0;
}
