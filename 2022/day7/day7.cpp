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

class FileSystem {
	std::string pwd;
	std::vector<std::string> dirs;
	std::vector<std::pair<int, std::string>> files;
public:
	std::string cd(std::string dir) {
		if (dir == "/") {
			pwd = "/";
		} else if (dir == "..") {
			size_t pos = pwd.rfind("/");
			std::string new_pwd = pwd.substr(0, pwd.rfind("/"));
			pwd = new_pwd;
		} else {
			if (pwd.back() != '/')
				pwd += "/";
			pwd += dir;
		}
		return pwd;
	};
	void handlePrintout(std::string output) {
		std::string first, second;
		std::stringstream ss(output);
		ss >> first;
		ss >> second;
		if (first == "dir") {
			std::string path = pwd;
			if (path != "/")
				path += "/";
			path += second;
			dirs.push_back(path);
		} else {
			std::string path = pwd;
			if (path != "/")
				path += "/";
			path += second;
			std::pair<int, std::string> pair(std::stoi(first), path);
			files.push_back(pair);
		}
	};
	int directorySize(std::string path) {
		int total = 0;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].second.find(path) != std::string::npos)
				total += files[i].first;
		}
		return total;
	};
	int partOne(void) {
		int total = 0;

		for (int i = 0; i < dirs.size(); i++) {
			int dirSz = directorySize(dirs[i]);
			if (dirSz <= 100000)
				total += dirSz;
		}
		return total;
	};
	int partTwo(void) {
		int min = 30000000 - (70000000 - directorySize("/"));
		int candidate = 0;

		for (int i = 0; i < dirs.size(); i++) {
			int dirSz = directorySize(dirs[i]);
			if (dirSz >= min && (!candidate || candidate > dirSz))
				candidate = dirSz;
		}
		return candidate;
	};
};

int main() {
	int marker, packet;
	Input in = Input("input");
	FileSystem fs;

	while (in.nextLine()) {
		std::string first, ln = in.getLine();
		std::stringstream ss = std::stringstream(ln);
		ss >> first;
		if (first == "$") {
			std::string operation, parameter;
			ss >> operation;
			if (operation == "cd") {
				ss >> parameter;
				fs.cd(parameter);
			} else if (operation == "ls") {
			} else {
				std:: cout << "Unknown operation: " << operation << std::endl;
			}
		} else {
			fs.handlePrintout(ln);
		}
	}
	std::cout << "Part One: " << fs.partOne() << std::endl;
	std::cout << "Part Two: " << fs.partTwo() << std::endl;

	return 0;
}
