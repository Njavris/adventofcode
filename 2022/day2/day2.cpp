#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Input {
	std::ifstream input;
	std::string ln;
public:
	Input(std::string filename) { input = std::ifstream(filename); };
	int next(void) { return !!std::getline(input, ln); };
	std::string getLine(void) { return ln; };
};

enum shapes {
	ROCK = 0,
	PAPER = 1,
	SCISSORS = 2,
	SHAPES_MAX = 3,
};

char my_encodings[] = {
	[ROCK] = 'X',
	[PAPER] = 'Y',
	[SCISSORS] = 'Z',
};

char their_encodings[] = {
	[ROCK] = 'A',
	[PAPER] = 'B',
	[SCISSORS] = 'C',
};

char *names[] = {
	[ROCK] = "ROCK",
	[PAPER] = "PAPER",
	[SCISSORS] = "SCISSORS",
};

/*
ROCK beats SCISSORS
SCISSORS beats PAPER
PAPER beats ROCK
*/
int win[] = {
	[ROCK] = SCISSORS,
	[PAPER] = ROCK,
	[SCISSORS] = PAPER,
};

int lose[] = {
	[ROCK] = PAPER,
	[PAPER] = SCISSORS,
	[SCISSORS] = ROCK,
};

#define DRAW(M, T)	((M) == (T))
#define WIN(M, T)	(win[(M)] == ((T)))
#define LOSE(M, T)	(lose[(M)] == ((T)))

int round_score_part_one(int my, int their) {
	int my_score = 0, their_score = 0;
	my_score += my + 1;
	their_score += their + 1;

	if (DRAW(my, their)) {
		my_score += 3;
		their_score += 3;
	} else if (WIN(my, their)) {
		my_score += 6;
	} else if (LOSE(my, their)) {
		their_score += 6;
	}
	return my_score;
}

enum action {
	LOSE = 0,
	DRAW = 1,
	WIN = 2,
};

char action_encoding[] = {
	[LOSE] = 'X',
	[DRAW] = 'Y',
	[WIN] = 'Z',
};

char *action_names[] = {
	[LOSE] = "LOSE",
	[DRAW] = "DRWAW",
	[WIN] = "WIN",
};

int round_action_part_two(int their, char action) {
	int shape = 0;

	if (action == LOSE)
		shape = win[their];
	else if (action == DRAW)
		shape = their;
	else if (action == WIN)
		shape = lose[their];
	return shape;
}

int main() {
	int sum = 0;
	int rounds = 0;
	Input in("input");

	while(in.next()) {
		char second, first;
		int score;
		int my_shape;
		std::string ln = in.getLine();
		std::cout << "Strategy: \"" <<  ln << "\"";
		std::stringstream ststr(ln);
		ststr >> first >> second;

		my_shape = round_action_part_two((int)(first - 'A'),(int)(second - 'X'));
//		score = round_score_part_one((int)(second - 'X'), (int)(first - 'A'));
		score = round_score_part_one(my_shape, (int)(first - 'A'));
		sum += score;
		rounds ++;

		std::cout << " THEIR:" << names[first - 'A'] << "(" << first <<
		") have to:" << action_names[second - 'X'] << "(" << second <<
		") MY:" << names[my_shape] << " score:" << score << std::endl;
	}
	std::cout << "In " << rounds << " rounds total score = " << sum << "!"<< std:: endl;

	return 0;
}
