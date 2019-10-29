CC=g++

all: test1

test1: test.o Matrix.o
	$(CC) -o test1 test.o Matrix.o

test.o: test.cpp Matrix.h
	$(CC) -c test.cpp -O2 -std=c++14 

Matrix.o: Matrix.cpp Matrix.h
	$(CC) -c Matrix.cpp -O2 -std=c++14 

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1