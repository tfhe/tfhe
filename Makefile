CPP_FILES=$(wildcard *.cpp)
C_FILES=$(wildcard *.c)
MAIN_CPP_FILES=$(wildcard test*.cpp)
MAIN_C_FILES=$(wildcard test*.c)
LIB_CPP_FILES=$(filter-out ${MAIN_CPP_FILES},${CPP_FILES})
LIB_C_FILES=$(filter-out ${MAIN_C_FILES},${C_FILES})
H_FILES=$(wildcard *.h)
#--------------------------------------------------------------
O_FILES=$(patsubst %.cpp,%.o,${CPP_FILES}) \
	$(patsubst %.c,%.o,${C_FILES})
LIB_O_FILES=$(patsubst %.cpp,%.o,${LIB_CPP_FILES}) \
	$(patsubst %.c,%.o,${LIB_C_FILES})
MAIN_FILES=$(patsubst %.cpp,%,${MAIN_CPP_FILES}) \
	$(patsubst %.c,%,${MAIN_C_FILES})
#--------------------------------------------------------------
DEBUG_O_FILES=$(patsubst %,debug/%,${O_FILES})
DEBUG_LIB_O_FILES=$(patsubst %,debug/%,${LIB_O_FILES})
DEBUG_MAIN_FILES=${MAIN_FILES}
OPTIM_O_FILES=$(patsubst %,optim/%,${O_FILES})
OPTIM_LIB_O_FILES=$(patsubst %,optim/%,${LIB_O_FILES})
OPTIM_MAIN_FILES=$(patsubst %,%-optim,${MAIN_FILES})

OPTIMFLAGS=-O2 -DNDEBUG -funroll-loops -funroll-all-loops
DEBUGFLAGS=-O0 -g3 -Wall -Werror
LIBS=-lfftw3 -lm

all: ${DEBUG_O_FILES} ${DEBUG_MAIN_FILES}
	
optim: ${OPTIM_O_FILES} ${OPTIM_MAIN_FILES}

clean:
	rm ${OPTIM_O_FILES} ${OPTIM_MAIN_FILES} ${DEBUG_MAIN_FILES} ${DEBUG_O_FILES} 2>/dev/null >/dev/null || true


debug/%.o: %.cpp ${H_FILES}
	g++ -std=gnu++11 ${DEBUGFLAGS} -c -o $@ $<

debug/%.o: %.c ${H_FILES}
	gcc ${DEBUGFLAGS} -c -o $@ $<

%: debug/%.o ${DEBUG_LIB_O_FILES}
	g++ ${DEBUGFLAGS} -o $@ $< ${DEBUG_LIB_O_FILES} ${LIBS}

optim/%.o: %.cpp ${H_FILES}
	g++ -std=gnu++11 ${OPTIMFLAGS} -c -o $@ $<

optim/%.o: %.c ${H_FILES}
	gcc ${OPTIMFLAGS} -c -o $@ $<

%-optim: optim/%.o ${OPTIM_LIB_O_FILES}
	g++ ${OPTIMFLAGS} -o $@ $< ${OPTIM_LIB_O_FILES} ${LIBS}

