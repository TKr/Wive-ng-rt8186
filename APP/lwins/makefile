CC = mips-linux-gcc
OBJS = config.o debug.o discover.o netbt.o replicate.o server.o
HDR = lwins.h
FLAGS = -Os -fPIC -Wall -Wshadow -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
	-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
	-Wstrict-prototypes

.PHONY: all
all: install

.PHONY: test
test: lwins
	@echo [TEST] $<
	@sudo ./lwins --config lwins.conf

.PHONY: clean
clean:
	@$(RM) $(wildcard lwins $(OBJS))

install: $(OBJS)
	@echo [LD] $@
	@$(CC) -o lwins $(OBJS)

%.o: %.c $(HDR)
	@echo [CC] $<
	@$(CC) $(FLAGS) -I. -c $< -o $@

