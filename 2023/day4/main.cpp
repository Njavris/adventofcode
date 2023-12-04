#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

int main(int c, char **v) {
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);
	int partOne = 0, partTwo = 0;
	vector<int> match, count;
	string line;

	while (getline(ifs, line)) {
		string tmp;
		int cnt = 0;
		vector<int> numbers;
		istringstream iss(line);
		iss >> tmp >> tmp;
		while (iss >> tmp) {
			if (tmp == "|")
				continue;
			numbers.push_back(stoi(tmp));
		}
		for (int i = 10; i < numbers.size(); i++) {
			for (int j = 0; j < 10; j++) {
				if (numbers[i] == numbers[j]) {
					cnt ++;
					break;
				}
			}
		}
		partOne += pow(2, cnt - 1);
		match.push_back(cnt);
		count.push_back(1);
	}

	for (int i = 0; i < match.size(); i++) {
		for (int j = 0; j < count[i]; j++)
			for (int k = 1; k <= match[i] && i + k < match.size(); k++)
				count[i + k] ++;
		partTwo += count[i];
	}
	cout << "Part Two: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;
	return 0;
}
