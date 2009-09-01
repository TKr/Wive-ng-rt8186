include ../.config

KERNELSRC = $(WIVEROOT)/kernel
INCLUDEALL = $(CROSSPATH)/mips-linux-uclibc/include

export CFLAGS = -Os -g -Wall -D__MIPSEB__ -fPIC -fomit-frame-pointer -mno-memcpy -DLINUX -I$(INCLUDEALL)
export CC = $(CROSS_COMPILE)gcc $(CFLAGS)
export LD = $(CROSS_COMPILE)ld
export AR = $(CROSS_COMPILE)ar
export STRIP = $(CROSS_COMPILE)strip
export OBJCOPY = $(CROSS_COMPILE)objcopy
export INCLUDEDIR=$(INCLUDEALL)

SUBDIRS_APP  = busybox bridge-utils dropbear mount
SUBDIRS_APP += ader wireless_tools iwcontrol wl arg mii-tool ethtool tcpdump
SUBDIRS_APP += iptables iproute2 ntpclient igmpproxy dnsmasq
SUBDIRS_APP += ppp rp-pppoe l2tpd lwins mesh

all: configure
	@set -e; for i in $(SUBDIRS_APP); do $(MAKE) -C $$i; done

configure:
	cd ppp && ./DoConfigure.sh && cd ..
	cd dropbear && ./DoConfigure.sh && cd ..
	cd iproute2 && ./DoConfigure.sh && cd ..
	cd ethtool && ./DoConfigure.sh && cd ..
	cd rp-pppoe && ./DoConfigure.sh && cd ..
	cd tcpdump/libpcap ./DoConfigure.sh && cd ../..

clean:
	@set -e; for i in $(SUBDIRS_APP); do $(MAKE) -C $$i clean; rm -rfd $$i/filesystem/*; done

install:
	@set -e; for i in $(SUBDIRS_APP); do $(MAKE) -C $$i install; done
