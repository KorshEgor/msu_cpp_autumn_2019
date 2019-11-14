#include "Deserializer.h"
#include <cstdint>
#include <cstddef>

using namespace std;

Deserializer::Deserializer(std::istream& in) : in_(in) {}

Error Deserializer::doLoad(bool &val) {
	string s;
	in_ >> s;
	if (s == "true") {
		val = true;
	}
	else if (s == "false") {
		val = false;
	}
	else {
		return Error::CorruptedArchive;
	}
	return Error::NoError;
}

Error Deserializer::doLoad(uint64_t &val) {
	string s;
	in_ >> s;
	if (!s.size() || s[0] < '0' || '9' < s[0]) {
		return Error::CorruptedArchive;
	}
	size_t pos = 0;
	val = std::stoull(s, &pos, 10);
	if (pos != s.size()) {
		return Error::CorruptedArchive;
	}
	return Error::NoError;
}