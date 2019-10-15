#include <iostream>
#include <string>

using namespace std;

enum STATES { NUMB, DIGIT, OPER };

int calc(const char *str, size_t &pos, int prev_pr);
int make_op(const char *str, size_t &pos, int prev_pr, unsigned left);
int get_pr(char op);
int get_sign(char op);

int main(int argc, char **argv) {
	size_t pos = 0;
	if (argc < 2) {
		cerr << "add expression in args, like \"3 + 3 * -3\"" << endl;
		return 1;
	} else if (argc > 2) {
		cerr << "write expression in only one arg, like \"3 + 3 * -3\"" << endl;
		return 1;
	}
	
	try {
		cout << calc(argv[1], pos, 0) << endl;
	} catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}

int calc(const char *str, size_t &pos, int prev_pr) {
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
				res = make_op(str, pos, prev_pr, res);
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

int make_op(const char *str, size_t &pos, int prev_pr, unsigned left) {
	int right;
	switch (str[pos - 1]) {
	case '+':
		return left + calc(str, pos, get_pr(str[pos - 1]));
	case '-':
		return left - calc(str, pos, get_pr(str[pos - 1]));
	case '*':
		return left * calc(str, pos, get_pr(str[pos - 1]));
	case '/':
		right = calc(str, pos, get_pr(str[pos - 1]));
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

int get_pr(char op) {
	switch (op) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		throw "Wrong operation";
	}
}

int get_sign(char op) {
	switch (op) {
	case '+':
		throw runtime_error("Unary + is't supported");
	case '-':
		return -1;
	case '*':
	case '/':
		throw runtime_error("Wait for number");
	default:
		throw runtime_error("Wrong operation");
	}
}