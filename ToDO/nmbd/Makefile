# Router rc Makefile
#

CC = mips-uclibc-gcc

AR = mips-uclibc-ar

STRIP = mips-uclibc-strip

CFLAGS = -I./ 
LDFLAGS = 

OBJS :=  sserver.o 

all: nmbd

.c.o:
	$(CC) $(CFLAGS) -c $<
		

clean:
	rm -f *.o *~ nmbd .*.depend

nmbd: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
#	$(CC) -o $@ $(OBJS) $(CFLAGS)
