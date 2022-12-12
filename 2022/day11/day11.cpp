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
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
	int64_t nextByte(void) { ifs.get(lastByte); return !ifs.eof(); };
	char getByte(void) { return lastByte; };
};

class Monkey {
	int64_t nr;
	std::vector<int64_t> items;
	std::vector<std::string> params;
	std::string opOperand1;
	std::string opOperand2;

	std::vector<int64_t> itemsCaught;
	char opOp;

	int64_t divisor;
	int64_t trueThrowNr;
	int64_t falseThrowNr;

	int64_t caught;
	int64_t thrown;
	int64_t inspected;
	int64_t divider;

	int64_t normal;

	void parse(void) {
		for (int64_t i = 0; i < params.size(); i++) {
			std::string token;
			std::stringstream ss(params[i]);
			ss >> token;

			if (token.find("Monkey") != std::string::npos) {
				ss >> token;
				token = token.substr(0, token.size() - 1);
				nr = std::stoi(token);
			} else if (token.find("Starting") != std::string::npos) {
				ss >> token; // "items:"
				do {
					std::string tmp;
					ss >> token;
					if (token[token.size() - 1] == ',')
						tmp = token.substr(0, token.size() - 1);
					else
						tmp = token;
					items.push_back(std::stoi(tmp));
				} while (token[token.size() - 1] == ',');
			} else if (token.find("Operation") != std::string::npos) {
				ss >> token; // "new"
				ss >> token; // "="
				ss >> token; // "old"
				opOperand1 = token;
				ss >> token; // "+" / "*"
				opOp = token[0];
				ss >> token; // num
				opOperand2 = token;
			} else if (token.find("Test") != std::string::npos) {
				ss >> token;
				ss >> token;
				ss >> token;
				divisor = std::stoi(token);
			} else if (token.find("If") != std::string::npos) {
				ss >> token;
				if (token == "true:") {
					ss >> token; // throw
					ss >> token; // to
					ss >> token; // monkey
					ss >> token;
					trueThrowNr = std::stoi(token);
				}
				if (token == "false:") {
					ss >> token; // throw
					ss >> token; // to
					ss >> token; // monkey
					ss >> token;
					falseThrowNr = std::stoi(token);
				}
			}
		}
	}
public:
	Monkey(std::vector<std::string> setup, int64_t c) : params(setup),
			divider(c), thrown(0), caught(0), inspected(0) {
//		for (int64_t i = 0; i < params.size(); i++)
//			std::cout << params[i] << std::endl;
		parse();
	};
	void monkeyCatch(int64_t item) {
		caught++;
		items.push_back(item);
	};
	void inspect(std::vector<Monkey> *monkes) {
//		std::cout << "Monkey " << nr << std::endl;
		for (auto it = items.begin(); it != items.end();) {
			int64_t operand1, operand2;
//			std::cout << *it;
			if (opOperand1 == "old")
				operand1 = *it;
			else
				operand1 = std::stoi(opOperand1);

			if (opOperand2 == "old")
				operand2 = *it;
			else
				operand2 = std::stoi(opOperand2);

			*it = operand1;
			if (opOp == '+')
				*it += operand2;
			else if (opOp == '*')
				*it *= operand2;
//			std::cout << "->" << *it;

			inspected ++;
			*it /= divider;
//			std::cout << "->" << *it << " thrown to ";

			if (!(*it % divisor)) {
//				std::cout << trueThrowNr;
				(*monkes)[trueThrowNr].monkeyCatch(*it % normal);
			} else {
//				std::cout << falseThrowNr;
				(*monkes)[falseThrowNr].monkeyCatch(*it % normal);
			}
//			std::cout << std::endl;
			it = items.erase(it);
		}
	};
	std::vector<int64_t> getItems(void) {
		return items;
	};
	int64_t getInspected(void) {
		return inspected;
	};
	int64_t getNr(void) {
		return nr;
	};
	int64_t getDivisor(void) {
		return divisor;
	};
	void setNormal(int64_t n) {
		normal = n;
	};
};

class Jungle {
	std::vector<Monkey> monkeys;
	void round(void) {
		for (auto it = monkeys.begin(); it != monkeys.end(); it++)
			it->inspect(&monkeys);
	};
	void printMonkeys(void) {
		std::vector<int64_t> inspected;
		for (auto it = monkeys.begin(); it != monkeys.end(); it++) {
			std::vector<int64_t> items = it->getItems();
			std::cout << it->getNr() << ":" << std::endl;
			std::cout << "inspected " << it->getInspected() <<
				" items" << std::endl;
			std::cout << "Holding: ";
			for (auto jt = items.begin(); jt != items.end(); jt++)
				std::cout << *jt << ", ";
			std::cout << std::endl;
		}
	};
	int64_t monkeyBusiness(int64_t rounds) {
		std::vector<int64_t> inspected;
		for (int64_t i = 0; i < rounds; i++)
			round();
//		printMonkeys();
		for (auto it = monkeys.begin(); it != monkeys.end(); it++)
			inspected.push_back(it->getInspected());

		std::sort(inspected.begin(), inspected.end(), std::greater<int64_t>());
		return inspected[0] * inspected[1];
	};
public:
	Jungle(std::string filename, int64_t divider) {
		Input in = Input(filename.size() > 0 ? filename : "input");
		int64_t normal = 1;
		while (in.nextLine()) {
			std::vector<std::string> ln;
			for (int64_t i = 0; i < 7; i++) {
				if (i > 0)
					in.nextLine();
				ln.push_back(in.getLine());
			}
			Monkey monke(ln, divider);
			normal *= monke.getDivisor();
			monkeys.push_back(monke);
		}
		for (auto it = monkeys.begin(); it != monkeys.end(); it++)
			it->setNormal(normal);
	};
	int64_t partOne(void) {
		return monkeyBusiness(20);
	};
	int64_t partTwo(void) {
		return monkeyBusiness(10000);
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);
	Jungle jungle(param, 3);
	std::cout << "Part One: " << jungle.partOne() << std::endl;

	jungle = Jungle(param, 1);
	std::cout << "Part Two: " << jungle.partTwo() << std::endl;


	return 0;
}
