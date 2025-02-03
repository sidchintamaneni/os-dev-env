

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>

#define CHARDEV_PATH "/dev/chardev"

int main() {
    int chardev_fd, err;
    ssize_t bytes_read; 
    
    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }

    bytes_read = read(chardev_fd, NULL, 0);
    printf("bytes_read: %ld\n", bytes_read);

    return 0;
}
