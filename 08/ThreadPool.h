#pragma once
#include <cstddef>
//#include <thread>
#include "mingw.thread.h"
//#include <future>
#include "mingw.future.h"
#include <vector>
#include <queue>
#include <functional>
//#include <condition_variable>
#include "mingw.condition_variable.h"
//#include <mutex>
#include "mingw.mutex.h"
#include <memory>

template <class Func, class... Args> 
static void task2(std::shared_ptr<std::promise<void>> p, Func func, Args... args);

template <class Promise, class Func, class... Args> 
static void task2(Promise p, Func func, Args... args);

class ThreadPool {
	std::queue<std::function<void()>> funcs;
	std::vector<std::thread> threads;
	std::condition_variable funcReady;
	volatile bool done = false;
	std::mutex m;
public:
	explicit ThreadPool(std::size_t poolSize);
	ThreadPool(ThreadPool &t) = delete;
	ThreadPool(ThreadPool &&t) = delete;
	~ThreadPool();
	void close();

	// pass arguments by value
	template <class Func, class... Args>
	auto exec(Func func, Args... args)->std::future<decltype(func(args...))> {
		auto promise = std::make_shared<std::promise<decltype(func(args...))>>();
		std::future<decltype(func(args...))> future = promise->get_future();

		auto task = [](std::shared_ptr<std::promise<decltype(func(args...))>> p, Func func, Args... args) {
			task2(p, func, args...);
		};
		{
			std::lock_guard<std::mutex> mylock(m);
			funcs.push(std::bind(task, promise, func, args...));
		}
		funcReady.notify_one();
		return future;
	}
};

template <class Func, class... Args>
static void task2(std::shared_ptr<std::promise<void>> p, Func func, Args... args) {
	func(args...);
	p->set_value();
}

template <class Promise, class Func, class... Args>
static void task2(Promise p, Func func, Args... args) {
	p->set_value(func(args...));
}