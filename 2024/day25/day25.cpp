#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	vector<vector<int>> keys, locks;

	for (string s; ifs >> s;) {
		bool key = true;
		if (s == ".....") {
			key = true;
			keys.push_back(vector<int>(5, 0));
		} else if (s == "#####") {
			key = false;
			locks.push_back(vector<int>(5, 0));
		}
		auto &curr = key ? keys.back() : locks.back();
		for (int y = 0; y < 5 && ifs >> s; y++) {
			for (int x = 0; x < 5; x++) {
				if (s[x] == '#')
					curr[x] ++;
			}
		}
		ifs >> s;
	}

	int partOne = 0;

	for (auto &k : keys) {
		for (auto &l : locks) {
			bool fit = true;
			for (int i = 0; i < 5; i++) {
				if ((k[i] + l[i]) > 5) {
					fit = false;
					break;
				}
			}
			if (fit)
				partOne ++;
		}
	}


	cout << "Part One: " << partOne << endl;

	return 0;
}
