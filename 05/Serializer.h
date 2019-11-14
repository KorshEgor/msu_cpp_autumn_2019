#pragma once
#include <ostream>
#include <cstdint>
#include "Error.h"

class Serializer {
	static constexpr char Separator = ' ';
	std::ostream &out_;

	Error doSave(bool val);
	Error doSave(std::uint64_t val);

	template <class T>
	Error process(T&& val) {
		return doSave(val);
	}

	template <class T, class... Args>
	Error process(T&& val, Args&&... args) {
		doSave(val);
		return process(std::forward<Args>(args)...);
	}
public:
	explicit Serializer(std::ostream& out);

	template <class T>
	Error save(const T& object) {
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args) {
		return process(args...);
	}
};
