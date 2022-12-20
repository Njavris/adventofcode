#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h>

class Input {
	std::ifstream ifs;
	std::string lastLine;
public:
	Input(std::string filename) : lastLine("") { ifs = std::ifstream(filename); };
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
};

class BEZ {
	struct sensor {
		int sX, sY;
		int bX, bY;
		int d;
	};
	int partOneLine;
	int partOneAnswer;
	unsigned long long partTwoAnswer;
	std::vector<struct sensor> sensors;

	int mDist(int x1, int y1, int x2, int y2) { return std::abs(x1 - x2) + std::abs(y1 - y2); };

	struct range {
		int a, b;
		bool operator<(struct range &rhs) { return a < rhs.a; };
		bool operator==(struct range &rhs) { return (a == rhs.a && b == rhs.b); };
	};
	std::vector<struct range> lineRanges(int line) {
		std::vector<struct range> ranges;
		for (auto s: sensors) {
			int distToRow = std::abs(line - s.sY);
			if (distToRow < s.d) {
				int delta = std::abs(s.d - distToRow);
				struct range r;
				r.a = s.sX - delta;
				r.b = s.sX + delta;
				ranges.push_back(r);
			}
		}

		std::sort(ranges.begin(), ranges.end());

		std::vector<struct range>::iterator it = ranges.begin();
		while (it != ranges.end()) {
			std::vector<struct range>::iterator jt = it + 1;
			while (jt != ranges.end()) {
//				std::cout << "Compare " << it->a << "," << it->b <<
//					" with " << jt->a << "," << jt->b << std::endl;
				if ((it->a <= jt->a && it->b + 1 >= jt->a) ||
						(jt->a <= it->a && jt->b >= it->a)) {
					it->a = std::min(it->a, jt->a);
					it->b = std::max(it->b, jt->b);
					ranges.erase(jt);
//					std::cout << " Merging into " << it->a << "," <<
//						it->b << std::endl;
				} else {
					jt++;
				}
//				std::cout << std::endl;
			}
			it ++;
		}
//		std::cout << line << ": Merged ranges: ";
//		for (auto r: ranges)
//			std::cout << "[" << r.a << "," << r.b << "] ";
//		std::cout << std::endl;

		return ranges;
	};
	void doPartOne(void) {
		std::vector<struct range> ranges = lineRanges(partOneLine);
		for (int i = 0; i < ranges.size(); i++) {
			for (auto s: sensors) {
				if (s.sY == partOneLine && s.sX >= ranges[i].a && s.sX <= ranges[i].b) {
					if (s.sX == ranges[i].a) {
						ranges[i].a++;
					} else if (s.sX == ranges[i].b) {
						ranges[i].b--;
					} else {
						struct range rNew = { s.sX + 1, ranges[i].b };
						ranges[i].b = s.sX - 1;
						ranges.push_back(rNew);
					}
				}
				if (s.bY == partOneLine && s.bX >= ranges[i].a && s.bX <= ranges[i].b) {
					if (s.bX == ranges[i].a) {
						ranges[i].a++;
					} else if (s.bX == ranges[i].b) {
						ranges[i].b--;
					} else {
						struct range rNew = { s.bX + 1, ranges[i].b };
						ranges[i].b = s.bX - 1;
						ranges.push_back(rNew);
					}
				}
			}
		}
		partOneAnswer = 0;
//		std::cout << "Range lengths: ";
		for (auto r: ranges) {
			int d = r.b -r.a + 1;
//			std::cout << "[" << r.a << "," << r.b << "]=" << d << " ";
			partOneAnswer += d;
		}
//		std::cout << std::endl;
	};
	void doPartTwo(void) {
		for (int i = 0; i <= 4000000; i++) {
			std::vector<struct range> ranges = lineRanges(i);
			if (ranges.size() < 2)
				continue;
			for (int j = 0; j < ranges.size(); j++) {
				for (int k = 0; k < ranges.size(); k++) {
					if (j == k)
						continue;
					if ((std::abs(ranges[j].b - ranges[k].b) == 2)  ||
						(std::abs(ranges[j].a - ranges[k].b) == 2)) {
							int x;
							if (std::abs(ranges[j].a - ranges[k].b) == 2)
								x = std::min(ranges[j].a, ranges[k].b) + 1;
							else
								x = std::min(ranges[k].a, ranges[j].b) + 1;
							std::cout << x << "," << i << std::endl;
							partTwoAnswer = 4000000ull * x + i;
						return;
					}
				}
			}
		}
	};
public:
	BEZ(std::string filename, int lineNr) : partOneLine(lineNr) {
		Input in(filename.size() ? filename : "input");
		while (in.nextLine()) {
			struct sensor s;
			std::string line = in.getLine();
			sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
				&s.sX, &s.sY, &s.bX, &s.bY);
			s.d = mDist(s.sX, s.sY, s.bX, s.bY);
			std::cout << s.sX << "," << s.sY << " " <<
				s.bX << "," << s.bY << " " << s.d << std::endl;
			sensors.push_back(s);
		}
		doPartOne();
		std::cout << partOneAnswer << " ?= " << 4793062 << std::endl;
		doPartTwo();
	};
	int partOne(void) {
		return partOneAnswer;
	};
	unsigned long long partTwo(void) {
		return partTwoAnswer;
	};
};

int main(int argc, char **argv) {
	std::string param;
	int line;
	if (argc > 1)
		param = std::string(argv[1]);
	if (argc > 2)
		line = std::stoi(std::string(argv[2]));
	else
		line = 2000000;

	BEZ bez(param, line);
	std::cout << "Part One: " << bez.partOne() << std::endl;
	std::cout << "Part Two: " << bez.partTwo() << std::endl;
	return 0;
}
