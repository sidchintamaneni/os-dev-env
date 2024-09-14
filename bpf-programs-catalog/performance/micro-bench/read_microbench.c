#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_ITER 100000
#define MEASUREMENTS 100

uint64_t run()
{
    struct timespec begin, end;
    char buf[256];
    const char *filename = "/dev/null";
    int fd = open(filename, O_RDONLY);

    // Warm up before benchmarking
    for (int i = 0; i < NUM_ITER; i++)
        read(fd, buf, sizeof(buf));

    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    for (int i = 0; i < NUM_ITER; i++)
        read(fd, buf, sizeof(buf));
        
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t time = (end.tv_sec - begin.tv_sec) * 1000000000UL + (end.tv_nsec - begin.tv_nsec);
    close(fd);

    return time;
}

int main(void){
    printf("PID: %d\n", getpid());
    uint64_t results[MEASUREMENTS];
    for (int i = 0; i < MEASUREMENTS; i++)
    {
        results[i] = run();
        printf("%lu\n", results[i]);
    }
    return 0;
}
