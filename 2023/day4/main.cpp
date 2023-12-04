#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int c, char **v) {
	int partOne = 0, partTwo = 0;
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);
	vector<int> count, numbers;
	string line;
	for (int ln = 0, c = 0, l = 0, n = 0; getline(ifs, line); ln ++, numbers.clear(), c = l = n = 0) {
		string tmp;
		istringstream iss(line);
		iss >> tmp >> tmp;
		while (iss >> tmp) {
			if (++ n && tmp == "|" && (l = n - 1))
				continue;
			numbers.push_back(stoi(tmp));
		}
		if (ln <= count.size())
			count.push_back(0);
		for (int i = l; i < numbers.size(); i ++) {
			for (int j = 0; j < l; j ++) {
				if (numbers[i] == numbers[j] && ++ c) {
					if (count.size() <= ln + c)
						count.push_back(0);
					count[ln + c] += count[ln] + 1;
					break;
				}
			}
		}
		partOne += 1 << (c - 1);
		partTwo += count[ln] + 1;
	}
	cout << "Part One: " << partOne << endl << "Part Two: " << partTwo << endl;
	return 0;
}
