#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

long int search(long int *cache, vector<string> &towels, string pat, int off = 0) {
	long int ret = 0;
	string tgt = pat.substr(off);

	for (int i = 0; i < towels.size(); i++) {
		auto &t = towels[i];
		if (!tgt.starts_with(t))
			continue;

		if (tgt.size() == t.size()) {
			ret++;
		} else {
			int k = tgt.size() - t.size() - 1;
			if (cache[k] == ~0)
				cache[k] =  search(cache, towels, tgt, t.size());
			ret += cache[k];
		}
	}
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	vector<string> towels;

	string ln;

	getline(ifs, ln);
	for (int i = 2, s = 0; i <= ln.size(); i++) {
		if (ln.substr(i - 2, 2) == ", " || i == ln.size()) {
			int d = i == ln.size() ? 0 : 2;
			towels.push_back(ln.substr(s, i - s - d));
			s = i;
		}
	}

	int partOne = 0;
	long int partTwo = 0;

	while (ifs >> ln) {
		long int *cache = new long int[ln.size()];
		memset(cache, ~0, ln.size() * sizeof(long int));

		unsigned long ret = search(cache, towels, ln);
		if (ret)
			partOne++;
		partTwo += ret;

		delete[] cache;
	}

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;

	return 0;
}
