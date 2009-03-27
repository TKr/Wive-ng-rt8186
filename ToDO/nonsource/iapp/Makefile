$CC=mips-linux-gcc

IAPPEXEC = iapp
IAPPBIN = iapp-0.6.obj
IAPPOBJS = iapp.o version.o

all: $(IAPPEXEC)

$(IAPPEXEC): $(IAPPBIN)
	cp $(IAPPBIN) $(IAPPEXEC) 
	chmod 755 $(IAPPEXEC)

RTL8185_SRC=n
ifeq ($(RTL8185_SRC),y)
$(IAPPBIN): $(IAPPOBJS)
	$(CC) $(LDFLAGS) -o $@ $(IAPPOBJS) $(LDLIBS$(LDLIBS-$(@)))
endif

clean:
	-rm -f $(IAPPEXEC) *.elf *.gdb *.o

