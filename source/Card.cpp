// Card.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Defines the operators for Card types.

#include "stdafx.h"
#include "Card.h"

using namespace std;

Card::Card() {
}

bool Card::operator< (const Card & card) const {  // sort cards according to rank first, then by suit
	return ((this->rank < card.rank) || (this->rank == card.rank && this->suit < card.suit));
}

bool Card::operator!= (const Card & card) const { //tell whether two cards are different
	return ((this->rank != card.rank) || (this->suit != card.suit));
}
