#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Parser {
	std::ifstream ifs;
	std::string ln;
public:
	Parser(std::string filename) { ifs = std::ifstream(filename); };
	int next(void) { return !!std::getline(ifs, ln); };
	std::string getLine(void) { return ln; };
};

int prioritySum(std::vector<char> items) {
#define PRIORITY(x)	(((x) > 'Z' ? (x) - 'a': \
			(x) - 'A' + ('z' - 'a' + 1)) + 1)
	int sum = 0;
	for (int i = 0; i < items.size(); i++) {
		if (items[i])
			sum += PRIORITY(items[i]);
	}
	return sum;
}

char findOverlapping(std::string first, std::string second) {
	char ret = 0;

	for (int i = 0; i < second.size(); i++) {
		if (first.find(second[i]) != std::string::npos)
			return second[i];
	}

	return ret;
}

int partOne(void) {
	int sum = 0;
	Parser par("input");
	std::vector<char> duplicates;

	while(par.next()) {
		std::string ln = par.getLine();
		int len = ln.size();

		std::string first = std::string(ln, 0, len / 2);
		std::string second = std::string(ln, len / 2, len / 2);
		duplicates.push_back(findOverlapping(first, second));
	}
	return prioritySum(duplicates);
}

char findBadge(std::string l1, std::string l2, std::string l3) {
	char ret = 0;
	std::cout << l1 << " " << l2 << " " << l3 << std::endl;
	for (int i = 0; i < l1.size(); i++) {
		if (l2.find(l1[i]) != std::string::npos &&
				l3.find(l1[i]) != std::string::npos)
			return l1[i];
	}
	return ret;
}

int partTwo(void) {
	int sum = 0;
	Parser par("input");
	std::vector<char> badges;

	while(par.next()) {
		std::string ln1, ln2, ln3;
		ln1 = par.getLine();
		if (par.next())
			ln2 = par.getLine();
		if (par.next())
			ln3 = par.getLine();

		badges.push_back(findBadge(ln1, ln2, ln3));
	}

	return prioritySum(badges);
}

int main() {
	std::cout << "Part One: " << partOne() << std::endl;
	std::cout << "Part Two: " << partTwo() << std::endl;

	return 0;
}
