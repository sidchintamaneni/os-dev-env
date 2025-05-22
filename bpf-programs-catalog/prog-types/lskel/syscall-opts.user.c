
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <errno.h>
#include  "syscall.kern.lskel.h"

char log_buf[1024 * 1024];

int main(int argc, char **argv)
{
	struct syscall_kern *skel;
	int err;
	LIBBPF_OPTS(bpf_ctx_open_opts, opts, .kernel_log_buf = log_buf,
						.kernel_log_size = sizeof(log_buf),
						.kernel_log_level = 1);

	printf("main: syscall_kern__open_opts: \n");
	skel = syscall_kern__open_opts(&opts);
	if (!skel)
		return -1;

	err = syscall_kern__load(skel);
	printf("main: err :%d \n", err);
	if (err < 0) {
		printf("Verifier log error\n");
		printf("%s", log_buf);
	}

	syscall_kern__destroy(skel);
	return 0;
}
