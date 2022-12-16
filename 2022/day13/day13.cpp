#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

class Csv {
	int value;
	bool empty;
	bool list;
	std::string init;
	int level;
public:
	std::vector<Csv> children;
	Csv(std::string val, int lvl = 0) : empty(false), level(lvl),
			init(val), value(0), list(false) {
		int depth = 0;
		int prev = 1;
//		std::cout << "Csv(" << val << ")" << std::endl;
		if (val[0] == '[')
			list = true;
		for (int i = 0; i < val.size(); i++) {
			if (val[i] == '[') {
				depth++;
				continue;
			}
			if (depth == 1) {
				if (val[i] == ',' || val[i] == ']') {
					std::string child = val.substr(prev, i - prev);
					if (child.size())
						children.push_back(Csv(child, level + 1));
					prev = i + 1;
				}
			}
			if (val[i] == ']') {
				depth--;
				continue;
			}
		}
		if (!list && val.size())
			value = std::stoi(val);
	};
	bool operator== (const Csv &rhs) {
		if (!list && !rhs.list) {
			return value == rhs.value;
		} else if (list && rhs.list) {
			if (children.size() != rhs.children.size())
				return false;
			for (int i = 0; i < children.size(); i++) {
				if (children[i] == rhs.children[i])
					continue;
				return false;
			}
			return true;
		} else if (!list && rhs.list) {
			std::string tmp = "[";
			if (!empty)
				tmp += std::to_string(value);
			tmp += "]";
			return Csv(tmp, level + 1) == rhs;
		} else if (list && !rhs.list) {
			std::string tmp = "[";
			if (!rhs.empty)
				tmp += std::to_string(rhs.value);
			tmp += "]";
			return *this == Csv(tmp, rhs.level + 1);
		}
		return false;
	};
	bool operator< (const Csv &rhs) {
//		std::string prefix;
//		for (int i = 0; i < level; i++)
//			prefix.push_back(' ');
//		std::cout << prefix << "- Compare " << init << "(" << level;
//		std::cout << ") vs " << rhs.init << "(" << rhs.level << ")";
//		std::cout << std::endl;

		if (!list && !rhs.list) {
			return value <= rhs.value;
		} else if (list && rhs.list) {
			for (int i = 0; i < std::max(children.size(), rhs.children.size()); i++) {
				if (children.size() > rhs.children.size() &&
						i == rhs.children.size()){
					return false;
				}
				if (children.size() < rhs.children.size() &&
						i == children.size()) {
					return true;
				}
				if (children[i] == rhs.children[i])
					continue;
				else
					return children[i] < rhs.children[i];
				return false;
			}
		} else if (!list && rhs.list) {
			std::string tmp = "[";
			if (!empty)
				tmp += std::to_string(value);
			tmp += "]";
			return Csv(tmp, level + 1) < rhs;
		} else if (list && !rhs.list) {
			std::string tmp = "[";
			if (!rhs.empty)
				tmp += std::to_string(rhs.value);
			tmp += "]";
			return *this < Csv(tmp, rhs.level + 1);
		}

		return false;
	};
	void print(int level = 0) {
		std::string prefix;
		for (int i = 0; i < level; i++)
			prefix.push_back('>');
		if (list) {
			std::cout << prefix << std::endl;
			for (auto c: children) {
				c.print(level + 1);
			}
		} else {
			std::cout << prefix << value << std::endl;
		}

	};
};

class PacketProcessor {
	int partOneSum;
	int partTwoMultiplication;
	std::vector<Csv> packets;
public:
	PacketProcessor(std::string filename) : partOneSum(0) {
		Input in(filename.size() > 0 ? filename : "input");
		int pair = 0;

		while (in.nextLine()) {
			bool correct;
			std::cout << "== Pair " << ++pair << " ==" << std::endl;
			std::string first = in.getLine();
			in.nextLine();
			std::string second = in.getLine();
			Csv lhs = Csv(first);
//			lhs.print();
			Csv rhs = Csv(second);
//			rhs.print();
			correct = lhs < rhs;
			if (correct)
				partOneSum += pair;
			std::cout << (correct ? "Correct" : "Wrong") << " order" << std::endl;
			in.nextLine();
			packets.push_back(first);
			packets.push_back(second);
		}

		Csv divider1 = Csv("[[2]]");
		Csv divider2 = Csv("[[6]]");

		packets.push_back(divider1);
		packets.push_back(divider2);

		std::sort(packets.begin(), packets.end());

		for (int i = 0; i < packets.size(); i++)
			if (packets[i] == divider1)
				partTwoMultiplication = i + 1;
		for (int i = 0; i < packets.size(); i++)
			if (packets[i] == divider2)
				partTwoMultiplication *= i + 1;
	};
	int partOne() {
		return partOneSum;
	};
	int partTwo() {
		return partTwoMultiplication;
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	PacketProcessor pp(param);
	std::cout << "Part One: " << pp.partOne() << std::endl;
	std::cout << "Part Two: " << pp.partTwo() << std::endl;
	return 0;
}
