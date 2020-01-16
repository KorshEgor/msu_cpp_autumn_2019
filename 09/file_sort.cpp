#include "file_sort.h"
#include <cstdint>
#include <cstddef>
#include <string>
#include <mutex>
#include <functional>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <cstdio>
#include <stdexcept>
#include "ThreadPool.h"

using namespace std;

void part_sort(vector<uint64_t>::iterator begin, vector<uint64_t>::iterator end, size_t cur_num) {
	sort(begin, end);

	stringstream sout;
	sout << "tmp" << cur_num << ".bin";
	ofstream fout(sout.str(), ios::binary | ios::out);
	if (!fout.is_open()) {
		throw runtime_error("Error open file: " + sout.str());
	}

	for (auto i = begin; i != end; ++i) {
		auto tmp = *i;
		fout.write((char*)&tmp, sizeof(tmp));
	}
}

void union_part_sort(ofstream &fout, vector<string>::const_iterator begin, vector<string>::const_iterator end) {
	typedef pair<uint64_t, size_t> qtype;
	size_t size = end - begin;
	vector<ifstream> files;
	priority_queue<qtype, vector<qtype>, std::greater<qtype> > q;

	size_t num = 0;
	for (auto i = begin; i != end; ++i) {
		files.emplace_back(*i, ios::binary | ios::in);
		if (!files[num].is_open()) {
			throw runtime_error("Error open file: " + *i);
		}
		++num;
	}

	for (size_t i = 0; i < files.size(); ++i) {
		if (files[i].is_open()) {
			uint64_t tmp;
			if (files[i].read((char*)&tmp, sizeof(tmp))) {
				q.push(make_pair(tmp, i));
			}
		}
	}

	while (!q.empty()) {
		uint64_t tmp;
		size_t i;
		tie(tmp, i) = q.top();
		q.pop();
		fout.write((char*)&tmp, sizeof(tmp));
		if (files[i].read((char*)&tmp, sizeof(tmp))) {
			q.push(make_pair(tmp, i));
		}
	}
}

void file_sort(const string &name_in, const string &name_out) {
	ThreadPool threads(2);
	ifstream fin(name_in, ios::binary | ios::in);
	if (!fin.is_open()) {
		throw runtime_error("Error open input file: " + name_in);
	}
	size_t cnt_parts = 0;

	vector<uint64_t> v;
	constexpr size_t maxsize = 1024 * 1024 * 8 / sizeof(v[0]) - 1000;
	v.resize(maxsize);
	size_t cnt = 0;
	while (fin.read((char*)&(v[cnt]), sizeof(v[0]))) {
		if (++cnt == maxsize) {
			auto ret1 = threads.exec(part_sort, v.begin(), v.begin() + cnt / 2, cnt_parts);
			auto ret2 = threads.exec(part_sort, v.begin() + cnt / 2, v.begin() + cnt, cnt_parts + 1);
			cnt_parts += 2;
			cnt = 0;
			ret1.get();
			ret2.get();
		}
	}
	fin.close();
	if (cnt) {
		auto ret1 = threads.exec(part_sort, v.begin(), v.begin() + cnt / 2, cnt_parts);
		auto ret2 = threads.exec(part_sort, v.begin() + cnt / 2, v.begin() + cnt, cnt_parts + 1);
		cnt_parts += 2;
		cnt = 0;
		ret1.get();
		ret2.get();
	}

	vector<string> names;
	for (size_t i = 0; i < cnt_parts; ++i) {
		stringstream sout;
		sout << "tmp" << i << ".bin";
		names.push_back(sout.str());
	}

	ofstream fout(name_out, ios::binary | ios::out);
	if (!fout.is_open()) {
		throw runtime_error("Error open output file: " + name_out);
	}

	union_part_sort(fout, names.cbegin(), names.cend());
	fout.close();

	for (const auto &i : names) {
		if (remove(i.c_str())) {
			throw runtime_error("Error remove file: " + i + ", next files will NOT be deleted.");
		}
	}
}