CC=mips-linux-gcc -Os
CRYPTLIB = portingssl
WSCSRC = src

all:
	make -C $(CRYPTLIB)
	make -C $(WSCSRC)
	
clean:
	make -C $(CRYPTLIB) clean
	make -C $(WSCSRC) clean

romfs:
	$(ROMFSINST) /etc/wscd.conf
	$(ROMFSINST) src/wscd /bin
	
