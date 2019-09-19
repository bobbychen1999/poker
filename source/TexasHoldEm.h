// TexasHoldEm.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Declare constructors and methods related to TexasHoldEm class.

#ifndef TEXASHOLDEM_H
#define TEXASHOLDEM_H

#include "Game.h"
#include "Deck.h"

class TexasHoldEm : public Game {
protected:
	size_t w; //dealer position
	size_t betMaxPosition;
	unsigned int betMax = 0;
public:
	TexasHoldEm();
	vector<Card> shareCards;
	void giveShareCards();
	virtual int bettingPhase();
	void dealCard();
	void printOthers(size_t);
	virtual int after_round();
	int after_round_special_case();
	virtual int before_turn(Player &);
	virtual int turn(Player &);
	virtual int after_turn(Player &);
	virtual int before_round();
	virtual int round();
};

#endif