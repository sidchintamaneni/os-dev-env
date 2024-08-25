#include <stdio.h>
#include <sys/syscall.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>

int main() {
    
    struct bpf_object *obj = bpf_object__open("call_helper_nested_test.kern.o"); 

    if (bpf_object__load(obj)) {
        printf("Failed to load the program\n");
        return 0;
    }

    struct bpf_program *prog = bpf_object__find_program_by_name(obj, 
                                "trigger_syscall_prog");
    if (!prog) {
        printf("Failed to find the prog\n");
        goto cleanup;
    }

    struct bpf_link *link = bpf_program__attach(prog);

    if (libbpf_get_error(link)) {
        printf("Failed to attach the program\n");
        bpf_link__destroy(link);
        link = NULL;
        goto cleanup;
    }
    
    printf("Attachment done\n");

    while(1) {
        // do nothing
    }
    
cleanup:
    bpf_object__close(obj);
    return 0;
}
