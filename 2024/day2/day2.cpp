#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

class ln : public std::string {};
std::istream &operator>>(std::istream &is, ln &l) { return std::getline(is, l); }

int main(int argc, char **argv) {
	std::ifstream ifs(argc == 2 ? argv[1] : "input");

	int partOne = 0, partTwo = 0;
	std::istream_iterator<ln> begin(ifs), end;
	std::for_each(begin, end, [&partTwo, &partOne] (ln l) {
		std::vector<int> report;
		std::stringstream ss(l);
		for (int tmp = 0; ss >> tmp;)
			report.push_back(tmp);

		for (int skip = -1; skip < (int)report.size(); skip ++) {
			int maxDist = 0, slope = 0;
			int start = skip == 0 ? 1 : 0;
			int prev = report[start];
			for (int i = start + 1; i < report.size(); i++)  {
				if (i == skip)
					continue;
				int dist = report[i] - prev, sl = (dist > 0) - (dist < 0);
				if (!slope)
					slope = sl;
				if (!dist || (slope != sl) || ((sl * dist) > 3))
					goto next;
				prev = report[i];
			}
			if (skip == -1)
				partOne++;
			partTwo ++;
			return;
next:
			;
		}
	});
	std::cout << "Part One: " << partOne << std::endl;
	std::cout << "Part Two: " << partTwo << std::endl;
	return 0;
}
