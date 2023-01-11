#include <fstream>
#include <chrono>

class Input {
	std::ifstream ifs;
	std::string lastLine;
public:
	Input(std::string filename) : lastLine("") { ifs = std::ifstream(filename); };
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
};

class Timer {
	bool stopped;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
public:
	Timer(void) : stopped(false) { startTiming(); };
	void startTiming(void) {
		stopped = false;
		start = std::chrono::high_resolution_clock::now();
	};
	void stopTiming(void) {
		end = std::chrono::high_resolution_clock::now();
		stopped = true;
	};
	std::string getTimeStr(void) {
		std::string ret = "";
		if (!stopped)
			return "";
		auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		auto durationS = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		auto durationM = std::chrono::duration_cast<std::chrono::minutes>(end - start);
		auto durationH = std::chrono::duration_cast<std::chrono::hours>(end - start);
		std::chrono::duration<long, std::micro> intUs = durationUs;
		std::chrono::duration<long, std::milli> intMs = durationMs;
		std::chrono::duration<long, std::ratio<1>> intS = durationS;
		std::chrono::duration<long, std::ratio<60>> intM = durationM;
		std::chrono::duration<long, std::ratio<3600>> intH = durationH;

		if (intMs.count()) {
			if (intS.count()) {
				if (intM.count()) {
					if (intH.count()) {
						ret += std::to_string(intH.count()) + "h ";
					}
					ret += std::to_string(intM.count() % 60) + "m ";
				}
				ret += std::to_string(intS.count() % 60) + "s ";
			}
			ret += std::to_string(intMs.count() % 1000) + "ms ";
		}
		ret += std::to_string(intUs.count() % 1000) + "us ";
		ret += "(" + std::to_string(intUs.count()) + "us total)";
		return ret;
	};
};
