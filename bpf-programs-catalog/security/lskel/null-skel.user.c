
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <errno.h>
#include  "null.kern.skel.h"

char log_buf[1024 * 1024];

int main(int argc, char **argv)
{
	struct null_kern *skel;
	int err;
	LIBBPF_OPTS(bpf_object_open_opts, opts, .kernel_log_buf = log_buf,
						.kernel_log_size = sizeof(log_buf),
						.kernel_log_level = 1);

	skel = null_kern__open_opts(&opts);
	if (!skel)
		return -1;

	err = null_kern__load(skel);
	if (err < 0) {
		printf("Verifier log error\n");
		printf("%s", log_buf);
	}

	null_kern__destroy(skel);

	return 0;
}
