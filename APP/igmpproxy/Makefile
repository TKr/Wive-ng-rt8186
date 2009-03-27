CC=mips-linux-gcc
BINDIR=./filesystem/bin
ETCDIR=./filesystem/etc
MANDIR=./filesystem/usr/share/man
CFLAGS=-Os -fPIC -fomit-frame-pointer -mno-memcpy

default : build.h igmpproxy

all	: igmpproxy

clean	:
	rm -f *.o *.asm build.h igmpproxy  

install : 
	cp igmpproxy ${BINDIR}
	if [ ! -e ${ETCDIR}/igmpproxy.conf ]; then cp igmpproxy.conf ${ETCDIR}; fi

igmpproxy : igmpproxy.o config.o confread.o request.o udpsock.o mcgroup.o rttable.o \
			igmp.o ifvc.o callout.o kern.o syslog.o lib.o mroute-api.o

build.h	:  
	echo '#define BUILD "' `date +%y%m%d` '"' >build.h
