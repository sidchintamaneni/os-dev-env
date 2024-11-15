#include <stdio.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
		printf("Expected: ./load.user {file_name}.kern.c\n");
		return -1;
    }

    char *bpf_path = argv[1];
}
