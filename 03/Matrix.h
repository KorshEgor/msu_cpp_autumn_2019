#pragma once
#include <cstddef>

class Matrix {
	int *mem = nullptr;
	std::size_t row, col;
public:
	class Row {
		int *row;
		std::size_t col;
	public:
		Row(int *row, std::size_t col);
		int &operator[](std::size_t i);
		const int &operator[](std::size_t i) const;
	};

	Matrix(std::size_t row, std::size_t col);
	std::size_t getRows() const;
	std::size_t getColumns() const;
	~Matrix();
	Row operator[](std::size_t i);
	const Row operator[](std::size_t i) const;
	const Matrix &operator*=(int a);
	bool operator==(const Matrix &m) const;
	bool operator!=(const Matrix &m) const;
};