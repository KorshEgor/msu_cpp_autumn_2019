#pragma once
#include <cstddef>

class LinearAllocator{
	char *mem, *cur, *last;
public:
	LinearAllocator(std::size_t maxSize);
	~LinearAllocator();
	char* alloc(std::size_t size);
	void reset();
};