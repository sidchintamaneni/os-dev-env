#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 1024

int main() {
    int chardev_fd, err;
    ssize_t bytes_written, bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }
    
    char *wr_buf = malloc(BUFFER_SIZE);
    if (!wr_buf) {
        err = -1;
        goto err_malloc;
    }
    char *rd_buf = malloc(BUFFER_SIZE);
    if (!rd_buf) {
        err = -1;
        free(wr_buf);
        goto err_malloc;
    }
    bytes_written = write(chardev_fd, wr_buf, BUFFER_SIZE);
    bytes_read = read(chardev_fd, rd_buf, BUFFER_SIZE);
    
   // fprintf(stdout, "bytes_written %ld, write buffer: %lx\n", bytes_written, *((unsigned long *)wr_buf));
   // fprintf(stdout, "bytes_read %ld, read buffer: %lx\n", bytes_read, *((unsigned long *)rd_buf));
    if (memcmp(wr_buf, rd_buf, BUFFER_SIZE) == 0) {
        fprintf(stdout, "Testcase - PASSED\n");
    } else {
        fprintf(stdout, "Testcase - FAILED\n");
    }
err_malloc:
    close(chardev_fd);


    return err;
    
}
