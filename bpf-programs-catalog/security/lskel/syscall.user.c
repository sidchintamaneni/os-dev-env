#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <bpf/libbpf_common.h>
#include <errno.h>
#include "syscall.kern.lskel.h"

struct args {
	__u64 log_buf;
	__u32 log_size;
	int max_entries;
	int map_fd;
	int prog_fd;
	int btf_fd;
};

int main(int argc, char **argv)
{
	static char verifier_log[8192];
	struct args ctx = {
		.max_entries = 1024,
		.log_buf = (uintptr_t) verifier_log,
		.log_size = sizeof(verifier_log),
	};
	LIBBPF_OPTS(bpf_test_run_opts, tattr,
		.ctx_in = &ctx,
		.ctx_size_in = sizeof(ctx),
	);
	struct syscall_kern *skel = NULL;
	__u64 key = 12, value = 0;
	int err, prog_fd;

	skel = syscall_kern__open_and_load();
	if (!skel) 
		goto cleanup;

	prog_fd = skel->progs.bpf_prog_trigger_syscall_prog.prog_fd;
	err = bpf_prog_test_run_opts(prog_fd, &tattr);

	err = bpf_map_lookup_elem(ctx.map_fd, &key, &value);
	
	printf("Printing log buf\n %s", (char *)ctx.log_buf);
cleanup:
	syscall_kern__destroy(skel);
	if (ctx.prog_fd > 0)
		close(ctx.prog_fd);
	if (ctx.map_fd > 0)
		close(ctx.map_fd);
	if (ctx.btf_fd > 0)
		close(ctx.btf_fd);
	return 0;
}
