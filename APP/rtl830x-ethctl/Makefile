CC=mips-linux-gcc -Os -fPIC
STRIP=mips-linux-strip -s

UTILSLIST=ethctl

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
	@set -e; 
	for i in $(UTILSLIST); do cp $$i ./filesystem/bin/$$i; done
