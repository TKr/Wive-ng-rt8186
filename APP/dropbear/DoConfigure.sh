ROOTDIR=/opt/Wive/APP/dropbear
CONFOPTS="--host=mips --disable-zlib --disable-openpty"
CONFOPTS="$CONFOPTS --disable-shadow --disable-pam"
CONFOPTS="$CONFOPTS --disable-lastlog --disable-utmp --disable-utmpx"
CONFOPTS="$CONFOPTS --disable-wtmp --disable-wtmpx --disable-loginfunc"
CONFOPTS="$CONFOPTS --disable-pututline --disable-pututxline"
CONFOPTS="$CONFOPTS --disable-largefile --prefix=$ROOTDIR/filesystem"
CC=mips-linux-gcc STRIP=mips-linux-strip
CFLAGS="-s -Os -fomit-frame-pointer -mno-memcpy" LDFLAGS="-s -Os"
export CC STRIP CFLAGS LDFLAGS
./configure $CONFOPTS
