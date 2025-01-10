#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

long int partOne(vector<int> &input) {
	long int ret = 0;
	for_each(begin(input), end(input), [&ret] (unsigned secret) {
		for (int i = 0; i < 2000; i++) {
			secret ^= (secret << 6) & 0xffffff;
			secret ^= (secret >> 5) & 0xffffff;
			secret ^= (secret << 11) & 0xffffff;
		}
		ret += secret;
	});
	return ret;
};

long int partTwo(vector<int> &input) {
	vector<unordered_map<unsigned, int>> maps;
	for_each(begin(input), end(input), [&maps] (unsigned secret) {
		maps.push_back(unordered_map<unsigned, int>());
		auto &buyer = maps.back();
		unsigned seq = 0;
		int prev = 0;
		for (int i = 0; i < 2000; i++) {
			secret ^= (secret << 6) & 0xffffff;
			secret ^= (secret >> 5) & 0xffffff;
			secret ^= (secret << 11) & 0xffffff;
			int curr = secret % 10;
			int delta = curr - prev;
			prev = curr;
			seq <<= 8;
			seq |= (delta & 0xff);
			if (i > 2 && !buyer.count(seq))
				buyer[seq] = curr;
		}
	});

	long int ret = 0;
	for (int i = 0; i < maps.size(); i++) {
		auto &map = maps[i];
		for (auto &[k,v] : map) {
			long int s = v;
			for (int j = i + 1; j < maps.size(); j++) {
				auto &m = maps[j];
				if (m.count(k)) {
					s += m[k];
					m.erase(k);
				}
			}
			if (s > ret)
				ret = s;
		}
	}
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<int> input;
	for (string s; ifs >> s; input.push_back(stoi(s)));

	cout << "Part One: " << partOne(input) << endl;
	cout << "Part Two: " << partTwo(input) << endl;

	return 0;
}
