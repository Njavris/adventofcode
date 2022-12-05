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

struct range {
	int start;
	int end;
};

struct pair {
	struct range elem[2];
};

struct pair parseRanges(std::string ln) {
	struct pair ret = { {{0, 0}, {0, 0}}};
	std::string worker[2];
	worker[0] = std::string(ln, 0, ln.find(','));
	worker[1] = std::string(ln, ln.find(',') + 1, ln.size());

	for (int i = 0; i < sizeof(ret.elem)/sizeof(range); i++) {
		std::string start = std::string(worker[i], 0,
			worker[i].find('-'));
		std::string end = std::string(worker[i],
			worker[i].find('-') + 1, worker[i].size());
		ret.elem[i].start = std::stoi(start);
		ret.elem[i].end = std::stoi(end);
	}

	return ret;
}

int partOne(struct pair ranges) {
#define RANGE_CHECK(a, b)	(((a).start >= (b).start) && ((a).end <= (b).end))
	if (RANGE_CHECK(ranges.elem[0], ranges.elem[1]) ||
			RANGE_CHECK(ranges.elem[1], ranges.elem[0]))
		return 1;
	return 0;
}

int partTwo(struct pair ranges) {
	int st0 = ranges.elem[0].start;
	int en0 = ranges.elem[0].end;
	int st1 = ranges.elem[1].start;
	int en1 = ranges.elem[1].end;
	if ((st0 >= st1 && st0 <= en1) || (st1 >= st0 && st1 <= en0) ||
			(en0 <= en1 && en0 >= st1) || (en1 <= en0 && en1 >= st0))
		return 1;

	return 0;
}

int main() {
	Parser par = Parser("input");
	int partOneSum = 0, partTwoSum = 0;

	while (par.next()) {
		std::string ln = par.getLine();
		struct pair ranges = parseRanges(ln);
//		std::cout << "[" << ranges.elem[0].start << ";" <<
//			ranges.elem[0].end << "] [" << ranges.elem[1].start <<
//			";" << ranges.elem[1].end << "]" << std::endl;
		partOneSum += partOne(ranges);
		partTwoSum += partTwo(ranges);
	}
	std::cout << "partOne: " << partOneSum << std::endl;
	std::cout << "partTwo: " << partTwoSum << std::endl;

	return 0;
}
