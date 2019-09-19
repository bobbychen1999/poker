// Player.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Source file. Define methods related to Player struct.

#include "stdafx.h"
#include "Player.h"
#include "Hand.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

Player::Player(char* file) : name(static_cast<string>(file)), numWon(0), numLost(0), robot(false), numChips(20), lastRound(0), numChipBet(0), fold(false) {
	string temp = file;
	if (temp[temp.size() - 1] == '*') {
		// checks whether the player is automated
		name.pop_back(); // removing the '*'
		robot = true;
	}
	ifstream in(file);
	if (in.is_open()) {
		// parsing the file
		string strinput;
		in >> strinput;
		name = strinput;
		in >> strinput;
		numWon = stoi(strinput);
		in >> strinput;
		numLost = stoi(strinput);
		in >> strinput;
		numChips = stoi(strinput);
		if (numChips <= 0) {
			zeroChip();
		}
	}
	else {
		// if cannot open the file, reset numWon and numLost to be 0
		numWon = 0;
		numLost = 0;
		numChips = 20;
	}
}

bool Player::zeroChip() { //deal with zero chip case
	const int start = 0;
	cout << endl;
	cout << name << endl;
	cout << "You have 0 chip. Type 'yes' to reset to chips to 20 and keep playing, or type 'no' to quit." << endl;
	string temp;
	getline(cin, temp);
	istringstream strread(temp);
	string tmpstr;
	vector<string> input;
	while (strread >> tmpstr) {
		input.push_back(tmpstr);
	}
	if (input.size() != 1) {
		return zeroChip();
	}
	if (input[start] == "yes") { //press yes to reset to 20 chips
		numChips = 20;
		return true;
	}
	else if (input[start] == "no") return false;
	else return zeroChip();
}

ostream & operator<< (ostream &o, const Player &p) {
	// insert the player's name, its num of winning and losing to the passed ostream
	o << p.name << " " << p.numWon << " " << p.numLost << " " << p.numChips;
	return o;
}


Hand Player::bestHand() { // find the best hand of the player from seven cards
	vector<Hand> possibleHand; //store all the possible hands
	for (int i = 0; i < cards.getsize()-1; ++i) {
		for (int j = i + 1; j < cards.getsize(); ++j) {
			Hand handtmp;
			for (int k = 0; k < cards.getsize(); ++k) {
				if (k != i && k != j) {
					handtmp.add_card(cards[k]);
				}
			}
			handtmp.sortHand();
			handtmp.push_hand();
			possibleHand.push_back(handtmp);
		}
	}
	sort(possibleHand.begin(), possibleHand.end(), poker_rank); //sort all the possible hands according to the poker rank
	return (*(possibleHand.begin())); //return the best hand
}