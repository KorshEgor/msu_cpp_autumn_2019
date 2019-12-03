CC=g++

all: test1

test1: test.o ThreadPool.o
	$(CC) -o test1 test.o ThreadPool.o

test.o: test.cpp ThreadPool.h
	$(CC) -c test.cpp -O2 -std=c++14 -pthread

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(CC) -c ThreadPool.cpp -O2 -std=c++14 -pthread

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1