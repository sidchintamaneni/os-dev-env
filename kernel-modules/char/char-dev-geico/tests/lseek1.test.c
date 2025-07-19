#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 8

/* 
 * Testing lseek syscall by using variable buffer
 * length
 */

int main() {
    int chardev_fd;
    ssize_t bytes_written, bytes_read; 
    int err = 0;
    
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
    memset(wr_buf, 0xAF, BUFFER_SIZE);
    memset(rd_buf, 0, BUFFER_SIZE);

    bytes_written = write(chardev_fd, wr_buf, BUFFER_SIZE);
    if (lseek(chardev_fd, 0, SEEK_SET) < 0) {
        fprintf(stderr, "Failed llseek\n");
        err = -1;
        goto err_llseek;
    }
    bytes_read = read(chardev_fd, rd_buf, BUFFER_SIZE);
    
    if (memcmp(wr_buf, rd_buf, BUFFER_SIZE) == 0) {
        fprintf(stdout, "Testcase - PASSED\n");
    } else {
        fprintf(stdout, "Testcase - FAILED\n");
    }

err_llseek:
    free(wr_buf);
    free(rd_buf);
err_malloc:
    close(chardev_fd);


    return err;
    
}
