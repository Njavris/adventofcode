#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

bool searchP1(int exclude, vector<string> &towels, string pattern, int offset = 0) {
	int ret = 0;
	if (offset == pattern.size())
		return true;
	string str = pattern.substr(offset);
	for (int i = 0; i < towels.size(); i++) {
		if (i == exclude)
			continue;
		auto &t = towels[i];
		if (t[0] != str[0] || t.size() > str.size())
			continue;
		if (str.starts_with(t)) {
			if (searchP1(exclude, towels, str, t.size()))
				return true;
		}

	}
	return false;
}

int partOne(vector<string> towels, vector<string> patterns) {
	int ret = 0;
	
	sort(begin(towels), end(towels));
	for (int i = 0; i < towels.size(); i++) {
		if (searchP1(i, towels, towels[i])) {
			towels.erase(begin(towels) + i);
			i --;
		}
	}

	for (auto &p : patterns)
		if (searchP1(-1, towels, p))
			ret++;
	return ret;
}

long int searchP2(unordered_map<int, long int> &cache,
			vector<string> &towels, string pat, int off = 0);
long int caching(unordered_map<int, long int> &cache,
			vector<string> &towels, string pat, int off = 0) {
	string tgt = pat.substr(off);
	int key = tgt.size();
	try {
		return cache.at(key);
	}
	catch (out_of_range) {
		cache[key] = searchP2(cache, towels, tgt);
	}
	return cache.at(key);
}

long int searchP2(unordered_map<int, long int> &cache,
			vector<string> &towels, string pat, int off) {
	long int ret = 0;

	for (int i = 0; i < towels.size(); i++) {
		auto &t = towels[i];
		if (!pat.starts_with(t))
			continue;
		if (pat.size() == t.size())
			ret++;
		else
			ret += caching(cache, towels, pat, t.size());
	}
	return ret;
}

long int partTwo(vector<string> towels, vector<string> patterns) {
	long int ret = 0;
	unordered_map<int, long int> cache;

	for (auto &p : patterns) {
		ret += caching(cache, towels, p);
		cache.clear();
	}
	
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	vector<string> towels;
	vector<string> patterns;

	string ln;
	getline(ifs, ln);
	for (int i = 2, s = 0; i <= ln.size(); i++) {
		if (ln.substr(i - 2, 2) == ", " || i == ln.size()) {
			int d = i == ln.size() ? 0 : 2;
			towels.push_back(ln.substr(s, i - s - d));
			s = i;
		}
	}

	while (ifs >> ln)
		patterns.push_back(ln);

	cout << "Part One: " << partOne(towels, patterns) << endl;
	cout << "Part Two: " << partTwo(towels, patterns) << endl;

	return 0;
}
