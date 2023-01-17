#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include "utils.h"

class MoMap {
	int64_t partOneVal;
	int64_t partTwoVal;
	std::string instr;
	int yMax = 0, xMax = 0;
	char *map;
	enum {
		eRight = 0,
		eDown = 1,
		eLeft = 2,
		eUp = 3,
	};
	struct coord {
		int x;
		int y;
		struct coord &operator+=(const struct coord &s) { x += s.x; y += s.y; return *this; };
		friend struct coord operator+(struct coord l, const struct coord &r) { l += r; return l; };
	};
	inline int cToIdx(const struct coord &c) { return c.x + c.y * xMax; };
	void walk(struct coord &pos, int dir, std::string steps) {
		int dx = 0, dy = 0, s;
		struct coord d = { 0, 0 };
		sscanf(steps.c_str(), "%d", &s);
		if (dir == eRight) d.x = 1;
		else if (dir == eDown) d.y = 1;
		else if (dir == eLeft) d.x = -1;
		else if (dir == eUp) d.y = -1;
		for (int i = 0; i < s; i++) {
			struct coord nc = pos + d;
			if (d.x) {
				if (nc.x >= xMax) {
					nc.x = 0;
				} else if (nc.x < 0) {
					nc.x = xMax - 1;
				} else if (!map[cToIdx(nc)]) {
					if (d.x > 0)
						nc.x = 0;
					if (d.x < 0)
						nc.x = xMax - 1;
				} else {
					goto wrapped;
				}
			} else if (d.y) {
				if (nc.y >= yMax) {
					nc.y = 0;
				} else if (nc.y < 0) {
					nc.y = yMax - 1;
				} else if (!map[cToIdx(nc)]) {
					if (d.y > 0)
						nc.y = 0;
					if (d.y < 0)
						nc.y = yMax - 1;
				} else {
					goto wrapped;
				}
			}
			for (int j = 0; (j < (d.x ? xMax : yMax)) && !map[cToIdx(nc)]; j++)
				nc += d;
wrapped:
			if (map[cToIdx(nc)] == '#')
				break;
			pos = nc;
		}
	};
	std::string getInstrToken(std::string instr, int *off) {
		int i = *off;
		int start = i;
		do {
			if (instr[i] >= '0' && instr[i] <= '9')
				continue;
			if (i == *off)
				i++;
			break;
		} while (instr[i++] != std::string::npos);
		*off = i;
		return instr.substr(start, i - start);
	};
	void doPartOne(void) {
		Timer timer = Timer();
		struct coord pos = { 0 };
		int dir = eRight;
		walk(pos, dir, "1");
		int off = 0;
		while (off < instr.size()) {
			std::string tkn = getInstrToken(instr, &off);
			if (tkn[0] == 'R')
				dir = (dir + 1) & 3;
			else if (tkn[0] == 'L')
				dir = (dir - 1) & 3;
			else
				walk(pos, dir, tkn);
		}
		partOneVal = (pos.y + 1) * 1000 + 4 * (pos.x + 1) + dir;
		if (partOneVal != 181128)
			std::cout << "Wrong!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part One: " << timer.getTimeStr() << std::endl;
	};
	void doPartTwo(void) {
		Timer timer = Timer();
		timer.stopTiming();
		std::cout << "Time taken by part Two: " << timer.getTimeStr() << std::endl;
	};
public:
	MoMap(std::string fileName) : partOneVal(0), partTwoVal(0) {
		Input in = Input(fileName.size() ? fileName : "input");
		std::vector<std::string> input;
		while (in.nextLine()) {
			std::string ln = in.getLine();
			if (!ln.size()) {
				in.nextLine();
				instr = in.getLine();
				continue;
			}
			input.push_back(ln);
			if (ln.size() > xMax)
				xMax = ln.size();
			yMax++;
		}
		map = new char[xMax * yMax];
		memset(map, 0, xMax * yMax);
		for (int y = 0; y < input.size(); y++)
			for (int x = 0; x < input[y].size(); x++)
				if (input[y][x] != ' ')
					map[cToIdx({x,y})] = input[y][x];

		doPartOne();
		doPartTwo();
	};
	~MoMap(void) {
		delete map;
	}
	int64_t partOne(void) { return partOneVal; };
	int64_t partTwo(void) { return partTwoVal; };
};

int main(int argc, char **argv) {
	std::string p;
	if (argc > 1)
		p = std::string(argv[1]);
	MoMap m(p);
	std::cout << "Part One: " << m.partOne() << std::endl;
	    std::cout << "Part Two: " << m.partTwo() << std::endl;
	return 0;
}
