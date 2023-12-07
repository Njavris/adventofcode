#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class Hand {
public:
	string hand;
	int bid;
	enum eType {
		eHigh = 0,
		eOnePair,
		eTwoPair,
		eThreeKind,
		eFullHouse,
		eFourKind,
		eFiveKind,
	} type[2];
	Hand(string hand, int bid) : hand(hand), bid(bid){
		string labels = "23456789TQKAJ";
		type[0] = eHigh;
		for (auto &l: labels) {
			int cnt = count(begin(hand), end(hand), l);
			if (l == 'J') {
				if (cnt) {
					if (cnt == 1)
						type[1] = (type[0] == eFourKind) ? eFiveKind :
							(type[0] == eThreeKind) ? eFourKind :
							(type[0] == eTwoPair) ? eFullHouse :
							(type[0] == eOnePair) ? eThreeKind : eOnePair;
					else if (cnt == 2)
						type[1] = (type[0] == eOnePair) ? eFourKind :
							(type[0] == eThreeKind) ? eFiveKind : eThreeKind;
					else if (cnt == 3)
						type[1] = (type[0] == eOnePair) ? eFiveKind : eFourKind;
					else if (cnt == 4 || cnt == 5)
						type[1] = eFiveKind;
				} else {
					type[1] = type[0];
				}
			}

			if (cnt == 2)
				type[0] = (type[0] == eOnePair) ? eTwoPair :
					(type[0] == eThreeKind) ? eFullHouse : eOnePair;
			else if (cnt == 3)
				type[0] = (type[0] == eOnePair) ? eFullHouse : eThreeKind;
			else if (cnt == 4)
				type[0] = eFourKind;
			else if (cnt == 5)
				type[0] = eFiveKind;
		}
	};
};

int main(int c, char **v) {
	int partOne = 0, partTwo = 0;
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);
	string line;
	vector<Hand> hands;
	while (getline(ifs, line)) {
		istringstream iss(line);
		string hand;
		int bid;
		iss >> hand >> bid;
		hands.push_back(Hand(hand, bid));
	}
	int part = 0;
	auto comp = [&part] (const Hand &l, const Hand &r) -> bool {
		auto labelToInt = [&part] (char c) -> int {
			string labels[] =  {"23456789TJQKA", "J23456789TQKA"};
			for (int i = 0; i < labels[part].length(); i++)
				if (labels[part][i] == c)
					return i;
			return 0;
		};
		if (r.type[part] > l.type[part])
			return true;
		if (r.type[part] == l.type[part]) {
			for (int i = 0; i < 5; i++) {
				char lch = l.hand[i], rch = r.hand[i];
				if (lch == rch)
					continue;
				if (labelToInt(rch) > labelToInt(lch))
					return true;
				else
					return false;
			}
		}
		return false;
	};

	sort(begin(hands), end(hands), comp);
	for (int i = 0; i < hands.size(); partOne += (i + 1) * hands[i].bid, i++);

	part++;
	sort(begin(hands), end(hands), comp);
	for (int i = 0; i < hands.size(); partTwo += (i + 1) * hands[i].bid, i++);

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;
	return 0;
}
