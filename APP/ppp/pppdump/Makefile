CC = mips-linux-gcc
DESTDIR = $(INSTROOT)/opt/Wive/APP/ppp/filesystem
BINDIR = $(DESTDIR)/sbin
MANDIR = $(DESTDIR)/share/man/man8

CFLAGS= -Os -fPIC -I../include/net -I../include/linux -fomit-frame-pointer -mno-memcpy
OBJS = pppdump.o bsd-comp.o deflate.o zlib.o

INSTALL= install

all:	pppdump

pppdump: $(OBJS)
	$(CC) -o pppdump $(OBJS)

clean:
	rm -f pppdump $(OBJS) *~

install:
	mkdir -p $(BINDIR) $(MANDIR)
	$(INSTALL)  -c pppdump $(BINDIR)
	$(INSTALL) -c -m 444 pppdump.8 $(MANDIR)
