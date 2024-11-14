
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 1024

/*
 * Testing read and write syscall by filling the entire
 * buffer
 */ 
int main() {
    int chardev_fd, err;
    ssize_t bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }
    
    char *rd_buf = malloc(BUFFER_SIZE);
    if (!rd_buf) {
        err = -1;
        goto err_malloc;
    }

    bytes_read = read(chardev_fd, rd_buf, 0);
    printf("bytes_read: %ld\n", bytes_read);
err_malloc:
    close(chardev_fd);
    return err;
    
}
