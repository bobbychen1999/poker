// FiveCardDraw.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Declare constructors and methods related to FiveCardDraw class.

#ifndef FIVECARDDRAW_H
#define FIVECARDDRAW_H
#include "Game.h"
#include "Hand.h"


class FiveCardDraw : public Game {
protected:
	size_t w; //dealer position
	Deck discardDeck;
	size_t betMaxPosition;
	unsigned int betMax = 0;
public:
	FiveCardDraw();
	virtual int before_turn(Player &);
	virtual int turn(Player &);
	virtual int after_turn(Player &);
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual int after_round_special_case();
	virtual int bettingPhase();
};

bool player_rank(shared_ptr<Player> p1, shared_ptr<Player> p2);

#endif /* FIVECARDDRAW_H */