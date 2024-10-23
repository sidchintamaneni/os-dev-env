#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 1024

int chardev_fd;
char *wr_buf, *rd_buf;

/*
 * Avoiding userspace mutexes to test driver locks
 */ 
void *write_function() {
    
    ssize_t bytes_written; 
    wr_buf = (char *)malloc(BUFFER_SIZE);
    if (!wr_buf) {
        exit(0);
    }
    memset(wr_buf, 0xAF, BUFFER_SIZE);

//    fprintf(stdout, "started write syscall in thread1\n");
    bytes_written = write(chardev_fd, wr_buf, BUFFER_SIZE);
}

void *read_function() {
    ssize_t bytes_read; 
    rd_buf = (char *)malloc(BUFFER_SIZE);
    if (!rd_buf) {
        exit(0);
    }
    memset(rd_buf, 0xAA, BUFFER_SIZE);

//    fprintf(stdout, "started read syscall in thread2\n");
    bytes_read = read(chardev_fd, rd_buf, BUFFER_SIZE);
}

int main() {
    int err = 0;
    pthread_t tid1, tid2;

    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }


    err = pthread_create(&tid1, NULL, write_function, NULL);
    if(err < 0) {
        fprintf(stderr, "Failed to create thread1\n");
        close(chardev_fd);
        return -1;
    }

    err = pthread_create(&tid2, NULL, read_function, NULL);
    if(err < 0) {
        fprintf(stderr, "Failed to create thread2\n");
        close(chardev_fd);
        exit(0);
    }

    pthread_join( tid1, NULL);
    pthread_join( tid2, NULL); 

    char tmp[BUFFER_SIZE];
    memset(tmp, 0xAA, BUFFER_SIZE);

    if (memcmp(wr_buf, rd_buf, BUFFER_SIZE) == 0) {
        fprintf(stdout, "Testcase - PASSED (WRITE happend first)\n");
    } else {
        size_t bytes_read = read(chardev_fd, rd_buf, BUFFER_SIZE);
    }

    if (memcmp(wr_buf, rd_buf, BUFFER_SIZE) == 0) {
        fprintf(stdout, "Testcase - PASSED\n");
    } else {        
        fprintf(stdout, "Testcase - FAILED\n");
    }
    
    close(chardev_fd);
    return err;

}
