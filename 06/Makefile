CC=g++

all: test1

test1: test.o format.o
	$(CC) -o test1 test.o format.o

test.o: test.cpp format.h
	$(CC) -c test.cpp -O2 -std=c++14

format.o: format.cpp format.h
	$(CC) -c format.cpp -O2 -std=c++14

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1