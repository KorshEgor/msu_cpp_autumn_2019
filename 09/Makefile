CC=g++

all: test1

test1: test.o ThreadPool.o file_sort.o
	$(CC) -o test1 test.o ThreadPool.o file_sort.o

test.o: test.cpp ThreadPool.h file_sort.h
	$(CC) -c test.cpp -O2 -std=c++14 -pthread

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(CC) -c ThreadPool.cpp -O2 -std=c++14 -pthread

file_sort.o: file_sort.cpp file_sort.h ThreadPool.h
	$(CC) -c file_sort.cpp -O2 -std=c++14 -pthread

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1