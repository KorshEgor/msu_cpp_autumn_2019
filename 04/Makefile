CC=g++

all: test1

test1: test.o BigInt.o
	$(CC) -o test1 test.o BigInt.o

test.o: test.cpp BigInt.h
	$(CC) -c test.cpp -O2 -std=c++14 

BigInt.o: BigInt.cpp BigInt.h
	$(CC) -c BigInt.cpp -O2 -std=c++14 

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1