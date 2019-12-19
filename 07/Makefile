CC=g++

all: test1

test1: test.o
	$(CC) -o test1 test.o

test.o: test.cpp Vector.h
	$(CC) -c test.cpp -O2 -std=c++14

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1