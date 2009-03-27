################################################
#Make files fo compile wive-ng router firmware.#
################################################
all:
	make linux
	make modules
	make root
	make split

clean:
	make -C APP clean
	rm -rf ./images/*
	rm -rf ./ro_root/*
	rm -rf ./rw_root/*
	make -C kernel clean
	rm -rf ./kernel/modules_installmake/*
	make -C tools clean

linux:
	make tools
	make -C kernel oldconfig
	make -C kernel dep
	make -C kernel all
	make -C kernel/rtkload bz2


modules:
	make -C kernel modules
	make -C kernel modules_install
	cd kernel; ./strip_modules; cd ..

root:
	make -C APP
	make -C APP install
	@./mkimg


split:
	cd ./tools; ./splitinone.pl
	mv ./tools/wive*.bin ./images/

tools:
	make -C tools
