#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

int64_t solve(int64_t t, int64_t r) {
	double d = sqrt(t * t - 4 * r);
	double x0 = (-t + d) / -2;
	double x1 = (-t - d) / -2;
	int64_t ret = floor(x1) - ceil(x0) + 1;
	if (ceil(x0) == x0)
		ret --;
	if (floor(x1) == x1)
		ret --;
	return ret;
}

int main(int c, char **v) {
	if (c != 2)
		return -1;
	ifstream ifs(v[1]);

	string ln_t, ln_r;
	getline(ifs, ln_t);
	getline(ifs, ln_r);
	istringstream iss_t(ln_t), iss_r(ln_r);
	iss_t.ignore(32, ' ');
	iss_r.ignore(32, ' ');

	int t, r, partOne = 1;
	while ( iss_t >> t && iss_r >> r)
		partOne *= solve(t, r);

	ln_t.erase(remove(begin(ln_t), end(ln_t), ' '), end(ln_t));
	ln_r.erase(remove(begin(ln_r), end(ln_r), ' '), end(ln_r));
	int64_t t2 = strtoll(ln_t.substr(ln_t.find(":") + 1).c_str(), NULL, 10);
	int64_t r2 = strtoll(ln_r.substr(ln_r.find(":") + 1).c_str(), NULL, 10);
	int64_t partTwo = solve(t2, r2);

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;
	return 0;
}
