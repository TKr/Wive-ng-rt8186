CC=mips-linux-gcc
STRIP=mips-linux-strip
OBJCOPY=mips-linux-objcopy

all :	wl

wl :	wl.o
	$(CC) -Os -o wl wl.o
	$(STRIP) wl
	$(OBJCOPY) --strip-all wl wl

scanner.o :	wl.c
	$(CC) -c wl.c

install :
	rm -rfd filesystem
	mkdir filesystem
	mkdir filesystem/bin
	cp wl filesystem/bin

clean :
	rm -f *.o wl
	rm -rfd filesystem
    