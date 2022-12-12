#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <list>
#include <array>

class Input {
	std::ifstream ifs;
	std::string lastLine;
	char lastByte;
public:
	Input(std::string filename) : lastByte(0), lastLine("")
				{ ifs = std::ifstream(filename); };
	int nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
	int nextByte(void) { ifs.get(lastByte); return !ifs.eof(); };
	char getByte(void) { return lastByte; };
};

class Simulation {
	class Position {
	public:
		int x;
		int y;
		bool operator== (Position b) { return (x == b.x && y == b.y); }
		Position() { x = y = 0; };
		Position(int xPos, int yPos) { x = xPos; y = yPos; };
	};
	std::array<Position, 10> tail;
	std::array<std::vector<Position>, 10> positions;
	int minX, maxX, minY, maxY;
	void step(std::string dir) {
		int dirX, dirY;
		if (dir == "U") {
			dirX = 0;
			dirY = 1;
		} else if (dir == "D") {
			dirX = 0;
			dirY = -1;
		} else if (dir == "L") {
			dirX = -1;
			dirY = 0;
		} else if (dir == "R") {
			dirX = 1;
			dirY = 0;
		}

		tail[0].x += dirX;
		tail[0].y += dirY;
		positions[0].push_back(tail[0]);

		for (int i = 1; i < tail.size(); i++) {
			Position curr = tail[i];
			int distX = tail[i - 1].x - tail[i].x;
			int distY = tail[i - 1].y - tail[i].y;
			int dist = sqrt((distX * distX) + (distY * distY));
			if (tail[i - 1].x != tail[i].x &&
					tail[i - 1].y != tail[i].y && dist > 1) {
				tail[i].x += abs(distX) / distX;
				tail[i].y += abs(distY) / distY;
			} else if (abs(distX) > 1) {
				tail[i].x += abs(distX) / distX;
			} else if (abs(distY) > 1) {
				tail[i].y += abs(distY) / distY;
			}
			positions[i].push_back(tail[i]);
		}

//		std::cout << "H(" << tail[0].x << "," << tail[0].y << ");";
//		for (int i = 1; i < tail.size(); i++)
//			std::cout << "T["<< i <<"](" << tail[i].x << "," << tail[i].y << ")";
//		std::cout << std::endl;
	}
	void move(std::string dir, int dist) {
//		std::cout << dir << " " << dist << std::endl;
		for (int i = 0; i < dist; i++)
			step(dir);
	};
	void printSpace(int minX, int maxX, int minY, int maxY) {
		for (int y = maxY; y >= minY; y--) {
			for (int x = minX; x <= maxX; x++) {
				int count = 0;
				int n = 0;
				for (int i = 0; i < tail.size(); i++) {
					if (tail[i].x == x && tail[i].y == y) {
						if (!count)
							n = i;
						count ++;
					}
				}
				if (!count)
					std::cout << ".";
				else if (n == 0)
					std::cout << "H";
				else if (count >= 0)
					std::cout << n;
			}
			std::cout << std::endl;
		}

	};
public:
	Simulation(std::string filename) {
		Input in(filename.size() ? filename : "input");
		while(in.nextLine()) {
			std::string dir, steps, ln = in.getLine();
			std::stringstream ss(ln);
			ss >> dir;
			ss >> steps;
			move(dir, std::stoi(steps));
		}
	};
	int uniquePos(int knot) {
		std::vector<Position> tmp;
		for (auto it = positions[knot].begin(); it != positions[knot].end(); it++) {
			bool unique = true;
			for (auto jt = tmp.begin(); jt != tmp.end(); jt++) {
				if (*jt == *it) {
					unique = false;
					break;
				}
			}
			if (unique)
				tmp.push_back(*it);
		}
		return tmp.size();
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Simulation sim(param);
	std::cout << "Part One: " << sim.uniquePos(1) << std::endl;
	std::cout << "Part Two: " << sim.uniquePos(9) << std::endl;
	return 0;
}
