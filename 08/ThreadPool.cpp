#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t poolSize) {
	for (size_t i = 0; i < poolSize; ++i) {
		threads.emplace_back([this]() {
			while (!done) {
				unique_lock<mutex> mylock(m);
				if (!funcs.empty()) {
					auto f(std::move(funcs.front()));
					funcs.pop();
					mylock.unlock();
					f();
				} else {
					funcReady.wait(mylock, [this]() { return done || !funcs.empty(); });
				}
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	close();
}

void ThreadPool::close() {
	if (!done) {
		done = true;
		funcReady.notify_all();
		for (auto &t : threads) {
			t.join();
		}
	}
}