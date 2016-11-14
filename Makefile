all: build
	make -C build

clean: build
	make -C build clean

distclean:
	rm -rf build; true

test: all
	make -C build test

build:
	mkdir build; cd build; cmake ../src; cd ..
