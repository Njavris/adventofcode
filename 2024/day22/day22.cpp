#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

long int partOne(vector<int> &input) {
	long int ret = 0;
	for_each(begin(input), end(input), [&ret] (uint32_t secret) {
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
	return 0;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<int> input;
	for (string s; ifs >> s; input.push_back(stoi(s)));

	cout << "Part One: " << partOne(input) << endl;
	cout << "Part Two: " << partTwo(input) << endl;

	return 0;
}
