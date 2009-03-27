CFLAGS +=  -D_GNU_SOURCE
# To cross-compile
CC = mips-linux-gcc
STRIP = mips-linux-strip
CFLAGS += -Os -fPIC -fomit-frame-pointer -mno-memcpy
LDFLAGS += -s

all: ntpclient

test: ntpclient
	./ntpclient -d -r <test.dat

ntpclient: ntpclient.o phaselock.o

adjtimex: adjtimex.o

clean:
	rm -f ntpclient adjtimex *.o
	rm -rdf filesystem

install:
	rm -rfd filesystem
	mkdir filesystem
	mkdir filesystem/bin
	$(STRIP) ntpclient
	cp ntpclient filesystem/bin/ntpclient
