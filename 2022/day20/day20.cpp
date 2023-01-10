#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include "utils.h"

class Decryption {
	int partOneVal;
	int64_t partTwoVal;
	struct elem {
		struct elem *p;
		struct elem *n;
		int64_t v;
		int id;
	};
	struct elem *head, *first;
	struct elem *zero;
	int sz;
	std::vector<int> input;
	std::vector<struct elem *> elems;

	std::string listStr(std::string delim) {
		std::string ret;
		struct elem *c = zero;
		do {
			ret += std::to_string(c->v) + delim;
			c = c->n;
		} while (c != zero);
		return ret;
	};
	int getN(struct elem *e) {
		struct elem *c = zero;
		for (int i = 0; i < sz; i++) {
			if (c == e)
				return i;
			c = c->n;
		}
		return -1;
	};
	void eval(int id) {
		struct elem *el = elems[id], *el1;
		int64_t v = el->v % (sz - 1);
		int s = v < 0 ? -v : v;
		if (!v) return; /* zero stays */
		el1 = (v > 0) ? el->n : el;
		/* unlink */
		el->n->p = el->p;
		el->p->n = el->n;
		for (int i = 0; i < s; i++) {
			if (v > 0) el1 = el1->n;
			else if (v < 0) el1 = el1->p;
		}
		/* link in new position */
		el->p = el1->p;
		el->p->n = el;
		el->n = el1;
		el->n->p = el;
	};
	int64_t getNthVal(int n) {
		struct elem *c = zero;
		for (int i = 0; i < n; i++) c = c->n;
		return c->v;
	};
	std::string getIdx(int v) {
		std::string ret = "(";
		struct elem *c = zero;
		for (int i = 0; i < sz; i++) {
			if (v == c->v) ret += std::to_string(i) + ";";
			c = c->n;
		}
		ret += ")";
		return ret;
	};
	void mix(int times) {
		for (int i = 0; i < times; i++) {
			for (int j = 0; j < sz; j++)
				eval(j);
		}
	};
public:
	void doPartOne(void) {
		Timer timer = Timer();
		mix(1);
		partOneVal = 0;
		partOneVal += getNthVal(1000);
		partOneVal += getNthVal(2000);
		partOneVal += getNthVal(3000);
//		if (partOneVal != 7153)
//			std::cout << "Wrong!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part One: " << timer.getTimeStr() << std::endl;
	};
	void doPartTwo(void) {
		Timer timer = Timer();
		int64_t key = 811589153;
		for (struct elem *e: elems)
			e->v *= key;
		mix(10);
		partTwoVal = 0;
		partTwoVal += getNthVal(1000);
		partTwoVal += getNthVal(2000);
		partTwoVal += getNthVal(3000);
//		if (partTwoVal != 6146976244822)
//			std::cout << "Wrong!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part Two: " << timer.getTimeStr() << std::endl;
	};
	Decryption(std::string fileName) : sz(0), head(NULL) {
		Input in = Input(fileName.size() ? fileName : "input");
		while (in.nextLine()) {
			std::string ln = in.getLine();
			input.push_back(std::stoi(ln));
//			std::cout << ln << std::endl;
			struct elem *e = new struct elem;
			elems.push_back(e);
			if (!head) {
				head = e;
				first = e;
			}
			e->n = head->n;
			e->p = head;
			e->p->n = e;
			e->n->p = e;
			e->v = std::stoi(ln);
			e->id = sz++;
			if (!e->v)
				zero = e;
			head = e;
		}
	};
	~Decryption(void) {
		for (struct elem *e: elems)
			delete e;
//		input.resize(0);
//		elems.resize(0);
	}
	int partOne(void) { return partOneVal; };
	int64_t partTwo(void) { return partTwoVal; };
};

int main(int argc, char **argv) {
	std::string p;
	if (argc > 1)
		p = std::string(argv[1]);
	Decryption d1(p);
	d1.doPartOne();
	std::cout << "Part One: " << d1.partOne() << std::endl;
	Decryption d2(p);
	d2.doPartTwo();
	std::cout << "Part Two: " << d2.partTwo() << std::endl;
	return 0;
}
