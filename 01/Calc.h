#pragma once
#include <cstddef>

class Calc {
	std::size_t pos;
	int calc(const char *str, int prev_pr);
	int make_op(const char *str, int prev_pr, unsigned left);
	int get_pr(char op);
	int get_sign(char op);
public:
	int operator()(const char *str);
};