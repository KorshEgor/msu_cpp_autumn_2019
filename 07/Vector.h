#pragma once
#include <exception>
#include <limits>
#include <iterator>

template <class T>
class Allocator {
public:
	T *allocate(std::size_t n) {
		T *p = (T*) malloc(n * sizeof(T));
		if (p == nullptr) {
			throw std::bad_alloc();
		}
		return p;
	}

	void deallocate(T *p, std::size_t n) {
		free(p);
	}

	size_t max_size() const noexcept {
		return std::numeric_limits<std::size_t>::max() / sizeof(T);
	}

	template<class... Args>
	void construct(T *p, std::size_t n, const Args&... val) {
		for (std::size_t i = 0; i < n; i++) {
			new (p + i) T(val...);
		}
	}

	template<class Arg>
	void construct(T *p, Arg&& val) {
		new (p) T(std::forward<Arg>(val));
	}

	void destroy(T *p, std::size_t n) {
		for (std::size_t i = 0; i < n; i++) {
			(p[i]).~T();
		}
	}
};

template <class T>
class Iterator {
	T *p;
	bool reverse;
public:
	explicit Iterator(T *ptr, bool reverse) : p(ptr), reverse(reverse) {}

	bool operator==(const Iterator<T>& other) const {
		return (p == other.p) && (reverse == other.reverse);
	}

	bool operator!=(const Iterator<T>& other) const {
		return !(*this == other);
	}

	T& operator*() const {
		return *p;
	}

	Iterator& operator++() {
		if (reverse) {
			--p;
		} else {
			++p;
		}
		return *this;
	}

	Iterator operator++(int) {
		Iterator it(p, reverse);
		if (reverse) {
			p--;
		} else {
			p++;
		}
		return it;
	}

	Iterator& operator--() {
		if (reverse) {
			++p;
		} else {
			--p;
		}
		return *this;
	}

	Iterator operator--(int) {
		Iterator it(p, reverse);
		if (reverse) {
			p++;
		} else {
			p--;
		}
		return it;
	}

	Iterator& operator+=(std::size_t n) {
		if (reverse) {
			p -= n;
		} else {
			p += n;
		}
		return *this;
	}

	Iterator& operator-=(std::size_t n) {
		if (reverse) {
			p += n;
		} else {
			p -= n;
		}
		return *this;
	}

	Iterator operator+(std::size_t n) {
		Iterator it(p, reverse);
		it += n;
		return it;
	}

	Iterator operator-(std::size_t n) {
		Iterator it(p, reverse);
		it -= n;
		return it;
	}
};

template <class T, class Alloc = Allocator<T>>
class Vector {
	Alloc alloc;
	std::size_t cur_size;
	std::size_t cur_capacity;
	T *mass;
public:
	Vector() {
		alloc = Alloc();
		mass = nullptr;
		cur_size = 0;
		cur_capacity = 0;
	}

	template<class... Args>
	explicit Vector(std::size_t n, const Args&... val) {
		alloc = Alloc();
		mass = alloc.allocate(n);
		alloc.construct(mass, n, val...);
		cur_size = n;
		cur_capacity = n;
	}

	~Vector() {
		alloc.destroy(mass, cur_size);
		alloc.deallocate(mass, cur_capacity);
	}

	Iterator<T> begin() noexcept {
		return Iterator<T>(mass, false);
	}
	Iterator<T> rbegin() noexcept {
		return Iterator<T>(mass + cur_size - 1, true);
	}

	Iterator<T> end() noexcept {
		return Iterator<T>(mass + cur_size, false);
	}
	Iterator<T> rend() noexcept {
		return Iterator<T>(mass - 1, true);
	}

	T &operator[](std::size_t i) {
		return mass[i];
	}

	const T &operator[](std::size_t i) const {
		return mass[i];
	}

	template<class Arg>
	void push_back(Arg&& value) {
		if (cur_capacity == 0) {
			mass = alloc.allocate(1);
			alloc.construct(mass, std::forward<Arg>(value));
			cur_size = 1;
			cur_capacity = 1;
		}
		else if (cur_size == cur_capacity) {
			T *new_data = alloc.allocate(cur_capacity * 2);
			alloc.construct(new_data, cur_size);
			std::copy(mass, mass + cur_size, new_data);
			alloc.destroy(mass, cur_size);
			alloc.deallocate(mass, cur_capacity);
			mass = new_data;
			alloc.construct(mass + cur_size++, std::forward<Arg>(value));
			cur_capacity *= 2;
		}
		else if (cur_size < cur_capacity) {
			alloc.construct(mass + cur_size++, std::forward<Arg>(value));
		}
	}

	void pop_back() {
		alloc.destroy(mass + --cur_size, 1);
	}

	void reserve(std::size_t n) {
		if (n > cur_capacity) {
			T *new_data = alloc.allocate(n);
			alloc.construct(new_data, cur_size);
			std::copy(mass, mass + cur_size, new_data);
			alloc.destroy(mass, cur_size);
			alloc.deallocate(mass, cur_capacity);
			mass = new_data;
			cur_capacity = n;
		}
	}

	template<class... Args>
	void resize(std::size_t newSize, const Args&... val) {
		if (newSize < cur_size) {
			alloc.destroy(mass + newSize, cur_size - newSize);
			cur_size = newSize;
		}
		else if (newSize > cur_size) {
			if (newSize > cur_capacity) {
				T *new_data = alloc.allocate(newSize);
				alloc.construct(new_data, cur_size);
				std::copy(mass, mass + cur_size, new_data);
				alloc.destroy(mass, cur_size);
				alloc.deallocate(mass, cur_capacity);
				mass = new_data;
				cur_capacity = newSize;
			}
			alloc.construct(mass + cur_size, newSize - cur_size, val...);
			cur_size = newSize;
		}
	}

	bool empty() {
		return cur_size == 0;
	}

	void clear() noexcept {
		alloc.destroy(mass, cur_size);
		cur_size = 0;
	}

	std::size_t size() {
		return cur_size;
	}

	std::size_t capacity() {
		return cur_capacity;
	}
};