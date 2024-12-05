#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");

	std::string line;
	std::vector<std::vector<int>> rules;

	while (getline(ifs, line)) {
		int x, y;
		if (!line.size())
			break;
		sscanf(line.c_str(), "%d|%d", &x, &y);
		rules.push_back(std::vector({x, y}));
	}

	while (getline(ifs, line)) {
		std::vector<int> pages;
		std::stringstream ss(line);
		std::string tmp;
		while (getline(ss, tmp, ','))
			pages.push_back(stoi(tmp));

		bool valid = true;
		for (auto &r: rules) {
			auto xIt = std::find(begin(pages), end(pages), r[0]);
			auto yIt = std::find(begin(pages), end(pages), r[1]);
			if (xIt > yIt && xIt != end(pages) && yIt != end(pages)) {
				valid = false;
				break;
			}
		}
		if (valid) {
			partOne += pages[pages.size() / 2];
			continue;
		}

		for (int i = 0; i < rules.size(); i++) {
			auto xIt = std::find(begin(pages), end(pages), rules[i][0]);
			auto yIt = std::find(begin(pages), end(pages), rules[i][1]);
			if (xIt > yIt && xIt != end(pages) && yIt != end(pages)) {
				std::rotate(yIt, yIt + 1, xIt + 1);
				i = 0;
			}
		}
		partTwo += pages[pages.size() / 2];
	}
	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;

	return 0;
}
