#include <iostream>
#include <string>
#include "Calc.h"

using namespace std;

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
		cout << Calc()(argv[1]) << endl;
	} catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}