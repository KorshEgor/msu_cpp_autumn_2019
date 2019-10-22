#include "LinearAllocator.h"
#include <cstdlib>

using namespace std;

LinearAllocator::LinearAllocator(size_t maxSize) : mem((char*) malloc(maxSize)), cur(mem), last(mem + maxSize) {}

LinearAllocator::~LinearAllocator() {
	free(mem);
}

char* LinearAllocator::alloc(size_t size) {
	if (mem && size && cur + size <= last) {
		cur += size;
		return cur - size;
	}
	return nullptr;
}

void LinearAllocator::reset() {
	cur = mem;
}