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
#include "ThreadPool.h"

using namespace std;

mutex m;

void part_sort(ifstream &fin, mutex &file_m, size_t &num, mutex &num_m) {
	//2мб
	vector<uint64_t> v;
	constexpr size_t maxsize = 1024 * 1024 * 2 / sizeof(v[0]) - 20;
	v.resize(maxsize);
	vector<uint64_t>(v).swap(v); //уменьшение размера реальной памяти
	v.clear();

	size_t cur_num;
	while (1) {
		v.clear();
		for (size_t i = 0; i < maxsize; ++i) {
			unique_lock<mutex> unique(file_m);
			uint64_t tmp;
			if (fin.read((char*)&tmp, sizeof(tmp))) {
				unique.unlock();
				v.push_back(tmp);
			}
			else {
				break;
			}
		}
		if (v.empty()) {
			return;
		}
		else {
			lock_guard<mutex> guard(num_m);
			cur_num = num;
			num++;
		}
		sort(v.begin(), v.end());
		stringstream sout;
		sout << "tmp" << cur_num << ".bin";
		ofstream fout(sout.str(), ios::binary | ios::out);
		for (auto i : v) {
			fout.write((char*)&i, sizeof(i));
		}
	}
}

void union_part_sort(ofstream &fout, vector<string>::const_iterator begin, vector<string>::const_iterator end) {
	typedef pair<uint64_t, size_t> qtype;
	size_t size = end - begin;
	vector<ifstream> files;
	priority_queue<qtype, vector<qtype>, std::greater<qtype> > q;

	for (auto i = begin; i != end; ++i) {
		files.emplace_back(*i, ios::binary | ios::in);
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
	mutex m1, m2;
	ifstream fin(name_in, ios::binary | ios::in);
	size_t cnt_parts = 0;

	auto ret1 = threads.exec(part_sort, ref(fin), ref(m1), ref(cnt_parts), ref(m2));
	auto ret2 = threads.exec(part_sort, ref(fin), ref(m1), ref(cnt_parts), ref(m2));
	ret1.get();
	ret2.get();
	fin.close();

	vector<string> names;
	for (size_t i = 0; i < cnt_parts; ++i) {
		stringstream sout;
		sout << "tmp" << i << ".bin";
		names.push_back(sout.str());
	}

	ofstream fout(name_out, ios::binary | ios::out);

	union_part_sort(fout, names.cbegin(), names.cend());
	fout.close();

	for (const auto &i : names) {
		remove(i.c_str());
	}
}