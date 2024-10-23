#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"
#define BUFFER_SIZE 2048 
#define EXCEED_SIZE 64

/*
 * Testing read and write syscall by filling the entire
 * buffer
 */ 
int main() {
    int chardev_fd, err;
    ssize_t bytes_written, bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }
    
    char *wr_buf = malloc(BUFFER_SIZE + EXCEED_SIZE);
    if (!wr_buf) {
        err = -1;
        goto err_malloc;
    }
    char *rd_buf = malloc(BUFFER_SIZE + EXCEED_SIZE);
    if (!rd_buf) {
        err = -1;
        free(wr_buf);
        goto err_malloc;
    }
    memset(wr_buf, 0xAF, BUFFER_SIZE);
    memset(wr_buf + BUFFER_SIZE, 0xDE, EXCEED_SIZE); 
    bytes_written = write(chardev_fd, wr_buf, BUFFER_SIZE + EXCEED_SIZE);

    if (lseek(chardev_fd, 0, SEEK_SET) < 0) {
        fprintf(stderr, "Failed llseek\n");
        err = -1;
        goto err_llseek;
    }

    bytes_read = read(chardev_fd, rd_buf, EXCEED_SIZE);

//    for (int i = 0; i < 8; i++) {
//        printf("%lx", *((unsigned long *)(wr_buf + ((i * 8) + BUFFER_SIZE) )));
//    }
//    printf("\n");
//    
//    for (int i = 0; i < 8; i++) {
//        printf("%lx", *((unsigned long *)(rd_buf + i * 8)));
//    }   
//    printf("\n");
    
    /*
     * Check for the wrapped memory
     */
    if (memcmp(wr_buf + BUFFER_SIZE, rd_buf, EXCEED_SIZE) == 0) {
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
