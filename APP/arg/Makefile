CC=mips-linux-gcc -Os -fPIC
STRIP=mips-linux-strip -s

UTILSLIST=arg

all:
	@set -e;
	for i in $(UTILSLIST); do $(CC) $$i.c -o $$i; $(STRIP) $$i; done

clean:
	@set -e; 
	for i in $(UTILSLIST); do rm -rf $$i; done
	rm -rfd filesystem

install:
	rm -rfd filesystem
	mkdir filesystem
	mkdir filesystem/bin
	mkdir filesystem/etc
	@set -e; 
	cp arglist filesystem/etc
	for i in $(UTILSLIST); do cp $$i ./filesystem/bin/$$i; done
