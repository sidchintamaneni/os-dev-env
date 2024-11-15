obj-m := kfunc-samples.o

# For generating the BTF information for the module
KBUILD_CFLAGS += -g -O2

all: modules bpf loader

bpf: bpf-kfunc.kern.o

bpf-kfunc.kern.o: bpf-kfunc.kern.c
	clang -I/linux/usr/include/ -I/linux/tools/lib/ -target bpf -g -Wall -O2 -c -o $@ $<

loader: bpf-kfunc.user

bpf-kfunc.user: bpf-kfunc.user.c
	clang -g -I/linux/usr/include -I/linux/tools/lib/ -L/linux/tools/lib/bpf/ $< -lbpf -o $@

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f bpf-kfunc.kern.o bpf-kfunc.user


