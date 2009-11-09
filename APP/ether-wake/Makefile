CC=mips-linux-gcc -Os -fPIC
STRIP=mips-linux-strip -s

EXEC = ether-wake
OBJS = ether-wake.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

install:
	echo "OK"

clean:
	-rm -f $(EXEC) *.elf *.o

