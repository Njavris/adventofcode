#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct map {
	int64_t dst;
	union {
		int64_t src;
		int64_t addr;
	};
	int64_t len;
	map(int64_t a, int64_t b, int64_t c) : dst(a), src(b), len(c) {};
	map(int64_t a, int64_t b) : dst(-1), src(a), len(b) {};
	bool operator< (map const& r) { return addr < r.addr; };
};

void map_ranges(vector<struct map> &ranges, vector<struct map> &maps) {
	vector<struct map> old = ranges;
	for (int i = 0; i < old.size(); i++) {
		auto &r = old[i];
		for (auto &m: maps) {
			if (r.addr >= m.src && (r.addr + r.len - 1) <= (m.src + m.len)) {
				/* range fits in map */
				ranges[i].src = m.dst + r.src - m.src;
			} else if (r.addr >= m.src && r.addr < (m.src + m.len) &&
					(r.addr + r.len - 1) >= (m.src + m.len)) {
				/* split into 2 ranges */
				struct map tmp(m.src + m.len, r.len - (m.src + m.len - r.addr));
				ranges[i].addr = m.dst + r.addr - m.src;
				ranges[i].len = m.src + m.len - r.addr;
				old.push_back(tmp);
				ranges.push_back(tmp);
			} else if (r.addr < m.src && (r.addr + r.len - 1) >= m.src &&
					(r.addr + r.len - 1) < (m.src + m.len)) {
				/* split into 2 ranges */
				struct map tmp(r.addr, m.src - r.addr);
				ranges[i].addr = m.dst;
				ranges[i].len = r.len - tmp.len;
				old.push_back(tmp);
				ranges.push_back(tmp);
			} else if (r.src < m.src && (r.addr + r.len - 1) >= (m.src + m.len)) {
				/* split into 3 ranges */
				struct map tmp1(r.addr, m.src - r.addr);
				ranges[i].addr = m.dst;
				ranges[i].len = m.len;
				struct map tmp2(m.src + m.len, r.len - tmp1.len - m.len);
				old.push_back(tmp1);
				old.push_back(tmp2);
				ranges.push_back(tmp1);
				ranges.push_back(tmp2);
			}
		}
	}
};

int main(int c, char **v) {
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);
	string line;
	getline(ifs, line);
	istringstream iss(line);
	iss.ignore(32, ' ');

	vector<struct map> maps, rangesOne, rangesTwo;
	int64_t tmp1, tmp2;
	while (iss >> tmp1 >> tmp2) {
		rangesOne.push_back(map(tmp1, 1));
		rangesOne.push_back(map(tmp2, 1));
		rangesTwo.push_back(map(tmp1, tmp2));
	}

	while(getline(ifs, line)) {
		if (!line.size())
			continue;
		if (line.find("map") != string::npos) {
			map_ranges(rangesOne, maps);
			map_ranges(rangesTwo, maps);
			maps.clear();
			continue;
		}

		stringstream iss(line);
		int64_t val[3];
		for (int i = 0; i < sizeof(val)/sizeof(val[0]); iss >> val[i++]);
		maps.push_back(map(val[0], val[1], val[2]));
	}
	map_ranges(rangesOne, maps);
	map_ranges(rangesTwo, maps);
	cout << "Part One: " << (*min_element(begin(rangesOne), end(rangesOne))).addr << endl;
	cout << "Part Two: " << (*min_element(begin(rangesTwo), end(rangesTwo))).addr << endl;
	return 0;
}
