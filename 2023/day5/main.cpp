#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct map {
	int64_t dst, src, len;
	map(int64_t a, int64_t b, int64_t c) : dst(a), src(b), len(c) {};
};

int64_t map_ranges(vector<struct map> &ranges, vector<struct map> &maps) {
	int64_t ret = -1;
	for (int i = 0; i < ranges.size(); i++) {
		auto r = ranges[i];
		for (auto &m: maps) {
			if (r.src >= m.src && (r.src + r.len - 1) <= (m.src + m.len)) {
				ranges[i].src = m.dst + r.src - m.src;
			} else if (r.src >= m.src && r.src < (m.src + m.len) &&
					(r.src + r.len - 1) >= (m.src + m.len)) {
				ranges[i].src = m.dst + r.src - m.src;
				ranges[i].len = m.src + m.len - r.src;
				ranges.push_back(map(-1, m.src + m.len, r.len - (m.src + m.len - r.src)));
			} else if (r.src < m.src && (r.src + r.len - 1) >= m.src &&
					(r.src + r.len - 1) < (m.src + m.len)) {
				ranges[i].src = m.dst;
				ranges[i].len = r.len - m.src + r.src;
				ranges.push_back(map(-1, r.src, m.src - r.src));
			} else if (r.src < m.src && (r.src + r.len - 1) >= (m.src + m.len)) {
				ranges[i].src = m.dst;
				ranges[i].len = m.len;
				ranges.push_back(map(-1, r.src, m.src - r.src));
				ranges.push_back(map(-1, m.src + m.len, r.len - m.src - r.len - m.len));
			}
		}
		if (ret == -1 || ranges[i].src < ret)
			ret = ranges[i].src;
	}
	return ret;
}

int main(int c, char **v) {
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);
	string line;
	getline(ifs, line);
	istringstream iss(line);
	iss.ignore(32, ' ');

	int64_t tmp1, tmp2;
	vector<struct map> maps, rangesOne, rangesTwo;
	while (iss >> tmp1 >> tmp2) {
		rangesOne.push_back(map(-1, tmp1, 1));
		rangesOne.push_back(map(-1, tmp2, 1));
		rangesTwo.push_back(map(-1, tmp1, tmp2));
	}

	while(getline(ifs, line)) {
		stringstream iss(line);
		int64_t val[3];
		if (!line.size())
			continue;
		if (line.find("map") != string::npos) {
			map_ranges(rangesOne, maps);
			map_ranges(rangesTwo, maps);
			maps.clear();
			continue;
		}

		for (int i = 0; i < sizeof(val)/sizeof(val[0]); iss >> val[i++]);
		maps.push_back(map(val[0], val[1], val[2]));
	}
	cout << "Part One: " << map_ranges(rangesOne, maps) << endl;
	cout << "Part Two: " << map_ranges(rangesTwo, maps) << endl;
	return 0;
}
