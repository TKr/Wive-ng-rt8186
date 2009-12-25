VPATH = src
CFLAGS ?= CFLAGS += -pipe -Wall -Wno-unused -Os -g
LD= $(CC)
OS_LAYER = osl-linux.c

all: lld2d lld2test

include src/common.mk

install:
	echo OK

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o lld2d lld2test

depend:
	$(CC) $(CFLAGS) -M $(DCFILES) >.depend

-include .depend
