#include <iostream>
#include <fstream>
#include <vector>


int solveP1(std::vector<std::string> &input, int x, int y, int dirX, int dirY, std::string str) {
	int matches = 0;
	while (true) {
		int endX = x + (dirX * (str.length() - 1));
		int endY = y + (dirY * (str.length() - 1));
		if (endX < 0 || endX >= input[0].size() || endY < 0 || endY >= input.size()) {
			return matches;
		}
		bool match = true, matchRev = true;
		for (int i = 0; i < str.size(); i++) {
			int c = input[y + (i * dirY)][x + (i * dirX)];
			if (c != str[i])
				match = false;
			if (c != str[str.size() - 1 - i])
				matchRev = false;
			if (!match && !matchRev)
				break;
		}
		if (match || matchRev)
			matches ++;
		x += dirX;
		y += dirY;
	}
	return matches;
};

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");
	std::string line;
	std::vector<std::string> input;
	while (getline(ifs, line))
		input.push_back(line);

	for (int i = 0; i < input.size(); i++) {
		/* Horizontal */
		partOne += solveP1(input, 0, i, 1, 0, "XMAS");
		/* Vertical */
		partOne += solveP1(input, i, 0, 0, 1, "XMAS");
		/* Rising diagonals */
		partOne += solveP1(input, i, 0, 1, 1, "XMAS");
		partOne += solveP1(input, 0, i + 1, 1, 1, "XMAS");
		/* Falling diagonals */
		partOne += solveP1(input, 0, i, 1, -1, "XMAS");
		partOne += solveP1(input, i + 1, input.size() - 1, 1, -1, "XMAS");
	}

	for (int y = 0; y < input.size() - 2; y++) {
		for (int x = 0; x < input[0].size() - 2; x++) {
			std::vector<std::string> window;
			for (int i = 0; i < 3; i++)
				window.push_back(input[y + i].substr(x, 3));
			if (solveP1(window, 0, 0, 1, 1, "MAS") && solveP1(window, 0, 2, 1, -1, "MAS"))
				partTwo++;
		}
	}

	std::cout << "PartOne: " << partOne << std::endl;
	std::cout << "PartTwo: " << partTwo << std::endl;
	return 0;
}
