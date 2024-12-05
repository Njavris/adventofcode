#include <iostream>
#include <fstream>
#include <vector>


int findStr(std::vector<std::string> &input, int x, int y, int dirX, int dirY, int len, std::string str, int offX = 0, int offY = 0) {
	int matches = 0;
	for (;;x += dirX, y += dirY) {
		int endX = x + (dirX * (str.length() - 1));
		int endY = y + (dirY * (str.length() - 1));
		if (endX < 0 || endX >= len || endY < 0 || endY >= len) {
			return matches;
		}
		bool match = true, matchRev = true;
		for (int i = 0; i < str.size(); i++) {
			int c = input[offY + y + (i * dirY)][offX + x + (i * dirX)];
			if (c != str[i])
				match = false;
			if (c != str[str.size() - 1 - i])
				matchRev = false;
			if (!match && !matchRev)
				break;
		}
		if (match || matchRev)
			matches ++;
	}
	return matches;
};

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");
	std::string line;
	std::vector<std::string> input;
	while (getline(ifs, line)) {
		static int l = 0;
		input.push_back(line);
		if (l++ < 2)
		    continue;
		for (int x = 0; x < input[0].size() - 2; x++) {
			if (findStr(input, 0, 0, 1, 1, 3, "MAS", x, l - 3) &&
					findStr(input, 0, 2, 1, -1, 3, "MAS", x, l - 3))
				partTwo++;
		}
	}

	for (int i = 0; i < input.size(); i++) {
		/* Horizontal */
		partOne += findStr(input, 0, i, 1, 0, input.size(), "XMAS");
		/* Vertical */
		partOne += findStr(input, i, 0, 0, 1, input.size(), "XMAS");
		/* Rising diagonals */
		partOne += findStr(input, i, 0, 1, 1, input.size(), "XMAS");
		partOne += findStr(input, 0, i + 1, 1, 1, input.size(), "XMAS");
		/* Falling diagonals */
		partOne += findStr(input, 0, i, 1, -1, input.size(), "XMAS");
		partOne += findStr(input, i + 1, input.size() - 1, 1, -1, input.size(), "XMAS");
	}

	std::cout << "PartOne: " << partOne << " ?= 2500" << std::endl;
	std::cout << "PartTwo: " << partTwo << " ?= 1933" << std::endl;
	return 0;
}
