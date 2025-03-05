LINUX_PATH := ../../linux
SOURCES := $(wildcard *.kern.c)
FILES := $(SOURCES:.c=.o)
USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c
INCLUDE := -I$(LINUX_PATH)/usr/include/ -I$(LINUX_PATH)/tools/lib/
USER-CFLAGS := -g -I$(LINUX_PATH)/usr/include -I$(LINUX_PATH)/tools/lib/ -L$(LINUX_PATH)/tools/lib/bpf/


all: $(FILES) $(USER)
	
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) -o $@ $<

$(USER) : % : %.c
	gcc $(USER-CFLAGS) $< -lelf -lbpf -o $@

.PHONY : clean

clean :
	rm $(FILES) $(USER)
