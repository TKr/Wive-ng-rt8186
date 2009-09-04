DESTDIR = $(INSTROOT)/opt/Wive/APP/ppp/filesystem
BINDIR = $(DESTDIR)/sbin
MANDIR = $(DESTDIR)/share/man/man8

#DO_DEFLATE=y
DO_BSD_COMPRESS=y
HAVE_ZLIB=y

OPTS := -Os -Wall
CFLAGS := -I../include/net
OBJS := pppdump.o
LIBS :=

ifdef DO_DEFLATE
CFLAGS += -DDO_DEFLATE=1
OBJS += deflate.o
ifdef HAVE_ZLIB
LIBS += -lz
else
OBJS += zlib.o
endif
else
CFLAGS += -DDO_DEFLATE=0
endif

ifdef DO_BSD_COMPRESS
CFLAGS += -DDO_BSD_COMPRESS=1
OBJS += bsd-comp.o
else
CFLAGS += -DDO_BSD_COMPRESS=0
endif

INSTALL= install

all:	pppdump

pppdump: $(OBJS)
	$(CC) $(CFLAGS) $(OPTS) -o pppdump $(OBJS) $(LIBS)

clean:
	rm -f pppdump $(OBJS) *~

install:
	mkdir -p $(BINDIR) $(MANDIR)
	$(INSTALL) -c pppdump $(BINDIR)
	$(INSTALL) -c -m 444 pppdump.8 $(MANDIR)
