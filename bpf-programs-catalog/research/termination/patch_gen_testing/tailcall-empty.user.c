#include <stdio.h>
#include <sys/syscall.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>

int main() {
    
    struct bpf_object *obj = bpf_object__open("tailcall-empty.kern.o"); 

    if (bpf_object__load(obj)) {
        printf("Failed to load the program\n");
        return 0;
    }

    struct bpf_program *prog1 = bpf_object__find_program_by_name(obj, 
                                "bpf_prog_trigger_syscall_prog");
    struct bpf_program *prog2 = bpf_object__find_program_by_name(obj, 
                                "bpf_prog_testing_tail_func");
    if (!prog1) {
        printf("Failed to find the prog1\n");
        goto cleanup;
    }

    if (!prog2) {
        printf("Failed to find the prog2\n");
        goto cleanup;
    }

    struct bpf_link *link1 = bpf_program__attach(prog1);
    struct bpf_link *link2 = bpf_program__attach(prog2);

    if (libbpf_get_error(link1)) {
        printf("Failed to attach the program1\n");
        bpf_link__destroy(link1);
        link1 = NULL;
        goto cleanup;
    }
    
    if (libbpf_get_error(link2)) {
        printf("Failed to attach the program2\n");
        bpf_link__destroy(link2);
        link2 = NULL;
        goto cleanup;
    }

    printf("Attachment of both programs is done\n");

    while(1) {
        // do nothing
    }
    
cleanup:
    bpf_object__close(obj);
    return 0;
}
