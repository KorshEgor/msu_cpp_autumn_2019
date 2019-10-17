CC=g++
exp=

all: calc

calc: main_ht1.o Calc.o
	$(CC) -o calc main_ht1.o Calc.o

main_ht1.o: main_ht1.cpp Calc.h
	$(CC) -c main_ht1.cpp -O2 -std=c++14 

Calc.o: Calc.cpp Calc.h
	$(CC) -c Calc.cpp -O2 -std=c++14 

clean:
	rm -rf *.o calc.exe calc

run: calc
	./calc $(exp)

test: calc
	python test.py