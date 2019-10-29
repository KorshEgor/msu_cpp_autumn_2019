#include "Matrix.h"
#include <stdexcept>
#include <cstddef>

using namespace std;

Matrix::Matrix(std::size_t row, std::size_t col) : row(row), col(col), mem(new int[row * col]()) {}

Matrix::~Matrix() {
	delete[] mem;
}

size_t Matrix::getRows() const {
	return row;
}

size_t Matrix::getColumns() const {
	return col;
}

Matrix::Row Matrix::operator[](size_t i) {
	if (i >= row) {
		throw out_of_range("Wrong row index");
	}
	return Row(mem + i * col, col);
}

const Matrix::Row Matrix::operator[](size_t i) const {
	if (i >= row) {
		throw out_of_range("Wrong row index");
	}
	return Row(mem + i * col, col);
}

Matrix::Row::Row(int *row, size_t col) : row(row), col(col) {}

int &Matrix::Row::operator[](size_t i) {
	if (i >= col) {
		throw out_of_range("Wrong column index");
	}
	return row[i];
}

const int &Matrix::Row::operator[](size_t i) const {
	if (i >= col) {
		throw out_of_range("Wrong column index");
	}
	return row[i];
}

const Matrix &Matrix::operator*=(int a) {
	for (size_t i = 0; i < row*col; ++i) {
		mem[i] *= a;
	}
	return *this;
}
bool Matrix::operator==(const Matrix &m) const {
	if (this == &m) {
		return true;
	}
	if (m.col != col || m.row != row) {
		return false;
	}
	for (size_t i = 0; i < row*col; ++i) {
		if (mem[i] != m.mem[i]) {
			return false;
		}
	}
	return true;
}
bool Matrix::operator!=(const Matrix &m) const {
	return !(*this == m);
}