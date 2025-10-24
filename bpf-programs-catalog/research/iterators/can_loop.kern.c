#include "bpf_experimental.h"


SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {
	
	// this is invoking timed_may_goto
	while(can_loop) {
	}
    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

