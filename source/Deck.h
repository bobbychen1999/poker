// Deck.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Declares constructors, functions and operators related to Deck objects.

#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <vector>
#include <cstring>
#include <string>
using namespace std;

class Hand; //forward declaration

class Deck {
	friend ostream& operator<<(ostream &out, const Deck &D);
	friend void operator<<(Hand &H, Deck &D);
	friend void operator<<(Deck &d1, Deck &d2);
	friend void operator<<(vector<Card> &c, Deck &d);
	friend void addFromDeck(Hand &H, Deck &D);
public:
	Deck();
	Deck(char *file);
	void add_card(Card c);
	void shuffleCards();
	int getsize() const;
	Card & dealCardHelper();
private:
	vector<Card> cardDeck;
	void load(char *file);
};

void operator<<(Deck &d1, Deck &d2);
ostream& operator<<(ostream &out, const Deck &D);
void pushCard(vector<Card> & cardstructs, string strCard);
bool check_str(string str);

#endif /* DECK_H */