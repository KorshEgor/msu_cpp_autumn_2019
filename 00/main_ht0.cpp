#include <chrono>
#include <iostream>
#include <limits.h>

using namespace std;

enum { SIZE = 4000 };

class Timer {
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
public:
	Timer()
		: start_(clock_t::now())
	{
	}

	~Timer()
	{
		const auto finish = clock_t::now();
		const auto us =
			std::chrono::duration_cast<microseconds>
			(finish - start_).count();
		std::cout << us << " us" << std::endl;
	}

private:
	const clock_t::time_point start_;
};

class Matr {
	int **matr = nullptr, *mem = nullptr;
public:
	Matr(int n) {
		mem = new int[n*n];
		matr = new int*[n];
		for (int i = 0; i < n; ++i) {
			matr[i] = mem + i * n;
			for (int j = 0; j < n; ++j) {
				matr[i][j] = rand() % n;
			}
		}
	}
	~Matr() {
		delete[] matr;
		delete[] mem;
	}
	int *operator[](int i) {
		return matr[i];
	}
};

int main() {
	volatile unsigned summ = 0;
	Matr matr(SIZE);
	
	{
		Timer t;
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				summ += matr[i][j];
			}
		}
	}

	summ = 0;
	{
		Timer t;
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				summ += matr[j][i];
			}
		}
	}
	return 0;
}