#pragma once
#include <ostream>
#include <cstddef>

class BigInt {
	int *mem;
	std::size_t size, memsize;
	int sign;
	void correct();
	void correct_size();
	void summ(const BigInt &val, int sgn);
	int abs_cmp(const BigInt &val) const;
	void resize(std::size_t nsize);
public:
	BigInt();
	BigInt(int val);
	BigInt(const BigInt &val);
	BigInt(BigInt &&val);
	~BigInt();
	BigInt &operator=(const BigInt &val);
	BigInt &operator=(BigInt &&val);
	BigInt &operator=(int val);
	BigInt operator+(const BigInt &val) const;
	BigInt operator-(const BigInt &val) const;
	BigInt operator-() const;
	BigInt &operator+=(const BigInt &val);
	BigInt &operator-=(const BigInt &val);
	bool operator==(const BigInt &val) const;
	bool operator!=(const BigInt &val) const;
	bool operator<=(const BigInt &val) const;
	bool operator>=(const BigInt &val) const;
	bool operator<(const BigInt &val) const;
	bool operator>(const BigInt &val) const;
	friend std::ostream& operator<<(std::ostream &out, const BigInt &value);
	friend BigInt operator-(int b, const BigInt &a);
};

std::ostream& operator<<(std::ostream &out, const BigInt &val);
BigInt operator+(int b, const BigInt &a);
BigInt operator-(int b, const BigInt &a);
bool operator==(int b, const BigInt &a);
bool operator!=(int b, const BigInt &a);
bool operator<=(int b, const BigInt &a);
bool operator>=(int b, const BigInt &a);
bool operator<(int b, const BigInt &a);
bool operator>(int b, const BigInt &a);