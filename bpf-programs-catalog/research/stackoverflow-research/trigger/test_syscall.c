#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define SYS_nested_bpf_test 462

int main() {

    long res = syscall(SYS_nested_bpf_test, 7);
    printf("Syscall returned %ld\n", res);

    return 0;
}
