#include <stdio.h>
#include <sys/syscall.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>

int main() {

	LIBBPF_OPTS(bpf_kprobe_multi_opts, opts);
    
    struct bpf_object *obj = bpf_object__open("bpf_prog1.kern.o"); 

    if (bpf_object__load(obj)) {
        printf("Failed to load the program\n");
        return 0;
    }

    struct bpf_program *prog1 = bpf_object__find_program_by_name(obj, 
                                "trigger_bpf_prog");
    if (!prog1) {
        printf("Failed to find the prog1\n");
        goto cleanup;
    }

    printf("Attaching the same program multiple times on same attachpoint\n");

	struct bpf_link *link1 = bpf_program__attach_kprobe_multi_opts(prog1,
			"bpf_get_current_pid_tgid", &opts);

    if (libbpf_get_error(link1)) {
        printf("Failed to attach the program1\n");
        goto cleanup;
    }

	struct bpf_link *link2 = bpf_program__attach_kprobe_multi_opts(prog1,
			"bpf_get_current_pid_tgid", &opts);
    
    if (libbpf_get_error(link2)) {
        printf("Failed to attach the program2\n");
        bpf_link__destroy(link1);
        link1 = NULL;
        goto link1_cleanup;
    }

    printf("Attachment of both programs is done\n");

    while(1) {
        // do nothing
    }

    bpf_link__destroy(link2);
    link2 = NULL;
    
link1_cleanup:
	bpf_link__destroy(link1);
	link1 = NULL;
cleanup:
    bpf_object__close(obj);
    return 0;
}
