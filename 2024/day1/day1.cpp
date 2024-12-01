#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char **argv) {
	int partOne = 0;
	int partTwo = 0;
	if (argc != 2)
		return -1;

	std::vector<int> leftList, rightList;

	std::string line;
	std::ifstream ifs(argv[1]);
	while (std::getline(ifs, line)) {
		int left, right;
		std::stringstream ss(line);
		ss >> left >> right;
		leftList.push_back(left);
		rightList.push_back(right);
	}
	std::sort(begin(leftList), end(leftList));
	std::sort(begin(rightList), end(rightList));

	for (int i = 0; i < leftList.size(); i++) {
		int dist = leftList[i] - rightList[i];
		int left = leftList[i];
		int val = 0;
		if (dist < 0)
			dist *= -1;
		partOne += dist;

		std::for_each(begin(rightList), end(rightList),
			[&val, &left] (int curr) {
				val += (curr == left) ? 1 : 0;
			});
		partTwo += val * left;
	}

	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;
	return 0;
}
