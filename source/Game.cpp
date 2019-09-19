// Game.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Source file. Define methods related to Game class.

#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"
#include "ReturnType.h"
#include <fstream>

Game *Game::p = 0;
Game* null = 0;

Game * Game::instance() {
	if (p == null) {
		throw static_cast<int>(instance_not_available);
	}
	auto temp = p;
	return temp;
}

void Game::start_game(string &s) {
	if (p != null) {
		throw static_cast<int>(game_already_started);
	}
	if (s == "FiveCardDraw") {
		p = new FiveCardDraw;
	}
	else if (s == "SevenCardStud") {
		p = new SevenCardStud;
	}
	else if (s == "TexasHoldEm") {
		p = new TexasHoldEm;
	}
	else throw static_cast<int> (unknown_game);
}

void Game::stop_game() {
	// stop the game
	if (p == null) throw static_cast<int>(no_game_in_progress);
	Game &g = *p;
	ofstream fst;
	for (shared_ptr<Player> d : g.players) {
		Player temp = *d;
		fst.open(temp.name);
		fst << temp;
		fst.close();
	}
	delete p;
	p = null;
}

void Game::add_player(const string &s) {
	for (shared_ptr<Player> d : players) {
		Player temp = *d;
		if (temp.name == s) {
			// if the player is already in the game, throw the corresponding exception
			throw static_cast<int>(already_playing);
		}
	}
	char* cstring = new char[s.length() + 1];
	strcpy_s(cstring, s.length() + 1, s.c_str());
	shared_ptr<Player> t = make_shared<Player>(cstring); // adding the player to the vector
	Player &p = *t;
	if (p.numChips != 0) players.push_back(t);
}

shared_ptr<Player> Game::find_player(const string &s) {
	for (shared_ptr<Player> pp : players) {
		Player temp = *pp;
		if (temp.name == s) return pp; // returns the found player
	}
	return 0; // returns a singular pointer
}

int Game::getSize() {
	// returns the number of players in the game
	return players.size();
}

Game::~Game() {

}
