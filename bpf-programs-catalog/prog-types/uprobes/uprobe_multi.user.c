#include <stdio.h>
#include <unistd.h>
#include "uprobe_multi.kern.skel.h"

__attribute__((__noinline__)) void uprobe_multi_func_1(void)
{
	asm volatile("");
}

__attribute__((__noinline__)) void uprobe_multi_func_2(void)
{
	asm volatile("");
}

int main(void) 
{

	LIBBPF_OPTS(bpf_uprobe_multi_opts, uprobe_multi_opts);
	struct uprobe_multi_kern *skel;

	skel = uprobe_multi_kern__open_and_load();
	if (!skel) {
		printf("failed to open and load\n");
		return 0;
	}

    skel->links.uprobe = bpf_program__attach_uprobe_multi(
							skel->progs.uprobe, getpid(), "/proc/self/exe",
        					"uprobe_multi_func_*", &uprobe_multi_opts);

    if (!skel->links.uprobe) {
		printf("failed to attach\n");
		goto cleanup;
	}
	uprobe_multi_func_1();
	uprobe_multi_func_2();

	// auto-attach can be done using 
	// uprobe_kern__attach(skel)

cleanup:
	uprobe_multi_kern__destroy(skel);	
	return 0;
}
