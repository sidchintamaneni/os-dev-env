SRC := $(wildcard *.test.c)
TESTS := $(SRC:.c=)


all: $(TESTS)

$(TESTS) : % : %.c
	gcc $< -o $@

.PHONY : clean run_tests

clean :
	rm $(TESTS)

run_tests:
	./run_tests.sh
