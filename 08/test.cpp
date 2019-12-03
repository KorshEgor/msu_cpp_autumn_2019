#include "ThreadPool.h"
#include <iostream>
#include <memory>
#include <vector>
#include <functional>

using namespace std;

struct A {};

mutex m;

void foo(const A&) { lock_guard<mutex> mylock(m); cout << "lets foo!" << endl; }

int main() {
	ThreadPool pool(8);
	{
		lock_guard<mutex> mylock(m);
		cout << "lets exec!" << endl;
	}
	auto task1 = pool.exec(foo, A());
	{
		lock_guard<mutex> mylock(m);
		cout << "lets get!" << endl;
	}
	task1.get();
	{
		lock_guard<mutex> mylock(m);
		cout << "lets exec!" << endl;
	}
	auto task2 = pool.exec([]() { lock_guard<mutex> mylock(m);  cout << "lets lambda!" << endl; return 1; });
	{
		lock_guard<mutex> mylock(m);
		cout << "lets get!" << endl;
	}
	task2.get();
	/*
	{
		lock_guard<mutex> mylock(m);
		cout << "lets close!" << endl;
	}
	pool.close();
	*/
	{
		lock_guard<mutex> mylock(m);
		cout << "lets return!" << endl;
	}
	return 0;
}
