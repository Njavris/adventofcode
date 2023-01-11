#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>

#include "utils.h"

class Riddle {
	int64_t partOneVal;
	int64_t partTwoVal;
	std::map<std::string, std::string> riddles;
public:
	bool getNum(std::string str, int64_t *num) {
		char *s = NULL;
		*num = strtol(str.c_str(), &s, 10);
		return !(*s);
	}
	int64_t evalPt1(std::string expr) {
		std::stringstream ss(expr);
		std::string first, second, third;
		int64_t ret = 0;
		int64_t operand0;
		int64_t operand1;

		ss >> first;
		if (getNum(first, &operand0))
			ret = operand0;
		else
			ret = evalPt1(riddles[first]);
		if (ss >> second && ss >> third) {
			int64_t tmp;
			if (getNum(third, &tmp))
				operand1 = tmp;
			else
				operand1 = evalPt1(riddles[third]);
			if (second == "*") {
				ret *= operand1;
			} else if (second == "/") {
				ret /= operand1;
			} else if (second == "+") {
				ret += operand1;
			} else if (second == "-") {
				ret -= operand1;
			} else {
				std::cout << "something wrong with \"" << expr << "\"" << std::endl;
				exit(-1);
			}
		}
		return ret;
	}
	void doPartOne(void) {
		Timer timer = Timer();
		partOneVal = evalPt1("root");
		if (partOneVal != 331120084396440)
			std::cout << "WRONG!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part One: " << timer.getTimeStr() << std::endl;
	};
	bool isNum(std::string str) {
		char *s = NULL;
		strtol(str.c_str(), &s, 10);
		return !(*s);
	}
	std::string evalPt2(std::string expr) {
		std::stringstream ss(expr);
		std::string first, second, third;
		std::string ret;
		bool numbers = false;
		int64_t operand0;
		int64_t operand1;

		ss >> first;
		if (first == "humn") {
			ret += "humn";
		} else if (getNum(first, &operand0)) {
			numbers = true;
			ret += first;
		} else {
			ret += evalPt2(riddles[first]);
		}

		if (ss >> second && ss >> third) {
			if (second == "=") {
				ret += " = ";
			} else {
				ret += " " + second + " ";
			}
			if (third == "humn")
				ret += "humn";
			else if(getNum(third, &operand1)) {
				numbers = numbers && true;
				ret += third;
			} else {
				ret += evalPt2(riddles[third]);
			}
			if (numbers) {
				int64_t val = operand0;
				if (second == "*") val *= operand1;
				else if (second == "/") val /= operand1;
				else if (second == "+") val += operand1;
				else if (second == "-") val -= operand1;
				ret = std::to_string(val);
			} else {
				if (ret.find("humn") == std::string::npos)
					ret = evalPt2(ret);
				if (!isNum(ret))
					ret = "(" + ret + ")";
			}
		}
		return ret;
	};
	int64_t operate(int64_t rhs, std::string oper, int64_t val, bool right) {
		if (oper == "+") return rhs - val;
		else if (oper == "*") return rhs / val;
		else if (oper == "-" && right) return rhs + val;
		else if (oper == "-" && !right) return -rhs + val;
		else if (oper == "/" && right) return rhs * val;
		else if (oper == "/" && !right) return val / rhs;
		std::cout << "Something went wrong!" << std::endl;
		exit(-1);
	}
	int64_t solve(std::string lhs, int64_t rhs) {
		int depth = 0, start = 0, end;
		std::string sub, str = lhs.substr(1, lhs.size() - 2);
		std::string op0, op1, oper;
		end = str.size() - 1;
		if (str.find('(') != std::string::npos) {
			for (int i = 0; i < str.size(); i++) {
				if (str[i] == '(') {
					if (!depth) start = i;
					depth ++;
				} else if (str[i] == ')') {
					depth --;
					if (!depth) end = i + 1;
				}
			}
		} else if (str.find("humn") != std::string::npos) {
			start = str.find("humn");
			end = start + std::string("humn").size();
		}
		sub = str.substr(start, end - start);
		if (start) {
			op0 = str.substr(0, start);
			std::stringstream ss(op0);
			ss >> op0;
			ss >> oper;
			op1 = sub;
			rhs = operate(rhs, oper, std::stoll(op0), false);
		} else {
			op1 = str.substr(end, str.size() - end);
			std::stringstream ss(op1);
			ss >> oper;
			ss >> op1;
			op0 = sub;
			rhs = operate(rhs, oper, std::stoll(op1), true);
		}

		if (op0 == "humn" || op1 == "humn")
			return rhs;
		return solve(sub, rhs);
	};
	void doPartTwo(void) {
		std::string root, eq, lhs, rhs;
		Timer timer = Timer();
		root = riddles["root"];
		std::replace(begin(root), end(root), '*', '=');
		std::replace(begin(root), end(root), '/', '=');
		std::replace(begin(root), end(root), '+', '=');
		std::replace(begin(root), end(root), '-', '=');
		riddles["root"] = root;

		eq = evalPt2("root");
		eq = eq.substr(1, eq.size() - 2);
		lhs = eq.substr(0, eq.find(" = "));
		rhs = eq.substr(eq.find(" = ") + 3);
		partTwoVal = solve(lhs, std::stoll(rhs));
		if (partTwoVal != 3378273370680)
			std::cout << "WRONG!" << std::endl;

		timer.stopTiming();
		std::cout << "Time taken by part Two: " << timer.getTimeStr() << std::endl;
	};
	Riddle(std::string fileName) : partOneVal(0), partTwoVal(0) {
		Input in = Input(fileName.size() ? fileName : "input");
		while (in.nextLine()) {
			char mon[32] = { 0 };
			char expr[32] = { 0 };
			std::string ln = in.getLine();
//			std::cout << ln << std::endl;
			sscanf(ln.c_str(), "%[^: ]: %32c", mon, expr);
			riddles[std::string(mon)] = std::string(expr);
		}
		doPartOne();
		doPartTwo();
	};
	~Riddle(void) {
	}
	int64_t partOne(void) { return partOneVal; };
	int64_t partTwo(void) { return partTwoVal; };
};

int main(int argc, char **argv) {
	std::string p;
	if (argc > 1)
		p = std::string(argv[1]);
	Riddle r(p);
	std::cout << "Part One: " << r.partOne() << std::endl;
	std::cout << "Part Two: " << r.partTwo() << std::endl;
	return 0;
}
