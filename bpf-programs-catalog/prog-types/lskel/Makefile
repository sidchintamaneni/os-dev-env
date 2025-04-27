LINUX := /linux
SOURCES := $(wildcard *.kern.c)
FILES := $(SOURCES:.c=.o)
USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
USER_SKEL := $(SOURCES:.c=.skel.h)
USER_LSKEL := $(SOURCES:.c=.lskel.h)
BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c
INCLUDE := -I$(LINUX)/usr/include/ -I$(LINUX)/tools/lib/ -I$(LINUX)/include/ -I$(LINUX)/tools/include
USER-CFLAGS := -I$(LINUX)/usr/include -I$(LINUX)/tools/lib/ -L$(LINUX)/tools/lib/bpf/

all: $(FILES) $(USER_SKEL) $(USER_LSKEL) $(USER)
	
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) -o $@ $<

$(USER) : % : %.c
	clang $(USER-CFLAGS) $< -lbpf -o $@

$(USER_SKEL) : %.skel.h : %.o
	bpftool gen skeleton $<  > $@

$(USER_LSKEL) : %.lskel.h : %.o
	bpftool gen skeleton -L $<  > $@

.PHONY : clean

clean :
	rm $(FILES) $(USER_SKEL) $(USER_LSKEL) $(USER)

