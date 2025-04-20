#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define KMOD_ST_OPS_PATH "/dev/kmod_st_ops"

int main() 
{
    int kmod_st_ops_fd, err;
    ssize_t bytes_read; 
    
    kmod_st_ops_fd = open(KMOD_ST_OPS_PATH, O_RDWR);
    if (kmod_st_ops_fd < 0) {
        fprintf(stderr, "Failed to open the file\n");
        return 0;
    }
    return 0;
}
