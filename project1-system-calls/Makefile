CC=gcc
CFLAGS=-Wall -Wextra -Wfloat-equal -Wshadow -Wstrict-overflow=2 -Wwrite-strings -Wswitch-default -Wswitch-enum -O2 -pipe


fcopy: jacksonmiller_fcopy.c
	$(CC) $^ -o $@

fcopy_byte: jacksonmiller_fcopy.c
	$(CC) $^ -DBYTE_COPY -o $@

all: fcopy fcopy_byte

.PHONY: clean

clean:
	rm *.o srec