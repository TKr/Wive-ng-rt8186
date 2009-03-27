CC=mips-linux-gcc 
CFLAGS=-Os -fPIC -Wall -g -fomit-frame-pointer

all:		bidi

clean:
		rm -f bidi

bidi:		bidi.c
		$(CC) $(CFLAGS) -o $@ $<
