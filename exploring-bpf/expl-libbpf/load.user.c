#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>

struct elf_fd {
	Elf *elf;
	int fd;
};


int elf_open(const char *binary_path, struct elf_fd *elf_fd)
{
	int fd, ret;
	Elf *elf;

	elf_fd->elf = NULL;
	elf_fd->fd = -1;

	fd = open(binary_path, O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		ret = -1;
		printf("elf: failed to open %s\n", binary_path);
		return ret;
	}
	elf = elf_begin(fd, ELF_C_READ, NULL);
	if (!elf) {
		printf("elf: could not read elf from %s: %s\n", binary_path, elf_errmsg(-1));
		close(fd);
		return -1;
	}
	elf_fd->fd = fd;
	elf_fd->elf = elf;
	return 0;
}

void elf_close(struct elf_fd *elf_fd)
{
	if (!elf_fd)
		return;
	elf_end(elf_fd->elf);
	close(elf_fd->fd);
}

int main(int argc, char *argv[]) {
    
	int err = 0;
	struct elf_fd elf_fd;

    if (argc != 2) {
		printf("Expected: ./load.user {file_name}.kern.c\n");
		return -1;
    }

	err = elf_open(argv[1], &elf_fd);
	if(!err) {
		printf("Failed to open the elf file\n");
		return -1;
	}

	elf_close(&elf_fd);

	return 0;

}
