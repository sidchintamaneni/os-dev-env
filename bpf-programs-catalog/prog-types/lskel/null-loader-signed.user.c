
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <linux/keyctl.h>
#include <stddef.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include  "null.kern.sign-lskel.h"
#include "verification_cert.h"

static __u32 register_session_key(const char *key_data, size_t key_data_size)
{
	return syscall(__NR_add_key, "asymmetric", "libbpf_session_key",
			(const void *)key_data, key_data_size, KEY_SPEC_SESSION_KEYRING);
}


int main(int argc, char **argv)
{
	struct null_kern *skel;
	int err;

	err = register_session_key((const char *)bpf_progs_verification_cert,
				   bpf_progs_verification_cert_len);
	if (err < 0) {
		printf("failure in registering session key");
		return err;
	}

	skel = null_kern__open();
	if (!skel) {
		printf("Error!!\n");
		return -1;
	}

	skel->keyring_id = KEY_SPEC_SESSION_KEYRING;
	printf("key ring: %d\n", KEY_SPEC_SESSION_KEYRING);
	err = null_kern__load(skel);
	if (err < 0) {
		printf("Error during loading - err: %d\n", err);
		return -1;
	}

	err = null_kern__attach(skel);
	if (err < 0) {
		printf("Error during attachment\n");
	}

	while(1);

	null_kern__destroy(skel);
	return 0;
}
