CC=mips-linux-gcc
CFLAGS = -D__MIPSEB__ -Os -fPIC
INCLUDEDIR=../kernel/include

SUBDIRS  = busybox bridge-utils dropbear mount
SUBDIRS += ader wireless_tools iwcontrol wl arg mii-tool ethtool tcpdump
SUBDIRS += iptables iproute2 ntpclient igmpproxy dnsmasq
SUBDIRS += ppp rp-pppoe l2tpd lwins mesh

all: configure
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

configure:
	cd ppp && ./DoConfigure.sh && cd ..
	cd dropbear && ./DoConfigure.sh && cd ..
	cd iproute2 && ./DoConfigure.sh && cd ..
	cd ethtool && ./DoConfigure.sh && cd ..
	cd rp-pppoe && ./DoConfigure.sh && cd ..
	cd tcpdump/libpcap ./DoConfigure.sh && cd ../..

clean:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; rm -rfd $$i/filesystem/*; done

install:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install; done
