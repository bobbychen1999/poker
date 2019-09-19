// CSE332_Lab4.cpp : Defines the entry point for the console application.
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Contains main function of the program as well as exception handling blocks.

#include "stdafx.h"
#include "Card.h"
#include "Hand.h"
#include "Deck.h"
#include "ReturnType.h"
#include "FiveCardDraw.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <sstream>
using namespace std;

const int programName = 0;
const int gameName = 1;
const int sameStr = 0;
const char* FiveCard = "FiveCardDraw";
const char* SevenCard = "SevenCardStud";
const char* TexasHold = "TexasHoldEm";

int recur() {    //ask user to start new game or not
	const int start = 0;
	const int numInput = 1;
	const int allfoldExceptOne = 1;
	bool valid = false;
	while (valid != true) {
		cout << "What do you want to do now? Type 'yes' to start a new game. Type 'no' to quit the game." << endl;
		string temp;
		getline(cin, temp);
		istringstream strread(temp);
		string tmpstr;
		vector<string> input;
		while (strread >> tmpstr) {
			input.push_back(tmpstr);
		}
		if (input.size() != numInput) {
			valid = false;
			cout << endl;
			continue;
		}
		if (input[start] == "yes") {
			valid = true;
			cout << "Please provide the information of the game. The first cmd argument should be the game's type. The other arguments should be names of players." << endl;
			string temp;
			getline(cin, temp);
			istringstream strread(temp);
			string tmpstr;
			vector<string> inputt;
			while (strread >> tmpstr) {
				inputt.push_back(tmpstr);
			}
			string t = inputt[start]; // obtaining the game type
			Game::start_game(t);
			Game *pGame = Game::instance();
			Game &p = *pGame; // obtaining a reference to the game
			for (size_t i = 1; i < inputt.size(); ++i) {
				try {
					p.add_player(inputt[i]);
				}
				catch (...) {
					// if there are duplicates, ignore that duplicate, continue
					continue;
				}
			}
			const int sufficient = 2;
			if (inputt[start] == "FiveCardDraw") {
				while (p.getSize() >= sufficient) {
					// while there are at least two players in the game, keep calling before_round, round and after_round
					int res = p.before_round();
					if (res == allfoldExceptOne) {
						p.after_round_special_case();
						continue;
					}
					int r = p.round();
					if (r == (int)emptyDecks) {
						// if the decks are empty
						p.stop_game();
						throw r;
					}
					if (r == allfoldExceptOne) {
						p.after_round_special_case();
						continue;
					}
					p.after_round();
				}
				p.stop_game();
			}
			else if (inputt[start] == "SevenCardStud") { // the game is SevenCardStud
				while (p.getSize() >= sufficient) {
					int res = p.round();
					if (res == allfoldExceptOne) {
						p.after_round_special_case();
						continue;
					}
					p.after_round();
				}
				p.stop_game();
			}
			else if (inputt[start] == "TexasHoldEm") { // the game is TexasHoldEm
				while (p.getSize() >= sufficient) {
					int res = p.round();
					if (res == allfoldExceptOne) {
						p.after_round_special_case();
						continue;
					}
					p.after_round();
				}
				p.stop_game();
			}
			return recur();
		}
		if (input[start] == "no") {
			return success;
		}
		else continue;
	}
	return success;
}

int main(int argc, char*argv[])
{
	try {
		const int sufficientNum = 4;
		if (argc < sufficientNum) {
			// check if there are enough cmd arguments
			throw static_cast<int> (incorrectNumOfcmdArgument);
		}
		string t = argv[gameName]; // obtaining the game type
		Game::start_game(t);
		Game *pGame = Game::instance();
		Game &p = *pGame; // obtaining a reference to the game
		for (int i = 2; i < argc; ++i) {
			try {
				p.add_player(argv[i]);
			}
			catch (...) {
				// if there are duplicates, ignore that duplicate, continue
				continue;
			}
		}
		const int sufficient = 2;
		const int allfoldExceptOne = 1;
		if (strcmp(argv[gameName], FiveCard) == sameStr) {
			while (p.getSize() >= sufficient) {
				// while there are at least two players in the game, keep calling before_round, round and after_round
				int res = p.before_round();
				if (res == allfoldExceptOne) {
					p.after_round_special_case();
					continue;
				}
				int r = p.round();
				if (r == allfoldExceptOne) {
					p.after_round_special_case();
					continue;
				}
				if (r == static_cast<int>(emptyDecks)) {
					// if the decks are empty
					p.stop_game();
					throw r;
				}
				p.after_round();
			}
			p.stop_game();
		}
		else if (strcmp(argv[gameName], SevenCard) == sameStr) { //start SevenCardStud game
			while (p.getSize() >= sufficient) {
				int res = p.round();
				if (res == allfoldExceptOne) {
					p.after_round_special_case();
					continue;
				}
				p.after_round();
			}
			p.stop_game();
		}
		else if (strcmp(argv[gameName], TexasHold) == sameStr) { //start TexasHoldEm game
			while (p.getSize() >= sufficient) {
				int res = p.round();
				if (res == allfoldExceptOne) {
					p.after_round_special_case();
					continue;
				}
				p.after_round();
			}
			p.stop_game();
		}
		recur();
	}
	catch (int &i) {
		// catching the defined exceptions, print the corresponding error messages
		if (i == static_cast<int>(incorrectNumOfcmdArgument)) {
			usageMessage(argv[programName], "Invalid number of cmd arguments, must have at least three cmd arguments other than the program's name");
			return incorrectNumOfcmdArgument;
		}
		if (i == static_cast<int>(emptyDecks)) {
			usageMessage(argv[programName], "Empty decks");
			return emptyDecks;
		}
		if (i == static_cast<int>(no_game_in_progress)) {
			usageMessage(argv[programName], "No game in progress, cannot stop game");
			return no_game_in_progress;
		}
		if (i == static_cast<int>(unknown_game)) {
			usageMessage(argv[programName], "Unknown game");
			return unknown_game;
		}
		if (i == static_cast<int>(invalidFileName)) {
			usageMessage(argv[programName], "Invalid file name, cannot open file");
			return invalidFileName;
		}
		if (i == static_cast<int>(indexOutOfBound)) {
			usageMessage(argv[programName], "Index out of bound");
			return indexOutOfBound;
		}
		if (i == static_cast<int>(game_already_started)) {
			usageMessage(argv[programName], "Game already started");
			return game_already_started;
		}
		if (i == static_cast<int>(instance_not_available)) {
			usageMessage(argv[programName], "Instance is not available");
			return instance_not_available;
		}
	}
	catch (...) {
		// catch all exceptions
		usageMessage(argv[programName], "An unknown exception has been thrown");
		return unknown;
	}
	return success; // returns 0 for success
}
