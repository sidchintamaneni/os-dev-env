/*
 * Credits: Eli Bendersky
 * https://github.com/eliben/libjit-samples/blob/master/basic_self_jit_x64.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define TEXT_SIZE 4096

void* alloc_writable_memory(size_t size)
{ 
	// Can I use memalign here instead of mmap
	void *buf = mmap(NULL, size,
					PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (buf == (void *)-1){
		perror("MMAP\n");
		return NULL;
	}

	return buf;
}

int make_memory_executable(void* buf, size_t size) {

	if (mprotect(buf, size, PROT_READ | PROT_EXEC) == -1) {
		perror("mprotect");
		return -1;
	}

  return 0;
}

size_t emit_code_into_memory(unsigned char *buf) {
	unsigned char code[] = {
		0x55,						/* push rbp */
		0x48, 0x89, 0xe5,			/* mov rsp, rbp */
		0x48, 0x89, 0xf8,			/* mov rdi, rax */
		0x48, 0x83, 0xc0, 0x04,		/* add $4, rax */
		0xc9,						/* leave */
		0xc3						/* ret */
	};
	memcpy(buf, code, sizeof(code));

	return sizeof(code);
}

int main()
{
	void *buf = alloc_writable_memory(TEXT_SIZE);
	emit_code_into_memory(buf);
	make_memory_executable(buf, TEXT_SIZE);

	long (*jited_text)(long);

	jited_text = buf;
	int result = jited_text(2);
	printf("result = %d\n", result);

	munmap(buf, TEXT_SIZE);
	
	return 0;
}

