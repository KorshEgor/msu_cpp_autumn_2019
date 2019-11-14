#include "Serializer.h"
#include "Deserializer.h"
#include <iostream>
#include <sstream>

using namespace std;

struct Data1 {
	uint64_t a;
	bool b;
	uint64_t c;
	bool d;

	Data1(uint64_t a = 0, bool b = false, uint64_t c = 0, bool d = false) : a(a), b(b), c(c), d(d) {}

	template <class Serializer>
	Error serialize(Serializer& serializer) {
		return serializer(a, b, c, d);
	}
	
	template <class Serializer>
	Error serialize(Serializer& serializer) const {
		return serializer(a, b, c, d);
	}

	bool operator==(const Data1 &data) const {
		return (a == data.a) && (b == data.b) && (c == data.c) && (d == data.d);
	}

	bool operator!=(const Data1 &d) const {
		return !(*this == d);
	}
};

struct Data2 {
	uint64_t a;
	bool b;

	Data2(uint64_t a = 0, bool b = false) : a(a), b(b) {}

	template <class Serializer>
	Error serialize(Serializer& serializer) {
		return serializer(a, b);
	}

	template <class Serializer>
	Error serialize(Serializer& serializer) const {
		return serializer(a, b);
	}

	bool operator==(const Data2 &data) const {
		return (a == data.a) && (b == data.b);
	}

	bool operator!=(const Data2 &d) const {
		return !(*this == d);
	}
};

int main() {
	stringstream s, s_test;
	Serializer serial(s);
	Deserializer deser(s);

	Data1 x11(10, true, 20, false), x12;
	Data2 x21(10, true), x22;
	
	if (serial.save(x11) != Error::NoError || s.str() != "10 true 20 false ") {
		cout << "Error save Data1";
		return 0;
	}
	if (serial.save(x21) != Error::NoError || s.str() != "10 true 20 false 10 true ") {
		cout << "Error save Data2";
		return 0;
	}
	if (deser.load(x12) != Error::NoError || deser.load(x22) != Error::NoError || x11 != x12 || x21 != x22) {
		cout << "Error load Data1 or Data2";
		return 0;
	}
	if (serial.save(x21) != Error::NoError || serial.save(x21) != Error::NoError) {
		cout << "Error save Data2";
		return 0;
	}
	if (deser.load(x12) != Error::NoError || x12 != Data1(10, true, 10, true)) {
		cout << "Error load Data1 from Data2";
		return 0;
	}

	Deserializer des_test(s_test);
	s_test << "-10 true";
	if (des_test.load(x22) != Error::CorruptedArchive) {
		cout << "Error1: load from CorruptedArchive done" << endl;
		return 0;
	}

	s_test.clear();
	s_test << "10 trYe";
	if (des_test.load(x22) != Error::CorruptedArchive) {
		cout << "Error2: load from CorruptedArchive done" << endl;
		return 0;
	}

	s_test.clear();
	s_test << "10a true";
	if (des_test.load(x22) != Error::CorruptedArchive) {
		cout << "Error3: load from CorruptedArchive done" << endl;
		return 0;
	}

	s_test.clear();
	s_test << "10 falsee";
	if (des_test.load(x22) != Error::CorruptedArchive) {
		cout << "Error4: load from CorruptedArchive done" << endl;
		return 0;
	}
	
	cout << "OK" << endl;
	return 0;
}
