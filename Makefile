CPP_FILES=$(wildcard *.cpp)
O_FILES=$(patsubst %.cpp,%.o,${CPP_FILES})

all: ${O_FILES}
	
clean:
	rm ${O_FILES}


%.o: %.cpp
	g++ -Wall -Werror -O0 -g3 -c -o $@ $<
