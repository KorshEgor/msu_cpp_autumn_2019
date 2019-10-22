CC=g++

all: test1

test1: test.o LinearAllocator.o
	$(CC) -o test1 test.o LinearAllocator.o

test.o: test.cpp LinearAllocator.h
	$(CC) -c test.cpp -O2 -std=c++14 

LinearAllocator.o: LinearAllocator.cpp LinearAllocator.h
	$(CC) -c LinearAllocator.cpp -O2 -std=c++14 

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1