#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
	int partOne = 0;
	long int partTwo = 0;
	ifstream ifs(argc == 2 ? argv[1] : "input");

	while (true) {
		long int xT, yT, x0, x1, y0, y1;
		string ln;
		if (!getline(ifs, ln))
			break;
		sscanf(ln.c_str(), "Button A: X+%ld, Y+%ld", &x0, &y0);
		getline(ifs, ln);
		sscanf(ln.c_str(), "Button B: X+%ld, Y+%ld", &x1, &y1);
		getline(ifs, ln);
		sscanf(ln.c_str(), "Prize: X=%ld, Y=%ld", &xT, &yT);
		getline(ifs, ln);

		double c1 = y1 * x0 - x1 * y0;
		double A = (y1 * xT - x1 * yT) / c1;
		double B = (yT * x0 - xT * y0) / c1;

		if ((long int)A == A && (long int)B == B)
			partOne += A * 3 + B;


		xT += 10000000000000;
		yT += 10000000000000;
		long double A2 = (y1 * xT - x1 * yT) / c1;
		long double B2 = (yT * x0 - xT * y0) / c1;

		if ((long int)A2 == A2 && (long int)B2 == B2)
			partTwo += A2 * 3 + B2;
	}

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;

	return 0;
}
