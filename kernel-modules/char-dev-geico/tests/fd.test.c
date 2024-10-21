#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define CHARDEV_PATH "/dev/chardev"

int main() {
    int chardev_fd;

    chardev_fd = open(CHARDEV_PATH, O_RDWR);
    if (chardev_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return -1;
    }

    close(chardev_fd);

    fprintf(stdout, "Testcase - PASSED\n");

    return 0;
    
}
