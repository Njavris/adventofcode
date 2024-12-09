#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Desc {
public:
	int fid;
	int ln;
	Desc() : fid(-1), ln(0) {};
	Desc(int f, int l) : fid(f), ln(l) {};
	string str() { return to_string(fid) + "," + to_string(ln); }
};

int main(int argc, char **argv) {
	long int partOne = 0, partTwo = 0;
	ifstream ifs(argc == 2 ? argv[1] : "input");
	string input;
	getline(ifs, input);
	vector<Desc> descsP1, descsP2;

	for (int i = 0, fid = 0; i < input.size(); i++, fid+=!(i%2))
		descsP1.push_back(Desc(!(i%2) ? fid : -1, input[i] - '0'));

	descsP2 = descsP1;

	for (int i = 0, idx = 0, ni = descsP1.size() - 1; i < descsP1.size(); i++) {
		Desc &d = descsP1[i];
		if (d.fid == -1) {
			while (d.ln > 0 && ni > i) {
				auto &b = descsP1[ni];
				partOne += b.fid * idx++;
				d.ln --;
				if (!--b.ln) {
					b.fid = -1;
					while (descsP1[ni].fid == -1 && ni > i) ni--;
				}
			}
		} else {
			for (; d.ln > 0 ; d.ln--)
				partOne += d.fid * idx++;
		}
		
	}

	for (int i = descsP2.size() -1; i >= 0; i--) {
		auto &d = descsP2[i];
		if (d.fid == -1)
			continue;
		for (int j = 0; j < i; j++) {
			auto &b = descsP2[j];
			if (b.fid != -1)
				continue;
			if (b.ln >= d.ln) {
				int ln = d.ln, fid = d.fid;
				d.fid = -1;
				b.ln -= ln;
				descsP2.insert(begin(descsP2) + j, Desc(fid, ln));
				i++;
				break;
			}
		}
	}

	for (int i = 0, idx = 0; i < descsP2.size(); i++) {
		auto &d = descsP2[i];
		if (d.fid == -1) {
			idx += d.ln;
		} else {
			for (int j = 0; j < d.ln; j++)
				partTwo += d.fid * idx++;
		} 
	}

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;

	return 0;
}
