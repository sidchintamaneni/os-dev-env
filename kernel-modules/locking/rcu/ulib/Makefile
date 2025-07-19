SRC := $(wildcard *.c)

OBJ := $(SRC:.c=.o)

BIN := $(SRC:.c=)

CC := gcc
CFLAGS := -Wall -Wextra -O2

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)
