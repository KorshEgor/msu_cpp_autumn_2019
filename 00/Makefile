CC=g++

all: test_matrix

test_matrix: main_ht0.o 
	$(CC) -o test_matrix main_ht0.o

main_ht0.o: main_ht0.cpp 
	$(CC) -c main_ht0.cpp -O2 -std=c++14

clean:
	rm -rf *.o test_matrix.exe
run:
	./test_matrix