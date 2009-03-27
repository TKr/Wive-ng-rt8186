#
# Makefile for the linux squashfs routines.
#

O_TARGET := squashfs.o

obj-y  := inode.o LzmaDecode.o

obj-m := $(O_TARGET)

CFLAGS_inode.o := -I $(TOPDIR)/fs/inflate_fs
include $(TOPDIR)/Rules.make
