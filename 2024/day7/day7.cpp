#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

long int recursive_search(bool part2, std::vector<long int> &eq, long int target,
							long int curr, int depth = 2) {
	long int opnd = eq[depth];
	if (depth > eq.size() - 1)
		return curr;
	if (curr < 0 || curr + opnd < 0 || curr * opnd < 0) {
		std::cout << "Overflow" << std::endl;
		for (auto &e: eq)
			std::cout << e << " ";
		std::cout << std::endl << curr << std::endl << opnd << std::endl;
		exit(-1);
	}
	if (recursive_search(part2, eq, target, curr + opnd, depth + 1) == target)
		return target;
	if (recursive_search(part2, eq, target, curr * opnd, depth + 1) == target)
		return target;

	if (!part2)
		return 0;

	long int ccated = std::stoll(std::to_string(curr) + std::to_string(opnd)); // could be cooked
	if (recursive_search(part2, eq, target, ccated, depth + 1) == target)
		return target;
	
	return 0;
}

int main (int argc, char **argv) {
	long int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");

	for (std::string line; std::getline(ifs, line);) {
		std::vector<long int> equation;
		long int number;
		std::stringstream ss(line);
		ss >> number;
		ss.ignore(line.length(), ':');
		equation.push_back(number);
		while (ss >> number)
			equation.push_back(number);
/*
		std::string test;
		test += std::to_string(equation[0]) + ":";
		for (int i = 1; i < equation.size(); i++)
			test += " " + std::to_string(equation[i]);
		if (test != line) {
			std::cout << "Failed to parse input: \"" << line
				<< "\" != \"" << test << "\"" << std::endl;
			exit(-1);
		}
*/
		if (recursive_search(false, equation, equation[0], equation[1]))
			partOne += equation[0];
		if (recursive_search(true, equation, equation[0], equation[1]))
			partTwo += equation[0];
	}
	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;
}
