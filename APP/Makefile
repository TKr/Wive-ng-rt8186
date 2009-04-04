CC=mips-linux-gcc
CFLAGS = -D__MIPSEB__ -Os -fPIC
INCLUDEDIR=../kernel/include

SUBDIRS  = busybox bridge-utils dropbear mount
SUBDIRS += ader wireless_tools iwcontrol wl arg mii-tool ethtool tcpdump rtl830x-ethctl
SUBDIRS += iptables iproute2 ntpclient igmpproxy dnsmasq
SUBDIRS += ppp pptp rp-pppoe l2tpd lwins mesh

all: 
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; rm -rfd $$i/filesystem/*; done

install:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install; done
