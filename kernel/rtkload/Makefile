# The kernel tree you do builds in.

LDSCRIPT = ld.script

# Uncomment if you're building for the emulator
include ../.config
KERNEL_ROOT = ..

# ---- shouldn't need to modify below this line.
COPTIONS = -DROM_MEMORY -DCOMPRESSED_KERNEL
COPTIONS += -DRTL8186

ifdef BZ2_COMPRESS
	SOURCES = display.c misc.c hfload.c start.S 
else
	SOURCES = string.c ctype.c display.c misc.c hfload.c start.S read_memory.c 
endif
LOADER_FILES = hfload.o read_memory.o

ifdef BZ2_COMPRESS
	SUPPORT_FILES = display.o misc.o
else
	SUPPORT_FILES = vsprintf.o prom_printf.o string.o ctype.o display.o misc.o
endif

CFLAGS = -g -fno-pic -mno-abicalls $(EMUOPTS) -I$(KERNEL_ROOT)/include/linux -I$(KERNEL_ROOT)/include -I$(KERNEL_ROOT)/lib $(COPTIONS)
ASFLAGS = -g $(EMUOPTS) -I$(KERNEL_ROOT)/include

ifdef BZ2_COMPRESS
CFLAGS += -DBZ2_COMPRESS
ASFLAGS += -DBZ2_COMPRESS
endif

START_FILE = start.o

include make.def

CC = $(CROSSPATH)$(CROSS_LINUX)gcc -mno-abicalls -fno-pic -G 0
LD = $(CROSSPATH)$(CROSS_LINUX)ld -static -nostdlib
NM = $(CROSSPATH)$(CROSS_LINUX)nm

ifdef CONFIG_RTL8186_AP
LOAD_START_ADDR=0x80300000
else
LOAD_START_ADDR=0x80800000
endif
         
SEDFLAGS	= s/LOAD_ADDR/$(LOAD_START_ADDR)/;

all: linux.bin system.map 

clean:
	rm -f *.o memload system.map rtkrom memload-partial memload-full vmlinux_img.gz target target.img strip1 linux.bin vmlinux-stripped  $(LDSCRIPT) vmlinux_img vmlinux_img.gz.out


memload-partial: $(LOADER_FILES) $(SUPPORT_FILES) $(START_FILE) ../vmlinux
	$(CROSSPATH)$(CROSS_LINUX)gcc -fno-pic -mno-abicalls -Os -fomit-frame-pointer -D__MIPSEB__ -D__KERNEL__ -D__BIG_ENDIAN_BITFIELD -c vmlinux_img.c -o vmlinux_img.o
	$(CROSSPATH)$(CROSS_LINUX)objcopy --add-section .vmlinux=vmlinux_img.gz vmlinux_img.o
	@sed "$(SEDFLAGS)" < ld.script.in > $(LDSCRIPT)
	$(LD) -G 0 -T $(LDSCRIPT) -o memload-partial $(START_FILE) $(LOADER_FILES) $(SUPPORT_FILES) vmlinux_img.o
	$(NM) memload-partial | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aU] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | sort > system.map
	cp memload-partial memload-full


vmlinux_img.gz: ../vmlinux
	cp $(KERNEL_ROOT)/vmlinux vmlinux-stripped
	$(CROSSPATH)$(CROSS_LINUX)strip vmlinux-stripped
	$(CROSSPATH)$(CROSS_LINUX)strip -R .comment -R .note vmlinux-stripped
	$(CROSSPATH)$(CROSS_LINUX)objcopy -Obinary vmlinux-stripped vmlinux_img
	rm -f vmlinux_img.gz
ifdef BZ2_COMPRESS
	cat vmlinux_img | bzip2 -9v > vmlinux_img.gz
else
	cat vmlinux_img | gzip -9 -f > vmlinux_img.gz
endif

memload-full: vmlinux_img.gz memload-partial


rtkrom:   memload-full  ../vmlinux
	$(CROSSPATH)$(CROSS_LINUX)objcopy -Obinary memload-full rtkrom

linux.bin: rtkrom
	./cvimg linux rtkrom linux.bin $(LOAD_START_ADDR) 10000		#Erwin
	cp linux.bin ../../images/linux.bin					#Erwin
	#@./size_chk vmlinux_img $(LOAD_START_ADDR)

target:	rtkrom $(BOOT_CODE) $(EXT2_IMG)
# Pading boot code to 512 bytes long (but only 256 bytes are allowed, 512 is to make it page alignment)
	$(CROSSPATH)$(CROSS_LINUX)objcopy -Obinary --pad-to=0x80000200 $(BOOT_CODE) bootcode
	$(CROSSPATH)$(CROSS_LINUX)objcopy -Obinary --remove-section=.bss --remove-section=.data --remove-section=.mdebug  --pad-to=0x806FFE00 memload-full nandrom
	$(CROSSPATH)$(CROSS_LINUX)ld -G0 -Ttarget.script -o target  -bbinary bootcode -bbinary nandrom -bbinary $(EXT2_IMG)
	$(CROSSPATH)$(CROSS_LINUX)objcopy -Obinary target target.img		 
	cp -f ./target.img /tftpboot
depend:
	rm -f .depend
	$(CC) $(CFLAGS) -MM $(SOURCES) >.depend

bz2:
	make BZ2_COMPRESS=1
