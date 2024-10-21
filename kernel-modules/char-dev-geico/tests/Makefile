SRC := $(wildcard *.test.c)
TESTS := $(SRC:.c=)


all: $(TESTS)

$(TESTS) : % : %.c
	gcc $< -o $@

.PHONY : clean

clean :
	rm $(TESTS)
