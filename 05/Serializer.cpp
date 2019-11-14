#include "Serializer.h"
#include <cstdint>

Serializer::Serializer(std::ostream& out) : out_(out) {}

Error Serializer::doSave(bool val) {
	out_ << (val ? "true" : "false") << Separator;
	return Error::NoError;
}

Error Serializer::doSave(std::uint64_t val) {
	out_ << val << Separator;
	return Error::NoError;
}