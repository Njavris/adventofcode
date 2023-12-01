#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
	int result = 0;
	if (argc != 2)
		return -1;

	bool part_two = true;

	std::string sdigits[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	std::string line;
	std::ifstream ifs(argv[1]);
	while (std::getline(ifs, line)) {
		int off = 0, first = -1, last;
		while (off < line.length()) {
			int digit = -1;
			if (line[off] >= '0' && line[off] <= '9') {
				digit = line[off] - '0';
			} else if (part_two) {
				for (int i = 0; i < sizeof(sdigits) / sizeof(sdigits[0]); i++) {
					bool found = true;
					if (sdigits[i].length() > (line.length() - off))
						continue;
					for (int j = 0; found && j < sdigits[i].length(); j++)
						found = (line[off + j] == sdigits[i][j]);
					if (found) {
						digit = i + 1;
						off += sdigits[i].length() - 2;
						break;
					}
				}
			}
			if (digit > 0) {
				last = digit;
				if (first < 0)
					first = digit;
			}
			off ++;
		}
		result += first * 10 + last;
	}
	std::cout << result << std::endl;
	return 0;
}
