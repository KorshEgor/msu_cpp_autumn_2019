#include "Calc.h"
#include <stdexcept>
#include <cctype>

enum STATES { NUMB, DIGIT, OPER };

using namespace std;

int Calc::operator()(const char *str) {
	pos = 0;
	return calc(str, 0);
}

int Calc::calc(const char *str, int prev_pr) {
	unsigned res = 0;
	int right;
	int sign = 1;
	STATES st = NUMB;
	while (str[pos]) {
		switch (str[pos]) {
		case '+':
		case '-':
		case '*':
		case '/':
			if (st == NUMB) {
				sign *= get_sign(str[pos]);
				++pos;
			} else {
				st = OPER;
				if (prev_pr >= get_pr(str[pos])) {
					return res;
				}
				++pos;
				res = make_op(str, prev_pr, res);
			}
			break;
		default:
			if (isdigit((unsigned char)(str[pos]))) {
				if (st == OPER) {
					throw runtime_error("Wait for operation");
				}
				st = DIGIT;
				res = res * 10 + sign * (str[pos] - '0');
			} else if (str[pos] == ' ') {
				if (st == DIGIT) {
					st = OPER;
				}
			} else {
				throw runtime_error("Wrong latter");
			}
			++pos;
			break;
		}
	}
	if (st != NUMB) {
		return res;
	} else {
		throw runtime_error("Wait for number");
	}
}

int Calc::make_op(const char *str, int prev_pr, unsigned left) {
	int right;
	switch (str[pos - 1]) {
	case '+':
		return left + calc(str, get_pr(str[pos - 1]));
	case '-':
		return left - calc(str, get_pr(str[pos - 1]));
	case '*':
		return left * calc(str, get_pr(str[pos - 1]));
	case '/':
		right = calc(str, get_pr(str[pos - 1]));
		if (!right) {
			throw runtime_error("Division by 0");
		}
		if (left != ~(~0u >> 1) || right != -1) {
			return int(left) / right;
		}
		return left;
	default:
		throw runtime_error("Wrong operation");
	}
}

int Calc::get_pr(char op) {
	switch (op) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		throw logic_error("Class calc: Wrong operation");
	}
}

int Calc::get_sign(char op) {
	switch (op) {
	case '+':
		throw runtime_error("Unary + is't supported");
	case '-':
		return -1;
	case '*':
	case '/':
		throw runtime_error("Wait for number");
	default:
		throw logic_error("Class calc: Wrong operation");
	}
}