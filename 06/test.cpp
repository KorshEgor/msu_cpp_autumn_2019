#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "format.h"

using namespace std;

template<class... ArgsT> void test_except(const string &msg, const ArgsT&... args);
void test(const string &form, const string &s);

int main() {
	try {
		test(format("{1}+{1} = {0} smth in end", 2, "one"), "one+one = 2 smth in end");
		test(format("+ = "), "+ = ");
		int i = 42;
		double d = 4.2;
		char c = 42;
		bool b = true;
		test(format(" {3} {2} {1} {0}", i, d, c, b), " 1 * 4.2 42");
		test(format("", i), "");
		test(format("{0}, {10}", i, i, i, i, i, i, i, i, i, i, d), "42, 4.2");
		test(format("{0}, {00000}", i), "42, 42");
	} catch (const exception &e) {
		cout << "Error test1, throw exception: " << e.what() << endl;
		return 0;
	}

	test_except("violation of context {n}", "{}");
	test_except("violation of context {n}", "}");
	test_except("violation of context {n}", "{-0}", 42);
	test_except("violation of context {n}", "{0 }", 42);
	test_except("violation of context {n}", "{0}, {00000", 42);
	test_except("wrong num of args", "{0}");
	test_except("wrong num of args", "{0} {3} {1} ", 42, 41, 40);

	cout << "OK" << endl;
	return 0;
}

template<class... ArgsT>
void test_except(const string &msg, const ArgsT&... args) {
	try {
		format(args...);
		cout << "Error test_except, \"" << msg << "\"" << endl;
		cout << "No throw" << endl;
		exit(0);
	} catch (const exception &e) {
		if (e.what() != msg) {
			cout << "Error test_except, \"" << msg << "\"" << endl;
			cout << "what: \"" << e.what() << "\"" << endl;
			exit(0);
		}
	}
}

void test(const string &form, const string &s) {
	if (form != s) {
		cout << "Error test, \"" << s << "\"" << endl;
		cout << "format: \"" << form << "\"" << endl;
		exit(0);
	}
}