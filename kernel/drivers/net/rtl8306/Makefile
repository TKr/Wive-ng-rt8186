#
# drivers/net/wireless/Makefile
#
# Makefile for the Linux Wireless network device drivers.
#

O_TARGET := re8306.o

obj-y		:=
obj-m		:=
obj-n		:=
obj-		:=

# Things that need to export symbols
export-objs	:=

obj-$(CONFIG_RE8306) += Rtl8306_Driver_s.o Rtl8306_Driver_sd.o Rtl8306_AsicDrv.o mdcmdio.o

include $(TOPDIR)/Rules.make
