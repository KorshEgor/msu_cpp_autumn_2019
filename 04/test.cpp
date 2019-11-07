#include "BigInt.h"
#include <iostream>
#include <sstream>

using namespace std;

enum { TESTCNT = 5 };

int main() {
	stringstream s[TESTCNT][2];

	s[0][0] << BigInt(0) << ' ' << -BigInt(0) << ' ' << BigInt(INT_MAX) << ' ' << BigInt(INT_MIN) << ' ';
	s[0][0] << BigInt(static_cast<int>(2e9)) << ' ' << BigInt(static_cast<int>(-2e9));
	s[0][1] << 0 << ' ' << 0 << ' ' << INT_MAX << ' ' << INT_MIN << ' ';
	s[0][1] << static_cast<int>(2e9) << ' ' << static_cast<int>(-2e9);
	if (s[0][0].str() != s[0][1].str()) {
		cout << "Error in test1 (out and construct from int)" << endl;
		cout << "s0: " << s[0][0].str() << endl;
		cout << "s1: " << s[0][1].str() << endl;
		return 0;
	}
	
	BigInt a = INT_MAX;
	BigInt b = INT_MIN;
	long long a1 = INT_MAX, b1 = INT_MIN, st1 = INT_MAX, st2 = INT_MIN, st = b1;
	BigInt step1 = a, step2 = b , step = b;
	for (int i = 0; i < 10; ++i) {
		step1 = step1 + step1;
		step2 += step2;
		st1 += st1;
		st2 += st2;
		step -= b + 10;
		st -= b1 + 10;
	}

	s[1][0] << step1 << ' ' << step2 << ' ' << a - b << ' ' << b - a << ' ' << b - b << ' ' << a + b << ' ' << 0 - step2 << ' ' << step << ' ';
	s[1][1] << st1 << ' ' << st2 << ' ' << a1 - b1 << ' ' << b1 - a1 << ' ' << 0  << ' ' << a1 + b1 << ' ' << -st2 << ' ' << st << ' ';
	BigInt z = 0;
	s[1][0] << z - a << ' ' << z + a << ' ' << -z - a << ' ' << -z + a << ' ';
	s[1][1] << -a1 << ' ' << a1 << ' ' << -a1 << ' ' << a1 << ' ';
	s[1][0] << z - b << ' ' << z + b << ' ' << -z - b << ' ' << -z + b << ' ';
	s[1][1] << -b1 << ' ' << b1 << ' ' << -b1 << ' ' << b1 << ' ';
	
	if (s[1][0].str() != s[1][1].str()) {
		cout << "Error in test2 (out, construct from BigInt [copy], summ and diff)" << endl;
		cout << "s0: " << s[1][0].str() << endl;
		cout << "s1: " << s[1][1].str() << endl;
		return 0;
	}

	BigInt c1(INT_MAX), c2(a), c3(INT_MIN), c4(b);

	s[2][0] << (a == a) << ' ' << (a == c1) << ' ' << (a == c2) << ' ' << (c1 == c1) << ' ' << (c1 == c2) << ' ' << (c2 == c2);
	s[2][1] << "1 1 1 1 1 1";
	s[2][0] << (a != b) << ' ' << (a != c3) << ' ' << (a != c4) << ' ' << (c3 != c1) << ' ' << (c3 != c2) << ' ' << (c4 != a);
	s[2][1] << "1 1 1 1 1 1";
	s[2][0] << (a != a) << ' ' << (a != c1) << ' ' << (a != c2) << ' ' << (c1 != c1) << ' ' << (c1 != c2) << ' ' << (c2 != c2);
	s[2][1] << "0 0 0 0 0 0";
	s[2][0] << (a == b) << ' ' << (a == c3) << ' ' << (a == c4) << ' ' << (c3 == c1) << ' ' << (c3 == c2) << ' ' << (c4 == a);
	s[2][1] << "0 0 0 0 0 0";
	s[2][0] << (a - a == 0) << ' ' << (0 == a - a) << ' ' << (a - 10 - a == -10) << ' ' << (-10 == a - 10 - a) << ' ';
	s[2][0] << (a != -a) << ' ' << ((a - a) == -(a - a));
	s[2][1] << "1 1 1 1 1 1";
	
	BigInt t1(c1);
	BigInt t2(move(c1));
	s[2][0] << (c1 == 0) << ' ' << (t2 == t1);
	s[2][1] << "1 1";

	if (s[2][0].str() != s[2][1].str()) {
		cout << "Error in test3 (out, equality, unary minus, construct from BigInt BigInt[move])" << endl;
		cout << "s0: " << s[2][0].str() << endl;
		cout << "s1: " << s[2][1].str() << endl;
		return 0;
	}

	s[3][0] << (a <= a) << ' ' << (a <= b) << ' ' << (b <= b) << ' ' << (b <= a);
	s[3][1] << "1 0 1 1";
	s[3][0] << (a >= a) << ' ' << (a >= b) << ' ' << (b >= b) << ' ' << (b >= a);
	s[3][1] << "1 1 1 0";
	s[3][0] << (a < a) << ' ' << (a < b) << ' ' << (b < b) << ' ' << (b < a);
	s[3][1] << "0 0 0 1";
	s[3][0] << (a > a) << ' ' << (a > b) << ' ' << (b > b) << ' ' << (b > a);
	s[3][1] << "0 1 0 0";

	if (s[3][0].str() != s[3][1].str()) {
		cout << "Error in test4 (out, inequality)" << endl;
		cout << "s0: " << s[3][0].str() << endl;
		cout << "s1: " << s[3][1].str() << endl;
		return 0;
	}

	BigInt g(42), g1, g2;
	g1 = g;
	g2 = move(g);

	s[4][0] << (g == 0) << ' ' << (g1 == g2) << ' ' << (g1 == 42);
	s[4][1] << "1 1 1";
	
	if (s[4][0].str() != s[4][1].str()) {
		cout << "Error in test5 (out, operator=[move and copy])" << endl;
		cout << "s0: " << s[4][0].str() << endl;
		cout << "s1: " << s[4][1].str() << endl;
		return 0;
	}

	cout << "OK" << endl;
}
