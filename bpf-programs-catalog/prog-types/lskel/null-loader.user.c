
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <errno.h>
#include  "null.kern.lskel.h"

int main(int argc, char **argv)
{
	struct null_kern *skel;

	skel = null_kern__open_and_load();
	if (!skel)
		return -1;

	null_kern__destroy(skel);
	return 0;
}
