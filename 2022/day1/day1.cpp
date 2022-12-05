#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
	std::string ln;
	std::ifstream input("input");
	std::vector<unsigned> elves;
	unsigned sum = 0;

	while (std::getline(input, ln)) {
		if (!ln.size()) {
			elves.push_back(sum);
			sum = 0;
			continue;
		}
		sum += std::stoi(ln);
	}
	elves.push_back(sum);

	std::vector<unsigned> sorted_elves(elves);
	sort(sorted_elves.begin(), sorted_elves.end());

	std::vector<unsigned> tmp;
	for (int i = sorted_elves.size() - 1; i >=0; i --) {
		tmp.push_back(sorted_elves[i]);
	}
	sorted_elves = tmp;
	for (int i = 0; i < sorted_elves.size(); i ++) {
		std::cout << sorted_elves[i] << std::endl;
	}
	std::cout << "top3 total = " << sorted_elves[0] + sorted_elves[1] + sorted_elves[2] << std::endl;

	return 0;
}
