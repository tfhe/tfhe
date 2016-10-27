all: build
	make -C build

clean: build
	make -C build clean

distclean:
	rm -rf build; true

build:
	mkdir build; cd build; cmake ../src; cd ..
