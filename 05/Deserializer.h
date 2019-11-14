#pragma once
#include <istream>
#include <string>
#include <cstdint>
#include "Error.h"

class Deserializer {
	static constexpr char Separator = ' ';
	std::istream &in_;

	Error doLoad(bool &val);
	Error doLoad(std::uint64_t &val);

	template <class T>
	Error process(T&& val) {
		return doLoad(val);
	}

	template <class T, class... Args>
	Error process(T&& val, Args&&... args) {
		const Error err = doLoad(val);
		if (err != Error::NoError) {
			return err;
		}
		return process(std::forward<Args>(args)...);
	}
public:
	explicit Deserializer(std::istream& in);

	template <class T>
	Error load(T& object) {
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args) {
		return process(std::forward<ArgsT>(args)...);
	}
};
