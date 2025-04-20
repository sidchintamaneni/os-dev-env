LINUX := /linux
SOURCES := $(wildcard *.kern.c)
FILES := $(SOURCES:.c=.o)
USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c
INCLUDE := -I$(LINUX)/usr/include/ -I$(LINUX)/tools/lib/
USER-CFLAGS := -I$(LINUX)/usr/include -I$(LINUX)/tools/lib/ -L$(LINUX)/tools/lib/bpf/

obj-m := kmod_st_ops.o

all: vmlinux-header $(FILES) $(USER)
	
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) -o $@ $<
	bpftool gen skeleton bpf_test_st_ops.kern.o > bpf_test_st_ops.skel.h

$(USER) : % : %.c
	clang $(USER-CFLAGS) $< -lbpf -o $@

vmlinux-header:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

.PHONY : clean

clean :
	rm $(FILES) $(USER) *.skel.h

modules-clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
