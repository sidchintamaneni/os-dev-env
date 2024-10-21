#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "chardev_ioctl.h"

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 64

int main() {
    int chardev_fd, err;
    ssize_t bytes_written, bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }

    if (ioctl(chardev_fd, CHARDEV_IOC_CLEAR_ON_READ) < 0) {
        fprintf(stderr, "Failed ioctl\n");
        err = -1;
        goto err_malloc;
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
    char *tmp_buf = malloc(BUFFER_SIZE);
    if (!tmp_buf) {
        err = -1;
        free(wr_buf);
        free(rd_buf);
        goto err_malloc;
    }
    memset(wr_buf, 0xAF, BUFFER_SIZE);
    bytes_written = write(chardev_fd, wr_buf, BUFFER_SIZE);

    if (lseek(chardev_fd, 0, SEEK_SET) < 0) {
        fprintf(stderr, "Failed llseek\n");
        err = -1;
        goto err_llseek;
    }

    bytes_read = read(chardev_fd, rd_buf, BUFFER_SIZE);

    if (lseek(chardev_fd, 0, SEEK_SET) < 0) {
        fprintf(stderr, "Failed llseek2\n");
        err = -1;
        goto err_llseek;
    }

    bytes_read = read(chardev_fd, tmp_buf, BUFFER_SIZE);
    char tmp2_buf[64] = {0};
    if (memcmp(wr_buf, rd_buf, BUFFER_SIZE) == 0 && memcmp(tmp_buf, tmp2_buf, BUFFER_SIZE) == 0) {
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
