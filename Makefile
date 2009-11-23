################################################
#Make files fo compile wive-ng router firmware.#
################################################
all:
	make linux
	make modules
	make root
	make split

clean:
	PWD=`pwd`
	echo "Clean temporary files..."
	make -C APP clean
	rm -rf ./images/*
	rm -rf ./ro_root/*
	rm -rf ./rw_root/*
	echo > kernel/.config
	make -C kernel clean
	make -C kernel mrproper
	rm -rf ./kernel/modules_installmake/*
	make -C tools clean
	find ${PWD}/APP -name '*.o' | xargs rm -f
	find ${PWD} -name '*.log' | xargs rm -f
	find ${PWD} -name '*.old' | xargs rm -f

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

dirs:
	mkdir -p `cat dirs_struct`
