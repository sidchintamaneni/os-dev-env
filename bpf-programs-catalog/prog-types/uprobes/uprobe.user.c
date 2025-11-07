#include <stdio.h>
#include <unistd.h>
#include "uprobe.kern.skel.h"

__attribute__((__noinline__)) void uprobe_multi_func_1(void)
{
	asm volatile("");
}

int main(void) 
{

	LIBBPF_OPTS(bpf_uprobe_opts, uprobe_opts);
	struct uprobe_kern *skel;

	skel = uprobe_kern__open_and_load();
	if (!skel) {
		printf("failed to open and load\n");
		return 0;
	}

	uprobe_opts.func_name = "uprobe_multi_func_1";
	skel->links.uprobe = bpf_program__attach_uprobe_opts(skel->progs.uprobe,
															getpid(), "/proc/self/exe", 0, &uprobe_opts);

	if (!skel->links.uprobe) {
		printf("failed to attach\n");
		goto cleanup;
	}
	uprobe_multi_func_1();

	// auto-attach can be done using 
	// uprobe_kern__attach(skel)

cleanup:
	uprobe_kern__destroy(skel);	
	return 0;
}
