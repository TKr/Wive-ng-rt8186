CC=gcc
STRIP=strip

all :	cvimg

cvimg :	cvimg.o
	$(CC) -O3 -o cvimg cvimg.o
	$(STRIP) cvimg
	rm cvimg.o
	make -C squashfs
	make -C sstrip

clean:
	rm -f cvimg
	make -C squashfs clean
	make -C sstrip clean
