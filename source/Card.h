// Card.h Lab4

// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Declare enumeration for card suit and card rank, as well as operators related to Cards.

#ifndef CARD_H
#define CARD_H
#include <vector>
#include <string>
#include <cstring>

using namespace std;


struct Card
{
	enum Suit {
		club = 0,
		diamond = 1,
		heart = 2,
		spade = 3
	};

	enum Rank {
		two = 0,
		three = 1,
		four = 2,
		five = 3,
		six = 4,
		seven = 5,
		eight = 6,
		nine = 7,
		ten = 8,
		jack = 9,
		queen = 10,
		king = 11,
		ace = 12
	};
	Suit suit;
	Rank rank;
	bool faceUp = true;

	Card();
	bool operator< (const Card & card) const; //sort the cards according to rank first then by suit in ascending order
	bool operator!= (const Card & card) const; //tell whether two cards are different
};


#endif /* CARD_H */