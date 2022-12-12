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

class Emulation {
	int cpuCycle;
	int X;
	int sum1;
	std::vector<std::string> program;

	void cycle(void) {
		cpuCycle++;

		switch (cpuCycle) {
		case 20:
		case 60:
		case 100:
		case 140:
		case 180:
		case 220:
			sum1 += X * cpuCycle;
//			std::cout << cpuCycle << ": strength = " << X * cpuCycle
//				<< std::endl;
			break;
		default:
			break;
		}

	};
	char pixel(int off, int val) {
		if ((off >= (val - 1)) && (off <= (val + 1)))
			return '#';
		return '.';
	};
	std::string emulate(void) {
		std::string ret;
		int cy = 1;
		int x = 1;
		int pc = 0;
		int exec = 0;
		int off = 0;

		while (true) {
			std::string instr = program[pc];
			std::stringstream ss(instr);
			std::string first;
			ss >> first;
			off %= 40;
			if (first == "addx") {
				std::string second;
				ss >> second;

				ret.push_back(pixel(off++, x));
				off %= 40;
				ret.push_back(pixel(off++, x));
				cy += 2;
				x += std::stoi(second);
			} else if (first == "noop") {
				ret.push_back(pixel(off++, x));
				cy ++;
			}
			if (++pc > program.size())
				break;
		}

		return ret;
	};
public:
	Emulation(std::string filename) : cpuCycle(0), X(1), sum1(0) {
		Input in(filename.size() ? filename : "input");
		while(in.nextLine()) {
			std::string inst, value, ln = in.getLine();
			int val = 0;
			std::stringstream ss(ln);
			ss >> inst;
			if (inst == "addx") {
				cycle();
				cycle();
				ss >> value;
				X += std::stoi(value);
			} else if (inst == "noop") {
				cycle();
			}
			program.push_back(ln);
		}
	};
	int partOne(void) {
		return sum1;
	};
	void partTwo(void) {
		std::string output = emulate();
		for (int i = 0; i < output.size(); i++) {
			if (!(i % 40) && i > 0)
				std::cout << std::endl;
			std::cout << output[i];
		}
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Emulation em(param);
	std::cout << "Part One: " << em.partOne() << std::endl;
	std::cout << "Part Two:" << std::endl;
	em.partTwo();
	return 0;
}
