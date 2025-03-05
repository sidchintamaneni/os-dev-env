#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

uint64_t r[1] = {0xffffffffffffffff};

int main(void)
{
	void *addr;

	addr = (void *)syscall(__NR_mmap, /*addr=*/0x20000000ul, /*len=*/0x1000000ul,
				/*prot=PROT_WRITE|PROT_READ|PROT_EXEC*/ 7ul,
				/*flags=MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE*/ 0x32ul, /*fd=*/-1,
				/*offset=*/0ul);
	if (addr == MAP_FAILED) {
		printf("MMAP Failed\n");
		return 1;
	}
	printf("Memory mapped addr: %p\n", addr);

	intptr_t res = 0;
	
	if (write(1, "executing program\n", sizeof("executing program\n") - 1)) {
	
	}
	
	memcpy((void*)0x200000c0, "/dev/ppp\000", 9);

	res = syscall(
  	    __NR_openat, /*fd=*/0xffffffffffffff9cul, /*file=*/0x200000c0ul,
  	    /*flags=O_SYNC|O_NOFOLLOW|O_NOATIME|O_CREAT|O_CLOEXEC|0x2*/ 0x1e1042,
  	    /*mode=*/0);
  	if (res != -1)
  	  r[0] = res;

  	*(uint32_t*)0x20000140 = 0;
  	syscall(__NR_ioctl, /*fd=*/r[0], /*cmd=*/0xc004743e, /*arg=*/0x20000140ul);
  	*(uint16_t*)0x20002a40 = 2;
  	*(uint64_t*)0x20002a48 = 0x200007c0;
  	*(uint16_t*)0x200007c0 = 0x40;
  	*(uint8_t*)0x200007c2 = 0;
  	*(uint8_t*)0x200007c3 = 0;
  	*(uint32_t*)0x200007c4 = 0;
  	*(uint16_t*)0x200007c8 = 6;
  	*(uint8_t*)0x200007ca = 0;
  	*(uint8_t*)0x200007cb = 0;
  	*(uint32_t*)0x200007cc = 2;
  	syscall(__NR_ioctl, /*fd=*/r[0], /*cmd=*/0x40107447, /*arg=*/0x20002a40ul);

  	memset((void*)0x20000900, 0, 2);
  	syscall(__NR_write, /*fd=*/r[0], /*data=*/0x20000900ul, /*size=*/2ul);

	close(res);

	munmap(addr, /*len=*/0x1000000ul);

	return 0;

}
