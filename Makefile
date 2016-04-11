CPP_FILES=$(wildcard *.cpp)
C_FILES=$(wildcard *.c)
MAIN_CPP_FILES=$(wildcard test*.cpp)
MAIN_C_FILES=$(wildcard test*.c)
LIB_CPP_FILES=$(filter-out ${MAIN_CPP_FILES},${CPP_FILES})
LIB_C_FILES=$(filter-out ${MAIN_C_FILES},${C_FILES})
H_FILES=$(wildcard *.h)
O_FILES=$(patsubst %.cpp,%.o,${CPP_FILES}) \
	$(patsubst %.c,%.o,${C_FILES})
LIB_O_FILES=$(patsubst %.cpp,%.o,${LIB_CPP_FILES}) \
	$(patsubst %.c,%.o,${LIB_C_FILES})
MAIN_FILES=$(patsubst %.cpp,%,${MAIN_CPP_FILES}) \
	$(patsubst %.c,%,${MAIN_C_FILES})

all: ${O_FILES} ${MAIN_FILES}
	
clean:
	rm ${O_FILES} ${MAIN_FILES} 2>/dev/null >/dev/null || true


%.o: %.cpp ${H_FILES}
	g++ -std=c++11 -Wall -Werror -O0 -g3 -c -o $@ $<

%.o: %.c ${H_FILES}
	gcc -Wall -Werror -O0 -g3 -c -o $@ $<

%: %.o ${LIB_O_FILES}
	g++ -Wall -Werror -O0 -g3 -o $@ $< ${LIB_O_FILES}

