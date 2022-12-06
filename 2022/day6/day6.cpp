#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Input {
	std::ifstream ifs;
	std::string lastLine;
	char lastByte;
public:
	Input(std::string filename) : lastByte(0), lastLine("")
				{ ifs = std::ifstream(filename); };
	int nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
	int nextByte(void) { ifs.get(lastByte); return !ifs.eof(); };
	char getByte(void) { return lastByte; };
};

int findUnique(std::string stream, int start, int markerLen) {
	int off = start;
	for (; off < stream.size(); off++) {
		bool uniq = true;
		std::string marker = std::string(stream, off, markerLen);
		for (int i = 0; i < markerLen; i++) {
			for (int j = 0; j < markerLen; j++) {
				if (i == j)
					continue;
				if (marker[i] == marker[j])
					uniq = false;
			}
		}
		if (uniq)
			break;
	}
	return off + markerLen;
}

int main() {
	int marker, packet;
	Input in = Input("input");

	if (!in.nextLine())
		return -1;
	std::string stream = in.getLine();
	std::cout << stream.size() << std::endl;
	marker = findUnique(stream, 0, 4);
	std::cout << "Part One: " << marker << std::endl;
	packet = findUnique(stream, marker, 14);
	std::cout << "Part Two: " << packet << std::endl;

	return 0;
}
