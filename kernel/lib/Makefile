#
# Makefile for some libs needed in the kernel.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

L_TARGET := lib.a

export-objs := cmdline.o dec_and_lock.o rwsem-spinlock.o rwsem.o

obj-y := errno.o ctype.o string.o vsprintf.o brlock.o cmdline.o bust_spinlocks.o rbtree.o

obj-$(CONFIG_RWSEM_GENERIC_SPINLOCK) += rwsem-spinlock.o

subdir-$(CONFIG_ZLIB_INFLATE) += zlib_inflate
subdir-$(CONFIG_ZLIB_DEFLATE) += zlib_deflate

# Include the subdirs, if necessary.
obj-y += $(join $(subdir-y),$(subdir-y:%=/%.o))

ifneq ($(CONFIG_HAVE_DEC_LOCK),y) 
  obj-y += dec_and_lock.o
endif

include $(TOPDIR)/Rules.make
