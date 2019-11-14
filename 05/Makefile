CC=g++

all: test1

test1: test.o Serializer.o Deserializer.o
	$(CC) -o test1 test.o Serializer.o Deserializer.o

test.o: test.cpp Error.h Serializer.h Deserializer.h
	$(CC) -c test.cpp -O2 -std=c++14 

Serializer.o: Serializer.cpp Serializer.h Error.h
	$(CC) -c Serializer.cpp -O2 -std=c++14 

Deserializer.o: Deserializer.cpp Deserializer.h Error.h
	$(CC) -c Deserializer.cpp -O2 -std=c++14 

clean:
	rm -rf *.o test1.exe test1

test: test1
	./test1