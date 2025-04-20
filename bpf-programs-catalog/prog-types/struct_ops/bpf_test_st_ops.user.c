#include <stdio.h>
#include <sys/syscall.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>
#include "bpf_test_st_ops.skel.h"

int main() {
	
	struct bpf_test_st_ops_kern *skel;
	int err;

	skel = bpf_test_st_ops_kern__open_and_load();
	if (!skel) {
		printf("Failed prog load\n");
		return -1;
	}

	err = bpf_test_st_ops_kern__attach(skel);
	if (err < 0) {
		printf("Failed attach\n");
		return -1;
	}

	while(1);

	bpf_test_st_ops_kern__destroy(skel);

	return 0;
}
