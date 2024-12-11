#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

unordered_map<long int, long int> *rCaches;
long int recursiveCachedSolve(long int stone, int depth);
long int intermediary(long int stone, int depth) {
	if (!stone)
		return recursiveCachedSolve(1, depth);

	long int tmp = stone;
	int dg = 0;
	for (; tmp; tmp /= 10, dg++);
	if (dg & 1)
		return recursiveCachedSolve(stone * 2024, depth);

	long int pow = 1;
	for (int k = 0; k < dg >> 1; k++, pow *= 10);

	long int ret = recursiveCachedSolve(stone / pow, depth);
	ret += recursiveCachedSolve(stone % pow, depth);
	return ret;
}

long int recursiveCachedSolve(long int stone, int depth) {
	int long ret = 0;
	if (!depth--)
		return 1;
	try {
		return rCaches[depth].at(stone);
	}
	catch (out_of_range) {
		rCaches[depth][stone] = intermediary(stone, depth);
	}
	return rCaches[depth].at(stone);
}

int main(int argc, char **argv) {
	long int partOne = 0, partTwo = 0;
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<long int> stones;

	for (long int tmp; ifs >> tmp;)
		stones.push_back(tmp);

	rCaches = new unordered_map<long int, long int>[25];
	for (auto &s: stones)
		partOne += recursiveCachedSolve(s, 25);
	delete[] rCaches;
	cout << "Part One: " << partOne << endl;

	rCaches = new unordered_map<long int, long int>[75];
	for (auto &s: stones)
		partTwo += recursiveCachedSolve(s, 75);
	delete[] rCaches;
	cout << "Part Two: " << partTwo << endl;

	return 0;
}
