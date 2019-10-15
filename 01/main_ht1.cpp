#include <iostream>
#include <string>

using namespace std;

enum STATES { NUMB, DIGIT, OPER };

int calc(const char *str, size_t &pos, int prev_pr);

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
			switch (st) {
			case NUMB:
				throw runtime_error("Unary + is't supported");
				break;
			case DIGIT:
				st = OPER;
			case OPER:
				if (prev_pr >= 1) {
					return res;
				}
				++pos;
				res += calc(str, pos, 1);
				break;
			default:
				break;
			}
			break;
		case '-':
			switch (st) {
			case NUMB:
				sign *= -1;
				++pos;
				break;
			case DIGIT:
				st = OPER;
			case OPER:
				if (prev_pr >= 1) {
					return res;
				}
				++pos;
				res -= calc(str, pos, 1);
				break;
			default:
				break;
			}
			break;
		case '*':
			switch (st) {
			case NUMB:
				throw runtime_error("Wait for number");
				break;
			case DIGIT:
				st = OPER;
			case OPER:
				if (prev_pr >= 2) {
					return res;
				}
				++pos;
				res *= calc(str, pos, 2);
				break;
			default:
				break;
			}
			break;
		case '/':
			switch (st) {
			case NUMB:
				throw runtime_error("Wait for number");
				break;
			case DIGIT:
				st = OPER;
			case OPER:
				if (prev_pr >= 2) {
					return res;
				}
				++pos;
				right = calc(str, pos, 2);
				if (!right) {
					throw runtime_error("Division by 0");
				}
				if(res != ~(~0u >> 1) || right != -1) {
					res = int(res) / right;
				}
				break;
			default:
				break;
			}
			break;
		default:
			if (isdigit((unsigned char)(str[pos]))) {
				switch (st) {
				case NUMB:
					st = DIGIT;
					break;
				case OPER:
					throw runtime_error("Wait for operation");
					break;
				default:
					break;
				}
				res = res * 10 + sign * (str[pos] - '0');
			} else if (str[pos] == ' ') {
				switch (st) {
				case DIGIT:
					st = OPER;
					break;
				default:
					break;
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