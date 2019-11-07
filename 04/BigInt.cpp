#include "BigInt.h"
#include <cstring>
#include <iomanip>

using namespace std;

enum { BASE = static_cast<int>(1e8), BASESIZE = 8 };

BigInt::BigInt() : mem(nullptr), size(0), memsize(0), sign(1) {}

BigInt::BigInt(int val) : mem(nullptr), memsize(0), sign(1 - 2 * (val < 0)) {
	size = (val != 0) + (val >= BASE || val <= -BASE);
	resize(size);
	int mass[] = { val % BASE, val / BASE };
	if (val < 0) {
		mass[0] *= -1;
		mass[1] *= -1;
	}
	memcpy(mem, mass, size * sizeof(int));
}

BigInt::BigInt(const BigInt &val) : mem(nullptr), size(val.size), memsize(0), sign(val.sign){
	resize(val.size);
	memcpy(mem, val.mem, val.size * sizeof(int));
}

BigInt::BigInt(BigInt &&val) : mem(val.mem), size(val.size), memsize(val.memsize), sign(val.sign) {
	val.memsize = val.size = 0;
	val.sign = 1;
	val.mem = nullptr;
}

BigInt::~BigInt() {
	delete[] mem;
}

BigInt &BigInt::operator=(const BigInt &val) {
	if (this != &val) {
		resize(val.size);
		memcpy(mem, val.mem, val.size * sizeof(int));
		sign = val.sign;
		size = val.size;
	}
	return *this;
}

BigInt &BigInt::operator=(BigInt &&val) {
	if (this != &val) {
		delete[] mem;
		mem = val.mem;
		sign = val.sign;
		size = val.size;
		memsize = val.memsize;
		val.memsize = val.size = 0;
		val.sign = 1;
		val.mem = nullptr;
	}
	return *this;
}

BigInt &BigInt::operator=(int val) {
	mem = nullptr;
	memsize = 0;
	sign = 1 - 2 * (val < 0);
	size = (val != 0) + (val >= BASE || val <= -BASE);
	resize(size);
	int mass[] = { val % BASE, val / BASE };
	if (val < 0) {
		mass[0] *= -1;
		mass[1] *= -1;
	}
	memcpy(mem, mass, size * sizeof(int));
	return *this;
}

void BigInt::correct() {
	int shift = 0;
	for (size_t i = 0; i < size; ++i) {
		mem[i] += shift;
		shift = (mem[i] + BASE) / BASE - 1;
		mem[i] -= shift * BASE;
	}
	if (shift) {
		resize(size + 1);
		mem[size++] = shift;
	}
	correct_size();
}

void BigInt::correct_size() {
	for (size_t i = size; i > 0; --i) {
		if (mem[i - 1]) {
			break;
		}
		--size;
	}
	if (!size) {
		sign = 1;
	}
}

void BigInt::summ(const BigInt &val, int sgn) {
	int s = sign * val.sign * sgn;
	if (abs_cmp(val) >= 0) {
		for (size_t i = 0; i < val.size; ++i) {
			mem[i] += s * val.mem[i];
		}
	} else {
		resize(val.size + (s == 1));
		sign = val.sign * sgn;
		for (size_t i = 0; i < size; ++i) {
			mem[i] = val.mem[i] + s * mem[i];
		}
		for (size_t i = size; i < val.size; ++i) {
			mem[i] = val.mem[i];
		}
		size = val.size;
	}
	correct();
}

int BigInt::abs_cmp(const BigInt &val) const {
	if (this == &val) {
		return 0;
	}
	if (size != val.size) {
		return static_cast<int>(size) - static_cast<int>(val.size);
	} else {
		for (size_t i = size; i > 0; --i) {
			if (mem[i - 1] != val.mem[i - 1]) {
				return mem[i - 1] - val.mem[i - 1];
			}
		}
	}
	return 0;
}

void BigInt::resize(std::size_t nsize) {
	if (memsize < nsize) {
		int *tmp = new int[nsize]();
		if (!tmp) {
			throw runtime_error("Error malloc memory for BigInt");
		}
		if (mem) {
			memcpy(tmp, mem, size * sizeof(int));
			delete[] mem;
		}
		mem = tmp;
		memsize = nsize;
	}
}

BigInt BigInt::operator+(const BigInt &val) const {
	BigInt tmp = *this;
	tmp.summ(val, 1);
	return tmp;
}

BigInt BigInt::operator-(const BigInt &val) const {
	BigInt tmp = *this;
	tmp.summ(val, -1);
	return tmp;
}

BigInt BigInt::operator-() const {
	BigInt tmp = *this;
	tmp.sign *= -1;
	if (!tmp.size) {
		tmp.sign = 1;
	}
	return tmp;
}

BigInt &BigInt::operator+=(const BigInt &val) {
	summ(val, 1);
	return *this;
}

BigInt &BigInt::operator-=(const BigInt &val) {
	summ(val, -1);
	return *this;
}

bool BigInt::operator==(const BigInt &val) const {
	if (this == &val) {
		return true;
	}
	return sign == val.sign && !abs_cmp(val);
}

bool BigInt::operator!=(const BigInt &val) const {
	return !(*this == val);
}

bool BigInt::operator<=(const BigInt &val) const {
	if (this == &val) {
		return true;
	}
	if (sign < val.sign) {
		return true;
	}
	if (sign > val.sign) {
		return false;
	}
	return abs_cmp(val) * sign <= 0;
}

bool BigInt::operator>=(const BigInt &val) const {
	if (this == &val) {
		return true;
	}
	if (sign < val.sign) {
		return false;
	}
	if (sign > val.sign) {
		return true;
	}
	return abs_cmp(val) * sign >= 0;
}

bool BigInt::operator<(const BigInt &val) const {
	return !(*this >= val);
}

bool BigInt::operator>(const BigInt &val) const {
	return !(*this <= val);
}

ostream& operator<<(ostream &out, const BigInt &val) {
	if (val.size) {
		if (val.sign == -1) {
			out << '-';
		}
		out << val.mem[val.size - 1];
		for (size_t i = val.size; i > 1; --i) {
			out << setfill('0') << setw(BASESIZE) << val.mem[i - 2]; //???
		}
	} else {
		out << '0';
	}
	return out;
}

BigInt operator+(int b, const BigInt &a) {
	return a + b;
}

BigInt operator-(int b, const BigInt &a) {
	BigInt t(a - b);
	if (t.size) {
		t.sign *= -1;
	}
	return t;
}

bool operator==(int b, const BigInt &a) {
	return a == b;
}

bool operator!=(int b, const BigInt &a) {
	return a != b;
}

bool operator<=(int b, const BigInt &a) {
	return a >= b;
}

bool operator>=(int b, const BigInt &a) {
	return a <= b;
}

bool operator<(int b, const BigInt &a) {
	return a > b;
}

bool operator>(int b, const BigInt &a) {
	return a < b;
}