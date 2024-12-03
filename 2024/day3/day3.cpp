#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <algorithm>


class ln : public std::string {};
std::istream &operator>>(std::istream &is, ln &l) { return std::getline(is, l); }

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");

	std::istream_iterator<ln> begin(ifs), end;
	std::for_each(begin, end, [&partTwo, &partOne] (ln l) {
		static bool skip = false;
		std::regex rex(R"(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))");
		std::smatch matches;
		std::string::const_iterator start(l.cbegin());
		while (std::regex_search(start, l.cend(), matches, rex)) {
			std::string tmp(matches[0]);
			if (tmp != "do()" && tmp != "don't()") {
				int n1, n2;
				sscanf(tmp.c_str(), "mul(%d,%d)", &n1, &n2);
				partOne += n1 * n2;
				if (!skip)
					partTwo += n1 * n2;
			} else if (tmp == "do()") {
				skip = false;
			} else if (tmp == "don't()") {
				skip = true;
			}
			start = matches.suffix().first;
		}
	});
	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;
	return 0;
}
