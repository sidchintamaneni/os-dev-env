# SPDX-License-Identifier: GPL-2.0
CC      ?= gcc
CFLAGS  ?= -O2 -Wall -Wextra -static
TARGETS := dirty-pagecache

all: $(TARGETS)

dirty-pagecache: dirty-pagecache.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
