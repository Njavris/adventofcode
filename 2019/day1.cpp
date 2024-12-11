#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "01.txt");

	for (int val; ifs >> val;) {
		partOne += (val / 3) - 2;
		while ((val = val / 3 - 2) >= 0) 
			partTwo += val;
	}

	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;
	return 0;
}
