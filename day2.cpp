#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int runMachine(vector<int> mem, int noun, int verb) {
	int pc = 0;
	mem[1] = noun;
	mem[2] = verb;
	while (true) {
		switch (mem[pc]) {
		case 99:
			return mem[0];
			break;
		case 1:
			mem[mem[pc + 3]] = mem[mem[pc + 1]] + mem[mem[pc + 2]];
			pc += 4;
			break;
		case 2:
			mem[mem[pc + 3]] = mem[mem[pc + 1]] * mem[mem[pc + 2]];
			pc += 4;
			break;
		default:
			cout << "Invalid instruction" << endl;
			exit(-1);
		}
	}
	return mem[0];
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "02.txt");
	vector<int> mem;	
	for (string s; getline(ifs, s, ','); mem.push_back(stoi(s)));

	cout << "Part One: " << runMachine(mem, 12, 1) << endl;
	for (int noun = 0, done = 0; noun <= 99 && !done; noun ++) {
		for (int verb = 0; verb <= 99 && !done; verb ++)
			if (done = runMachine(mem, noun, verb) == 19690720)
				cout << "Part Two: " << 100 * noun + verb << endl;
	}
	return 0;
}
