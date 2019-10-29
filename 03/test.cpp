#include "Matrix.h"
#include <iostream>

using namespace std;

void set_get_test();
void cmp_test();

int main() {

	try {
		set_get_test();
		cmp_test();
	} catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	cout << "OK" << endl;
	return 0;
}

void set_get_test() {
	size_t row = 10, col = 5;
	int matr[10][5];

	Matrix my_matr(row, col);

	if (my_matr.getRows() != row || my_matr.getColumns() != col) {
		throw logic_error("set_get_test: wrong cnt row or col in matr");
	}

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 5; ++j) {
			matr[i][j] = rand() % 1000;
			my_matr[i][j] = matr[i][j];
			if (my_matr[i][j] != matr[i][j]) {
				throw logic_error("set_get_test: error set or get element");
			}
		}
	}

	int mul = 3;
	my_matr *= mul;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (my_matr[i][j] != matr[i][j] * mul) {
				throw logic_error("set_get_test: error mul matr");
			}
		}
	}

	//check const Matrix can get element
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 5; ++j) {
			const Matrix &m = my_matr;
			if (m[i][j] != matr[i][j] * mul) {
				throw logic_error("set_get_test: error get element in const matr");
			}
		}
	}

	//check out of range
	bool passed = false;
	try {
		my_matr[row][0];
	} catch (const exception &e) {
		try {
			my_matr[0][col];
		} catch (const exception &e) {
			try {
				my_matr[row + 2][2];
			} catch (const exception &e) {
				try {
					my_matr[1][col + 1];
				} catch (const exception &e) {
					passed = true;
				}
			}
		}
	}
	if (!passed) {
		throw logic_error("set_get_test: no exception thrown");
	}
}

void cmp_test() {
	size_t row = 10, col = 5;
	Matrix matr1(row, col), matr2(row, col);
	Matrix w1(row - 1, col), w2(row, col + 1);

	if (!(matr1 == matr1) || matr1 != matr1) {
		throw logic_error("cmp_test: cmp m1m1");
	}
	if (!(matr1 == matr2) || matr1 != matr2) {
		throw logic_error("cmp_test: cmp m1m2");
	}
	++matr1[2][1];
	if (!(matr1 != matr2) || matr1 == matr2) {
		throw logic_error("cmp_test: cmp m1m2 after change m1");
	}
	--matr1[2][1];
	if (!(matr1 != w1) || matr1 == w1) {
		throw logic_error("cmp_test: cmp m1w1");
	}
	if (!(matr1 != w2) || matr1 == w2) {
		throw logic_error("cmp_test: cmp m1w2");
	}
}