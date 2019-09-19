// Hand.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Declares constructors functions, and operators related to Hand objects. Declare enums for Hand objects (classify them by rank group)


#ifndef HAND_H
#define HAND_H
#include "Card.h"
#include <vector>
#include <cstring>
#include <string>
using namespace std;


class Deck; //forward declaration

class Hand {
	friend ostream& operator<<(ostream &out, const Hand &h);
	friend void operator<<(Hand &H, Deck &D);
	friend void addFromDeck(Hand &H, Deck &D);

	friend bool poker_rank(const Hand &h1, const Hand &h2);
	friend bool rankfourofakind(const Hand &h1, const Hand &h2);
	friend bool rankfullhouse(const Hand &h1, const Hand &h2);
	friend bool rankflushnorank(const Hand &h1, const Hand &h2);
	friend bool rankstraight(const Hand &h1, const Hand &h2);
	friend bool rankthreeofakind(const Hand &h1, const Hand &h2);
	friend bool ranktwopairs(const Hand &h1, const Hand &h2);
	friend bool rankonepair(const Hand &h1, const Hand &h2);
public:
	enum HandRank { //classify hands into different hand groups
		straightflush = 0,
		fourofakind = 1,
		fullhouse = 2,
		flush = 3,
		straight = 4,
		threeofakind = 5,
		twopairs = 6,
		onepair = 7,
		norank = 8
	};




	Hand(); //default constructor
	Hand(const Hand &); // copy constructor
	Hand & operator= (const Hand &); //assignment operator
	int getsize() const; //get size
	bool operator== (const Hand &) const; //equivalence operator
	bool operator< (const Hand &) const; //less than operator
	string printOthers() const;
	string asString() const; //return output string
	void push_hand();
	Card operator[] (size_t i);
	void remove_card(size_t i);
	void add_card(Card &c);
	void sortHand();
	HandRank handrank;
	void addFromDeck(Hand &H, Deck &D);
private:
	vector<Card> cardHand;


};

ostream& operator<<(ostream &out, const Hand &h);
void operator<<(Hand &H, Deck &D);

bool checkSameSuit(vector<Card> & fiveCard);
int findMaxSameRank(int &differentRank, vector<Card> & fiveCard);
bool checkConsecutive(vector<Card> & fiveCard);

bool poker_rank(const Hand &h1, const Hand &h2);

bool rankfourofakind(Hand &h1, Hand &h2);
bool rankfullhouse(Hand &h1, Hand &h2);
bool rankflushnorank(Hand &h1, Hand &h2);
bool rankstraight(Hand &h1, Hand &h2);
bool rankthreeofakind(Hand &h1, Hand &h2);
bool ranktwopairs(Hand &h1, Hand &h2);
bool rankonepair(Hand &h1, Hand &h2);



#endif /* HAND_H */
