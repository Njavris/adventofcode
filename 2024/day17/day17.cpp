#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;


unsigned long runMachine(unsigned long A, vector<int> prgrm, int &cnt) {
	unsigned long ret = 0;
	unsigned long regs[3] = { 0 };
	regs[0] = A;
//	for (int i = 0; i < regs.size(); i++) {
//		cout << "Register  " << (char)(i + 'A') << ": " << regs[i] << endl;
//	}
//	cout << endl << "Program:";
//	for (auto &i: prgrm)
//		cout << i << ",";
//	cout << endl;
	int pc = 0;
	cnt = 0;
	while (true) {
		if (pc >= prgrm.size())
			break;
		int instr = prgrm[pc];
		unsigned long op = prgrm[++pc];
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
			ret |= combo;
			ret <<= 3;
			cnt ++;
			break;
		case 6: // bdv
			regs[1] = regs[0] >> combo;
			break;
		case 7: // cdv
			regs[2] = regs[0] >> combo;
			break;
		}
		pc ++;
	}
	return ret;
}

string solveP1(vector<int> regs, vector<int> prgrm) {
	string ret;
	int cnt;
	unsigned long out = runMachine(regs[0], prgrm, cnt);
	while (out) {
		if (ret.size())
			ret.insert(0, 1, ',');
		ret.insert(0, 1, '0' +(char)(out & 7));
		out >>= 3;
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

void recursive(vector<int> &prgrm, unsigned long tgt, unsigned long &best, int size, unsigned long a = 0, int idx = 2) {
	unsigned long mask = (1ull << (3 * idx)) - 1;
	unsigned long v = tgt & mask;
	unsigned long c = (a << 3);
	int bits = 3 + 1;
	if (idx == 2)
		bits = (3 * (idx + 1)) + 7 + 1;

	for (unsigned long t = 0; t < (1 << bits); t++) {
		unsigned long tmp = c | t;
		int cnt;
		unsigned long res = runMachine(tmp, prgrm, cnt);
//		if (idx > 2)
//		cout << idx << " " << t << " " << res << " " << v << " " << cnt << endl;
		
		if (tgt == res) {
			if (!best || best > tmp)
				best = tmp;
//			cout << "tgt " << tgt << " " << tmp << endl;
			return;
		}
		if (res == v) {
			recursive(prgrm, tgt, best, size, tmp, idx + 1);
//			cout << t << " " << res << " " << v  << " " << tmp << endl;
		}
	}
};

int solveP2(vector<int> prgrm) {
	cout << oct;
	cout << endl;
	int cnt;
	unsigned long expect = 0;
	for (auto &p: prgrm) {
		expect <<= 3;
		expect |= p & 7;
	}
	disass(prgrm);
	cnt = prgrm.size() - 1;

	unsigned long  v = 44374556; 
	v = 34081368; 
	runMachine(v, prgrm, cnt);
	while (v != 0) {
		cout << "with " << v << " got " << runMachine(v, prgrm, cnt) << " " << cnt << endl;
		v >>= 3;
	}
	expect = 01503730310;

	cout << "Looking for : " << expect << endl;

	unsigned long result = 0;
	recursive(prgrm, expect, result, cnt);
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

	cout << "Part One: " << solveP1(regs, prgrm) << endl;
	cout << "Part Two: " << solveP2(prgrm) << endl;

	return 0;
}
