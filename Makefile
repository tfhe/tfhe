CPP_FILES=$(wildcard *.cpp)
C_FILES=$(wildcard *.c) fft_x86_assembly/fft-x8664-avx-aux.c # fft_x86_assembly/fft-model-of-x8664-avx.c
S_FILES=fft_x86_assembly/fft-x8664-avx.s fft_x86_assembly/fft-x8664-avx-reverse.s
MAIN_CPP_FILES=$(wildcard test*.cpp)
MAIN_C_FILES=$(wildcard test*.c)
LIB_CPP_FILES=$(filter-out ${MAIN_CPP_FILES},${CPP_FILES})
LIB_C_FILES=$(filter-out ${MAIN_C_FILES},${C_FILES})
LIB_S_FILES=$(filter-out ${MAIN_C_FILES},${S_FILES})
H_FILES=$(wildcard *.h) fft_x86_assembly/fft.h
#--------------------------------------------------------------
O_FILES=$(patsubst %.cpp,%.o,${CPP_FILES}) \
	$(patsubst %.c,%.o,${C_FILES}) \
	$(patsubst %.s,%.o,${S_FILES}) 
LIB_O_FILES=$(patsubst %.cpp,%.o,${LIB_CPP_FILES}) \
	$(patsubst %.c,%.o,${LIB_C_FILES}) \
	$(patsubst %.s,%.o,${LIB_S_FILES})
MAIN_FILES=$(patsubst %.cpp,%,${MAIN_CPP_FILES}) \
	$(patsubst %.c,%,${MAIN_C_FILES})
#--------------------------------------------------------------
DEBUG_O_FILES=$(patsubst %,debug/%,${O_FILES})
DEBUG_LIB_O_FILES=$(patsubst %,debug/%,${LIB_O_FILES})
DEBUG_MAIN_FILES=${MAIN_FILES}
OPTIM_O_FILES=$(patsubst %,optim/%,${O_FILES})
OPTIM_LIB_O_FILES=$(patsubst %,optim/%,${LIB_O_FILES})
OPTIM_MAIN_FILES=$(patsubst %,%-optim,${MAIN_FILES})

FFT_PROCESSOR=NAYUKI_FFT_PROCESSOR
#FFT_PROCESSOR=FFTW_FFT_PROCESSOR
OPTIMFLAGS=-O2 -DNDEBUG -D${FFT_PROCESSOR} -funroll-loops -funroll-all-loops
DEBUGFLAGS=-O0 -D${FFT_PROCESSOR} -g3 -Wall -Werror
LIBS=-lfftw3 -lm

all: ${DEBUG_O_FILES} ${DEBUG_MAIN_FILES}

pre: optim/fft_x86_assembly debug/fft_x86_assembly
	touch pre
	
optim/fft_x86_assembly:
	mkdir -p optim/fft_x86_assembly 2>/dev/null >/dev/null || true

debug/fft_x86_assembly:
	mkdir -p debug/fft_x86_assembly 2>/dev/null >/dev/null || true

optim: ${OPTIM_O_FILES} ${OPTIM_MAIN_FILES}

clean:
	rm pre ${OPTIM_O_FILES} ${OPTIM_MAIN_FILES} ${DEBUG_MAIN_FILES} ${DEBUG_O_FILES} 2>/dev/null >/dev/null || true


debug/%.o:  %.cpp pre ${H_FILES}
	g++ -std=gnu++11 ${DEBUGFLAGS} -c -o $@ $<

debug/%.o: %.c pre ${H_FILES}
	gcc ${DEBUGFLAGS} -c -o $@ $<

debug/%.o: %.s pre ${H_FILES}
	gcc ${DEBUGFLAGS} -c -o $@ $<


%: debug/%.o ${DEBUG_LIB_O_FILES}
	g++ ${DEBUGFLAGS} -o $@ $< ${DEBUG_LIB_O_FILES} ${LIBS}

optim/%.o: %.cpp pre ${H_FILES}
	g++ -std=gnu++11 ${OPTIMFLAGS} -c -o $@ $<

optim/%.o: %.c pre ${H_FILES}
	gcc ${OPTIMFLAGS} -c -o $@ $<

optim/%.o: %.s pre ${H_FILES}
	gcc ${DEBUGFLAGS} -c -o $@ $<

%-optim: optim/%.o ${OPTIM_LIB_O_FILES}
	g++ ${OPTIMFLAGS} -o $@ $< ${OPTIM_LIB_O_FILES} ${LIBS}

