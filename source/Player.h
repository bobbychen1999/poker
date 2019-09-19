//Player.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Define methods related to Player struct.

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
using namespace std;

struct Player { 
	unsigned int numChips;
	unsigned int numChipBet;
	bool robot;
	bool fold;
	string name;
	Hand cards;
	unsigned int numWon;
	unsigned int numLost;
	Player(char*);
	friend ostream & operator<< (ostream &o, const Player &h);
	bool zeroChip();
	int lastRound;
	Hand bestHand();
};
ostream & operator<< (ostream &o, const Player &p);



#endif /* PLAYER_H */