#!/bin/bash

ROOTDIR=`pwd`
CONFOPTS="--host=mips --prefix=$ROOTDIR/filesystem \
--disable-debugging --enable-plugin=plugin \
--includedir=$ROOTDIR/include"
CC=mips-linux-gcc STRIP=mips-linux-strip
CFLAGS="-s -Os" LDFLAGS="-s -Os"
export CC STRIP CFLAGS LDFLAGS
./configure $CONFOPTS
