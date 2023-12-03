#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Part {
public:
	int number;
	int posX0;
	int posX1;
	int posY;
	Part(int number, int posX0, int posX1, int posY) : number(number), posX0(posX0), posX1(posX1), posY(posY) {};
};

class Symbol {
public:
	char sym;
	int posX;
	int posY;
	vector<Part> prts;
	Symbol(char sym, int posX, int posY) : sym(sym), posX(posX), posY(posY) {};
};

int main(int c, char **v) {
	if (c != 2)
		return -1;

	ifstream ifs(v[1]);
	vector<Part> parts;
	vector<Symbol> symbols;

	int y = 0;
	string line;
	while (getline(ifs, line)) {
		int off = -1;
		for (int x = 0; x < line.length(); x++) {
			if (line[x] >= '0' && line[x] <= '9') {
				if (off < 0)
					off = x;
				if (x == line.length() - 1)
					parts.push_back(Part(stoi(line.substr(off)), off, x, y));
			} else {
				if (off >= 0) {
					string snum = line.substr(off, x - off);
					parts.push_back(Part(stoi(snum), off, x - 1, y));
					off = -1;
				}
				if (line[x] != '.')
					symbols.push_back(Symbol(line[x], x, y));
			}
		}
		y ++;
	}

	int partOne = 0;
	int partTwo = 0;
	for (auto &s: symbols) {
		for (auto &p: parts) {
			if (p.posX1 < s.posX - 1 || p.posX0 > s.posX + 1)
				continue;
			if (s.posY <= p.posY + 1 && s.posY >= p.posY - 1) {
				s.prts.push_back(p);
				partOne += p.number;
			}
		}
		if (s.sym == '*' && s.prts.size() == 2)
			partTwo += s.prts[0].number * s.prts[1].number;
	}
	cout << "Part One: " << partOne << endl;
	cout << "Part two: " << partTwo << endl;
	return 0;
}
