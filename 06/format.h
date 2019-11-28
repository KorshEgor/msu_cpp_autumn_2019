#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

enum STATES { SEARCHOPEN, SEARCHCLOSE, CNTSTATES };

template<class... ArgsT> std::string format(const std::string &s, const ArgsT&... args);
void process(std::vector<std::string> &v);
template<class T> void process(std::vector<std::string> &v, const T& val);
template<class T, class... ArgsT> void process(std::vector<std::string> &v, const T& val, const ArgsT&... args);

template<class... ArgsT>
std::string format(const std::string &s, const ArgsT&... args) {
	std::vector<std::string> v;
	process(v, args...);

	std::stringstream out;
	std::size_t last = 0;
	STATES state = SEARCHOPEN;
	const char sym_search[CNTSTATES] = { '{', '}' };

	for (std::size_t i = 0; i < s.size(); ++i) {
		if (s[i] == sym_search[state]) {
			std::string tmp;
			switch (state) {
			case SEARCHOPEN:
				state = SEARCHCLOSE;
				out << s.substr(last, i - last);
				last = i + 1;
				break;
			case SEARCHCLOSE:
				state = SEARCHOPEN;
				tmp = s.substr(last, i - last);
				if (tmp.size() > 0 && '0' <= tmp[0] && tmp[0] <= '9') {
					std::size_t pos, num;
					num = stoul(tmp, &pos, 10);
					if (pos != tmp.size()) {
						throw std::runtime_error("violation of context {n}");
					}
					if (num >= v.size()) {
						throw std::runtime_error("wrong num of args");
					}
					out << v[num];
				}
				else {
					throw std::runtime_error("violation of context {n}");
				}
				last = i + 1;
				break;
			default:
				break;
			}
		}
		else if (s[i] == '}' && state == SEARCHOPEN) {
			throw std::runtime_error("violation of context {n}");
		}
	}

	if (state == SEARCHOPEN) {
		out << s.substr(last, s.size() - last);
		return out.str();
	}
	else {
		throw std::runtime_error("violation of context {n}");
	}
}

template<class T>
void process(std::vector<std::string> &v, const T& val) {
	std::stringstream st;
	st << val;
	v.push_back(st.str());
}

template<class T, class... ArgsT>
void process(std::vector<std::string> &v, const T& val, const ArgsT&... args) {
	std::stringstream st;
	st << val;
	v.push_back(st.str());
	process(v, args...);
}