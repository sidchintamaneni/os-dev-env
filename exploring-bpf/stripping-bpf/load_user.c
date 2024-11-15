#include <stdio.h>
#include <bpf/libbpf.h>

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Expected: ./load.user {file_name}.kern.c bpf_program_name\n");
		return -1;
	}

	char *bpf_path = argv[1];
    char *prog_name = argv[2];
	struct bpf_object *obj = bpf_object__open(bpf_path);

	if(bpf_object__load(obj)) {
		printf("ERROR: Failed to verify the program\n");
	    goto cleanup;
    }

    struct bpf_program *program = bpf_object__find_program_by_name(obj, prog_name);
    if (program == NULL) {
		printf("ERROR: Failed to find the program\n");
        goto cleanup;
    }

    struct bpf_link *link = bpf_program__attach(program);
    if(libbpf_get_error(link)) {
		printf("ERROR: Failed to attach the program: %ld\n", libbpf_get_error(link));
        goto cleanup;
    }
	
    while(1) {
    }

cleanup:
	bpf_object__close(obj);
	return 0;
}
