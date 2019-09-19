// Game.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Declare constructors and methods related to Game class.

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include <memory>

class Game {
protected:
	static Game *p;
	Deck mainDeck;
	vector<shared_ptr<Player>> players;
public:
	int numFold;
	int chipPot;
	size_t maxPosition;
	int betMax;
	static Game * instance();
	static void start_game(string &);
	static void stop_game();
	void add_player(const string &);
	shared_ptr<Player> find_player(const string &);
	virtual ~Game();
	int getSize();
	virtual int before_turn(Player &) = 0;
	virtual int turn(Player &) = 0;
	virtual int after_turn(Player &) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual int bettingPhase() = 0;
	virtual int after_round_special_case() = 0;
};

#endif /* GAME_H */