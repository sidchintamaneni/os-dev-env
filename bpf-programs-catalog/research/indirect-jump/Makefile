LINUX := /linux
SOURCES := $(wildcard *.kern.c)
FILES := $(SOURCES:.c=.o)
USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
BPF-CLANG := clang-22
BPF_CLANG_CFLAGS := -target bpf -Wall
SPEC_CLANG_FLAGS := -mcpu=v4 -mllvm -bpf-min-jump-table-entries=5
INCLUDE := -I$(LINUX)/usr/include/ -I$(LINUX)/tools/lib/
USER-CFLAGS := -I$(LINUX)/usr/include -I$(LINUX)/tools/lib/ -L$(LINUX)/tools/lib/bpf/

all: $(FILES) $(USER)
	
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) $(SPEC_CLANG_FLAGS) -S $<
	$(BPF-CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) $(SPEC_CLANG_FLAGS) -c -o $@ $<

$(USER) : % : %.c
	clang $(USER-CFLAGS) $< -lbpf -o $@

.PHONY : clean

clean :
	rm $(FILES) $(USER) *.s
