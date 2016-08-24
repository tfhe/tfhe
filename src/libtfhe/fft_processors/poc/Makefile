newall:
	g++ -c -O0 -Wall -g3 poc.cpp -o poc.o
	gcc -c -O0 -Wall -g3 poc.s -o poc-asm.o
	g++ -O0 -Wall -g3 poc.o poc-asm.o -o test

newall-optim:
	g++ -c -O2 -Wall -DNDEBUG poc.cpp -o poc.o
	gcc -c -O2 -Wall -DNDEBUG poc.s -o poc-asm.o
	g++ -O2 -Wall -DNDEBUG poc.o poc-asm.o -o test-optim
all:
	g++ -DNDEBUG -O2 -mavx poc.cpp -o poc
	g++ -O0 -Wall -mavx poc.cpp -o poc-debug
