#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>

#include "utils.h"

class Tetris {
	int partOneVal;
	int64_t partTwoVal;
	std::vector<uint32_t> shapeMasks;
	std::vector<int> shapeHeights;
	std::string input;

	void printShape(uint32_t shape) {
		std::cout << std::hex << shape << std::endl;
		for (int i = 0; i < sizeof(shape); i++) {
			uint8_t l = ((uint8_t *)&shape)[i];
			for (int b = 0; b < 7; b++) {
				int bit = (l >> b) & 1;
				std::cout << (bit ? '#' : '.');
			}
			std::cout << std::endl;
		}
	};
	bool collision(std::vector<uint8_t> &tunnel, uint32_t shape, int row, int hght) {
		if (row < 0)
			return true;
		for (int i = hght - 1; i >= 0 ; i--) {
			if (tunnel[row + (hght - 1) - i] & ((uint8_t *)&shape)[i])
				return true;
		}
		return false;
	};
	void printTunnel(std::vector<uint8_t> &tunnel) {
		for (int i = tunnel.size() - 1; i >= 0; i--) {
			uint8_t l = tunnel[i];
			std::cout << std::dec << i << " |";
			for (int b = 0; b < 7; b++) {
				int bit = (l >> b) & 1;
				std::cout << (bit ? '#' : '.');
			}
			std::cout << "|" << std::endl;
		}
		std::cout << "  +-------+" << std::endl;
	};
	int checkForPattern(std::vector<uint8_t> &tunnel, int a, int b) {
		int i = 0;
		while (tunnel[a + i] == tunnel[b + i] && (a + i) < b) {
			i ++;
		}
		return i;
	}
	int findPattern(std::vector<uint8_t> &tunnel, int *offs) {
		int best = 0;
		int bestOffs = *offs;
		for (int i = 0; i < (tunnel.size() - best); i++) {
			for (int j = i + 1; j < tunnel.size(); j++) {
				if (tunnel[i] == tunnel[j]) {
					int matched = 0;
					while (tunnel[i + matched] == tunnel[j + matched] &&
							(i + matched) < j)
						matched ++;
					if (matched > best) {
						best = matched;
						bestOffs = i;
						std::cout << "\u001b[2K" << "Most matched: " << std::dec <<
						best << " " << i << " " << j << std::endl << "\033[F";
					}
				}
			}
		}
		std::cout << std::endl;

		*offs = bestOffs;
		return best;
	};
	int doTunnel(std::vector<uint8_t> &tunnel, int rockLimit = 2022,
			bool lookForPattern = false, int *patternOff = NULL, int *patternLen = NULL, int *patternRocks = NULL) {
		int rows = 0;
		int in = 0;
		int currShape = 0;
		int rocks = 0;
		int patOff = 0;
		int patLen = 0;
		int patRocks = 0;

		tunnel.resize(0, 0);
		bool done = false;
		while (!done) {
			uint32_t shape = shapeMasks[currShape];
			int currTop = 0;
			for (int i = tunnel.size() - 1; i >= 0; i--) {
				if (tunnel[i]) {
					currTop = i + 1;
					break;
				}
			}
			int shapeHght = shapeHeights[currShape];

			tunnel.resize(currTop + shapeHght + 3, 0);
			for (int i = tunnel.size() - shapeHght; i >= 0; i--) {
				char dir = input[in];
				uint32_t tmp = shape;
				in = (in + 1) % input.size();

				if (dir == '<' && !(tmp & 0x01010101))
					tmp >>= 1;
				else if (dir == '>' && !(tmp & 0x40404040))
					tmp <<= 1;
				if (!collision(tunnel, tmp, i, shapeHght))
					shape = tmp;

				if (collision(tunnel, shape, i - 1, shapeHght)) {
					for (int j = 0; j < shapeHght; j++)
						tunnel[i + j] |= ((uint8_t *)&shape)[shapeHght - 1 - j];
					break;
				}
			}
			currShape = (currShape + 1) % shapeMasks.size();
			rocks ++;

			if (lookForPattern) {
				int currBest = 0;
				int currOffs = 0;
				for (int s = currOffs; s < tunnel.size() - currBest; s++) {
					for (int p = s + 1; p < tunnel.size(); p++) {
						int n = 0;
						if (tunnel[s] != tunnel[p])
							continue;
						while (tunnel[s + n] == tunnel[p + n] && (s + n) < p)
							n++;
						if (tunnel[s] != tunnel[p + n]) {
							p += n - 1;
							break;
						}
						if (n > currBest) {
							currOffs = s;
							currBest = n;
//							std::cout << "\u001b[2K" << "Most matched: " <<
//								std::dec << currBest << " " << s
//								<< " " << p << std::endl << "\033[F";
						}
					}
				}
				if (currBest > patLen) {
					patLen = currBest;
					patOff = currOffs;
					patRocks = rocks;
				}
			}
			if (rockLimit && rockLimit == rocks)
				break;
		}
		auto it = std::find_if(begin(tunnel), end(tunnel),
				[](uint8_t v) -> bool { return !v; });
			if (it != std::end(tunnel))
				rows = std::distance(begin(tunnel), it);
		if (lookForPattern && patternLen && patternOff) {
			*patternLen = patLen;
			*patternOff = patOff;
			*patternRocks = patRocks;
		}
		return rows;
	};
	void doPartOne(void) {
		Timer tim = Timer();
		std::vector<uint8_t> tunnel;
		partOneVal = doTunnel(tunnel);
//		printTunnel(tunnel);
		tim.stopTiming();
		std::cout << "Time for Part One: " << tim.getTimeStr() << std::endl;
	};
	void doPartTwo(void) {
		Timer tim = Timer();
		std::vector<uint8_t> tunnel;
		int64_t total = 1000000000000ull;
		int64_t startRocks = 0;
		int64_t startRows = 0;
		int64_t periodRocks = 0;
		int64_t periodRows = 0;
		int64_t answer;

		int rows, off, len0, len1, rocks0, rocks1;
		rows = doTunnel(tunnel, 5000, true, & off, &len0, &rocks0);
//		std::cout << "rows: " << rows << " off: " << off << " len: " << len0 <<
//				" rocks: " << rocks0 << std::endl;
		rows = doTunnel(tunnel, rocks0 * 2, true, & off, &len1, &rocks1);
//		std::cout << "rows: " << rows << " off: " << off << " len: " << len1 <<
//				" rocks: " << rocks1 << std::endl;
		startRows = (int64_t)off;
		startRocks = (int64_t)(rocks0 * 2 - rocks1);
		periodRocks = ((int64_t)(rocks0) - startRocks) / 2;
		periodRows = (int64_t)(len0);
//		std::cout << "startRows: " << startRows << " startRocks: " << startRocks << std::endl;
//		std::cout << "periodRows: " << periodRows << " periodRocks: " << periodRocks << std::endl;

		answer = ((total - startRocks) / periodRocks) * periodRows;
		int64_t tmp = ((total - startRocks) % periodRocks) + startRocks;
		answer += (int64_t)doTunnel(tunnel, (int)tmp);
////		int64_t correct = 1514285714288; // testInput
//		int64_t correct = 1532183908048; // mine
//		if (answer != correct)
//			std::cout << "Wrong! \n" << answer << "\n" << correct<< std::endl;
		partTwoVal = answer;
		tim.stopTiming();
		std::cout << "Time for Part Two: " << tim.getTimeStr() << std::endl;
	};
public:
	Tetris(std::string fileName, std::string shapesFile) {
		Input in = Input(fileName.size() ? fileName : "input");
		if (in.nextLine())
			input = in.getLine();

		in = Input(shapesFile);
		while (in.nextLine()) {
			int size = 0;

			uint32_t shape = 0;
			for (int i = sizeof(shape) - 1; i >= 0; i--) {
				std::string ln = in.getLine();
				if (!ln.size()) {
					break;
				} else if (ln.size() > 7) {
					std::cout << "Wrong shape size " << ln.size() << std::endl;
					exit(-1);
				}
				for (int b = 0; b < ln.size(); b++) {
					if (ln[b] == '#')
						((uint8_t *)(&shape))[sizeof(shape) - i - 1] |= 1 << (b + 2);
				}
				size++;
//				std::cout << ln << ": " << std::hex << shape << std::endl;
				in.nextLine();
			}
			shapeMasks.push_back(shape);
			shapeHeights.push_back(size);
		}
//		for (int i = 0; i < shapeMasks.size(); i++) {
//			std::cout << i << ":" << std::endl;
//			printShape(shapeMasks[i]);
//		}


//		std::cout << input << std::endl;
		doPartOne();
		doPartTwo();
	};
	int partOne(void) {
		return partOneVal;
	};
	int64_t partTwo(void) {
		return partTwoVal;
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Tetris tetris(param, "shapes");
	std::cout << "Part One: " << std::dec << tetris.partOne() << std::endl;
	std::cout << "Part Two: " << std::dec << tetris.partTwo() << std::endl;
	return 0;
}
