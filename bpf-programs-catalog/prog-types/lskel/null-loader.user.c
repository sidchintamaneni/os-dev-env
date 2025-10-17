
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
	if (!skel) {
		printf("Error!!");
		return -1;
	}

	int err = null_kern__attach(skel);
	printf("After attach - err %d\n", err);
	if (err < 0) {
		printf("Error during attachment");
	}

	while(1);

	null_kern__destroy(skel);
	return 0;
}
