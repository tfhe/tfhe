CMAKE_COMPILER_OPTS=
CMAKE_TESTS_OPTS=-DENABLE_TESTS=on -DENABLE_FFTW=on \
		 -DENABLE_NAYUKI_PORTABLE=on -DENABLE_NAYUKI_AVX=on \
		 -DENABLE_SPQLIOS_AVX=on -DENABLE_SPQLIOS_FMA=on
CMAKE_DTESTS_OPTS=${CMAKE_COMPILER_OPTS} -DCMAKE_BUILD_TYPE=debug ${CMAKE_TESTS_OPTS}
CMAKE_OTESTS_OPTS=${CMAKE_COMPILER_OPTS} -DCMAKE_BUILD_TYPE=optim ${CMAKE_TESTS_OPTS}

all: build
	make -C build

install: build
	make -C build install

clean: build
	make -C build clean

distclean:
	rm -rf build builddtests buildotests; true

test: builddtests buildotests src/test/googletest/CMakeLists.txt
	make -j $(nproc) -C builddtests VERBOSE=1
	make -j $(nproc) -C buildotests VERBOSE=1
	make -j $(nproc) -C builddtests test VERBOSE=1
	make -j $(nproc) -C buildotests test VERBOSE=1

build: src/test/googletest/CMakeLists.txt
	mkdir build; cd build; cmake ../src; cd ..

builddtests:
	rm -rf $@; true; mkdir $@; 
	cd $@; cmake ../src ${CMAKE_DTESTS_OPTS}; 
	cd $@; cmake ../src ${CMAKE_DTESTS_OPTS};
	cd ..

buildotests:
	rm -rf $@; true; mkdir $@; 
	cd $@; cmake ../src ${CMAKE_OTESTS_OPTS}; 
	cd $@; cmake ../src ${CMAKE_OTESTS_OPTS};
	cd ..

src/test/googletest/CMakeLists.txt:
	git submodule init
	git submodule update

alltests:
	make distclean && make test CMAKE_COMPILER_OPTS="-DCMAKE_CXX_COMPILER=clang++-libc++ -DCMAKE_C_COMPILER=clang"
	make distclean && make test CMAKE_COMPILER_OPTS="-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
	make distclean && make test CMAKE_COMPILER_OPTS="-DCMAKE_CXX_COMPILER=g++-7 -DCMAKE_C_COMPILER=gcc-7"
	make distclean && make test CMAKE_COMPILER_OPTS="-DCMAKE_CXX_COMPILER=g++-6 -DCMAKE_C_COMPILER=gcc-6"
	make distclean && make test CMAKE_COMPILER_OPTS="-DCMAKE_CXX_COMPILER=g++-5 -DCMAKE_C_COMPILER=gcc-5"

