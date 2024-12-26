#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

unsigned long runMachine(unsigned long A, vector<int> prgrm, int &cnt) {
	unsigned long ret = 0;
	unsigned long regs[3] = { 0 };
	regs[0] = A;
	int pc = 0;
	cnt = 0;
	while (true) {
		if (pc >= prgrm.size())
			break;
		int instr = prgrm[pc++];
		unsigned long op = prgrm[pc++];
		unsigned long combo = (op & 4) ? regs[op & 3] : op;
		combo &= 7;
		switch(instr) {
		case 0: // adv
			regs[0] >>= combo;
			break;
		case 1: // bxl
			regs[1] ^= op;
			break;
		case 2: // bst
			regs[1] = combo;
			break;
		case 3: // jnz
			if (regs[0]) {
				pc = op;
				continue;
			}
			break;
		case 4: // bxc
			regs[1] ^= regs[2];
			break;
		case 5: // out
			ret <<= 3;
			ret |= combo % 8;
			cnt ++;
			break;
		case 6: // bdv
			regs[1] = regs[0] >> combo;
			break;
		case 7: // cdv
			regs[2] = regs[0] >> combo;
			break;
		}
	}
	return ret;
}

string solveP1(int a, vector<int> prgrm) {
	string ret;
	int cnt;
	for (unsigned long out = runMachine(a, prgrm, cnt),
			i = 0; out; out >>= 3, i++) {
		if (i)
			ret.insert(0, 1, ',');
		ret.insert(0, 1, '0' + (char)(out & 7));
	}
	return ret;
}

void disass(vector<int> prgrm) {
	int pc = 0;
	while (true) {
		if (pc >= prgrm.size())
			break;
		int instr = prgrm[pc];
		int op = prgrm[pc + 1];
		string combo;
		if (op & 4) {
			combo = "(";
			combo.push_back((char)('A' + op - 4));
			combo += " & 7)";
		} else {
			combo = to_string(op & 7) + "      ";
		}
		cout << pc << ":\t" << instr << " " << op << "\t";
		switch(instr) {
		case 0:
			cout << "adv " << combo << "\t" << ";; A >>= " << combo << endl;
			break;
		case 1:
			cout << "bxl " << op << "      \t;; B = B ^ " << op << endl;
			break;
		case 2:
			cout << "bst " << combo << "\t;; B = " << combo << endl;
			break;
		case 3:
			cout << "jnz " << op << "      \t;; if (A) goto " << op << endl;
			break;
		case 4:
			cout << "bxc        " << "\t;; B = B ^ C" << endl;
			break;
		case 5:
			cout << "out " << combo << "\t;; print " << combo << endl;
			break;
		case 6:
			cout << "bdv " << combo << "\t;; B = A >> " << combo << endl;
			break;
		case 7:
			cout << "cdv " << combo << "\t;; C = A >> " << combo << endl;
			break;
		default:
			return;
		}
		pc += 2;
	}
}

void recursive(vector<int> &prgrm, unsigned long tgt, unsigned long &bst, unsigned long a = 0, int idx = 2) {
	unsigned long msk = (1ull << (3 * idx)) - 1;
	unsigned long v = tgt & msk;
	unsigned long c = (a << 3);
	int cnt, bits = (idx == 2) ? (3 * idx + 1) + 7 + 1 : 3 + 1;

	for (unsigned long t = 0; t < (1 << bits); t++) {
		unsigned long tst = c | t;
		unsigned long res = runMachine(tst, prgrm, cnt);

		if (tgt == res) {
			if (!bst || bst > tst)
				bst = tst;
			return;
		}
		if (res == v)
			recursive(prgrm, tgt, bst, tst, idx + 1);
	}
};

unsigned long solveP2(vector<int> prgrm) {
	int cnt;
	unsigned long result = 0, expect = 0;
	for (auto &p: prgrm) {
		expect <<= 3;
		expect |= p & 7;
	}
	cnt = prgrm.size() - 1;
//	disass(prgrm);
	recursive(prgrm, expect, result);
	return result;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<int> prgrm, regs(3, 0);

	for (string str; getline(ifs, str) && str.size(); ) {
		char c;
		int val;
		sscanf(str.c_str(), "Register %c: %d", &c, &val);
		regs[c - 'A'] = val;
	}
	string tmp;
	getline(ifs, tmp, ' ');
	while (getline(ifs, tmp, ','))
		prgrm.push_back(stoi(tmp));

	cout << "Part One: " << solveP1(regs[0], prgrm) << endl;
	cout << "Part Two: " << solveP2(prgrm) << endl;

	return 0;
}
