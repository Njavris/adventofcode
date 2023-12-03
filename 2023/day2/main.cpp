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
	string line;

	while (getline(ifs, line)) {
		int cnt, resOne, resTwo[3] = { 0 }; // RGB
		string tmp, color;
		istringstream iss(line);

		iss >> tmp >> resOne >> tmp;

		while (iss >> cnt >> color) {
			cout << cnt << " " << color << endl;
			if (color.find("red") != string::npos) {
				if (cnt > 12)
					resOne = 0;
				if (cnt > resTwo[0])
					resTwo[0] = cnt;
			}
			if (color.find("green") != string::npos) {
				if (cnt > 13)
			   		resOne = 0;
				if (cnt > resTwo[1])
					resTwo[1] = cnt;
			}
			if (color.find("blue") != string::npos) {
				if (cnt > 14)
					resOne = 0;
				if (cnt > resTwo[2])
					resTwo[2] = cnt;
			}
		}
		partOne += resOne;
		partTwo += resTwo[0] * resTwo[1] * resTwo[2];
	}

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;
	return 0;
}
