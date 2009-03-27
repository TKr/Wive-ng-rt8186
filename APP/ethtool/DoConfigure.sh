ROOTDIR=/opt/Wive/APP/ethtool
CONFOPTS="--host=mips --prefix=$ROOTDIR/filesystem"
CC=mips-linux-gcc STRIP=mips-linux-strip
CFLAGS="-s -Os -fomit-frame-pointer -mno-memcpy" LDFLAGS="-s -Os"
export CC STRIP CFLAGS LDFLAGS
./configure $CONFOPTS
