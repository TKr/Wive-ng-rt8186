# $Id: Makefile,v 1.1 2007/05/15 14:34:17 yachang Exp $

.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean all depend

LIBS = 
CFLAGS = -g -Os -Wall
CPPFLAGS =  -I. -I.
DEPEND = .depend
LDFLAGS	= -g -s
CC = mips-linux-gcc
CPP = mips-linux-gcc -E
#AR = mips-uclibc-ar

SOURCES = minixml.c, upnphttp.c, upnpreplyparse.c, upnpsoap.c \
	mini_upnp.c		
OBJS = $(SOURCES:.c=.o)

UPNP_DAEMON = mini_upnpd

ifeq ($(STATIC_LIB),1)
########## Build Static Library ##################
all: mini_upnp.a	

UPNP_LIB = mini_upnp.a

mini_upnp.a: mini_upnp.o minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
	$(AR) rcs $@ mini_upnp.o minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o

else
########## Build Shared Library ##################
all: $(UPNP_DAEMON) $(UPNP_LIB)	

UPNP_LIB = mini_upnp.so

CFLAGS += -DUSE_SHARED_DAEMON

minixml.o: minixml.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<

upnphttp.o: upnphttp.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<	

upnpreplyparse.o: upnpreplyparse.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<		
	
upnpsoap.o: upnpsoap.c
	$(CC) -c -o $@ -fpic $(CFLAGS) $(IFLAGS) $<		
	
$(UPNP_LIB): minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o
	$(CC) -s -shared -o $@ minixml.o upnphttp.o upnpreplyparse.o upnpsoap.o

$(UPNP_DAEMON): mini_upnp.o $(UPNP_LIB)
	$(CC) -o $@ $(APMIB_LIB) $^ $(LDFLAGS) $(LIBS)

endif


clean:
	rm -f *.o *.so *.a $(UPNP_DAEMON)

romfs:
	$(ROMFSINST) /lib/$(UPNP_LIB)
	$(ROMFSINST) /etc/simplecfgservice.xml
# depend stuff
depend: $(SOURCES)
	$(CPP) $(CPPFLAGS) -MM $^ > $(DEPEND)
        
-include $(DEPEND)

# tags
tags:	$(SOURCES)
	ctags -o tags $^ *.h

.c.o:
	${CC} -c -o $@ $(CFLAGS) $(IFLAGS) $<




