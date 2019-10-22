#include <iostream>
#include <vector>
#include "LinearAllocator.h"

using namespace std;

size_t check(const vector<size_t> &s, size_t alloc_size);

int main() {
	size_t n = 0;
	if ((n = check(vector<size_t>{ 10, 50, 0, 10, 40 }, 100))) {
		cout << "Error on 1 allocator in test " << n << endl;
	}
	if ((n = check(vector<size_t>{ 10, 50, 0, 10, 40, 500, 500, 390, 1 }, 1000))) {
		cout << "Error on 2 allocator in test " << n << endl;
	}
	if ((n = check(vector<size_t>{ 10, 50, 0, 10, 40 }, 0))) {
		cout << "Error on 3 allocator in test " << n << endl;
	}
	cout << "done" << endl;
	return 0;
}

size_t check(const vector<size_t> &s, size_t alloc_size) {
	LinearAllocator alloc(alloc_size);
	
	vector<char*> p(s.size());
	for (size_t i = 0; i < p.size(); ++i) {
		p[i] = alloc.alloc(s[i]);
	}

	char *last = nullptr;
	size_t summ = 0, last_s = 0;
	for (size_t i = 0; i < s.size(); ++i) {
		if (s[i] && summ + s[i] <= alloc_size) {
			if (last && p[i] != last + last_s || !last && !p[i]) {
				return i + 1;
			}
			summ += s[i];
			last_s = s[i];
			last = p[i];
		} else {
			if (p[i]) {
				return i + 1;
			}
		}
	}

	alloc.reset();
	for (size_t i = 0; i < p.size(); ++i) {
		if (p[i] != alloc.alloc(s[i])) {
			return i + p.size() + 1;
		}
	}
	return 0;
}