#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum STATES { SEARCHOPEN, SEARCHCLOSE, CNTSTATES}; 

template<class... ArgsT> string format(const string &s, const ArgsT&... args);
void process(vector<string> &v);
template<class T> void process(vector<string> &v, const T& val);
template<class T, class... ArgsT> void process(vector<string> &v, const T& val, const ArgsT&... args);

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
	test_except("violation of context {n}", "{-0}", 42);
	test_except("violation of context {n}", "{0 }", 42);
	test_except("violation of context {n}", "{0}, {00000", 42);
	test_except("wrong num of args", "{0}");
	test_except("wrong num of args", "{0} {3} {1} ", 42, 41, 40);

	cout << "OK" << endl;
	return 0;
}

template<class... ArgsT>
string format(const string &s, const ArgsT&... args) {
	vector<string> v;
	process(v, args...);

	stringstream out;
	size_t last = 0;
	STATES state = SEARCHOPEN;
	const char sym_search[CNTSTATES] = { '{', '}' };

	for (size_t i = 0; i < s.size(); ++i) {
		if (s[i] == sym_search[state]) {
			string tmp;
			switch (state)
			{
			case SEARCHOPEN:
				state = SEARCHCLOSE;
				out << s.substr(last, i - last);
				last = i + 1;
				break;
			case SEARCHCLOSE:
				state = SEARCHOPEN;
				tmp = s.substr(last, i - last);
				if (tmp.size() > 0 && '0' <= tmp[0] && tmp[0] <= '9') {
					size_t pos, num;
					num = stoul(tmp, &pos, 10);
					if (pos != tmp.size()) {
						throw runtime_error("violation of context {n}");
					}
					if (num >= v.size()) {
						throw runtime_error("wrong num of args");
					}
					out << v[num];
				}
				else {
					throw runtime_error("violation of context {n}");
				}
				last = i + 1;
				break;
			default:
				break;
			}
		}
	}

	if (state == SEARCHOPEN) {
		out << s.substr(last, s.size() - last);
		return out.str();
	}
	else {
		throw runtime_error("violation of context {n}");
	}
}

void process(vector<string> &v) {}

template<class T>
void process(vector<string> &v, const T& val) {
	stringstream st;
	st << val;
	v.push_back(st.str());
}

template<class T, class... ArgsT>
void process(vector<string> &v, const T& val, const ArgsT&... args) {
	stringstream st;
	st << val;
	v.push_back(st.str());
	process(v, args...);
}

template<class... ArgsT>
void test_except(const string &msg, const ArgsT&... args) {
	try {
		format(args...);
		cout << "Error test_except, \"" << msg << "\"" << endl;
		cout << "No throw" << endl;
		exit(0);
	}
	catch (const exception &e) {
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