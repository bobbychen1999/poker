// Hand.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Defines constructors and operators for Hand types. Find the poker hand rank of each hand.

#include "stdafx.h"
#include "Hand.h"
#include "Deck.h"
#include "ReturnType.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <iterator>
#include <algorithm>
#include <random>

using namespace std;

Hand::Hand() { //default constructor: initializes empty Hand objects
}


Hand::Hand(const Hand &h) : cardHand(h.cardHand), handrank(h.handrank) { //copy constructor that takes in a const Hand object and copy the contents of the passed object
}


Hand & Hand::operator= (const Hand &h) { //assignment operator that takes in a const Hand object and then assign the contents of the passed object
	if (*this == h) { //check whether the two objects are equivalent using the equivalence operator
		return *this;
	}
	else { // if doesn't equal, then assign the contents of the passed object
		this->cardHand.clear();
		this->cardHand = h.cardHand;
		this->handrank = h.handrank;
		return *this;
	}
}

int Hand::getsize() const { //returns the size of the Hand object
	return static_cast<int>(this->cardHand.size()); //safer to have cast because it is converting from size_t to int
}

Card Hand::operator[] (size_t i) {
	if (i < cardHand.size()) return cardHand[i];
	else throw static_cast<int>(indexOutOfBound);
}

void Hand::remove_card(size_t i) {
	if (i < cardHand.size()) {
		for (size_t j = i; j < cardHand.size() - 1; j++) {
			// shifting all previous cards
			cardHand[j] = cardHand[j + 1];
		}
		cardHand.pop_back();
	}
	// if i is out of bound
	else throw static_cast<int>(indexOutOfBound);
}

bool Hand::operator== (const Hand & h) const { //equivalence operator: return true when two hand objects are equivalent
	vector<Card>::const_iterator i = this->cardHand.cbegin();
	vector<Card>::const_iterator j = h.cardHand.cbegin();
	while (i != this->cardHand.cend() && j != h.cardHand.cend()) {  // iterates until the end of the two Hand objects
		if (*i != *j) { // if two cards are not equal (using operator defined in Card class), then return false
			return false;
		}
		++i;
		++j;
	}
	return (i == this->cardHand.cend() && j == h.cardHand.cend()); //return false when lengths are different
}

bool Hand::operator< (const Hand & h) const { //less than operator (lexical order)
	vector<Card>::const_iterator i = this->cardHand.cbegin();
	vector<Card>::const_iterator j = h.cardHand.cbegin();
	while (i != this->cardHand.cend() && j != h.cardHand.cend()) {
		if (*i < *j) { //card is smaller
			return true;
		}
		else if (*j < *i) {
			return false;
		}
		else {
			++i;
			++j;
		}
	}
	if (i == this->cardHand.cend() && j == h.cardHand.cend()) { // two hands are equal, return false
		return false;
	}
	return (i == this->cardHand.cend()); //return true when shorter hand is smaller
}

string Hand::asString() const { //print out cards in the Hand object
	string str = "";
	for (size_t i = 0; i < this->cardHand.size(); ++i) {
		const string cardrank[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
		const char cardsuit[] = { 'C','D','H','S' };
		str += cardrank[this->cardHand.at(i).rank] + cardsuit[this->cardHand.at(i).suit] + " ";  //valid card definition string output
		
	}
	return str;
}

string Hand::printOthers() const {
	string str = "";
	for (size_t i = 0; i < this->cardHand.size(); ++i) {
		const string cardrank[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
		const char cardsuit[] = { 'C','D','H','S' };
		if (cardHand[i].faceUp == true) {
			str += cardrank[this->cardHand.at(i).rank] + cardsuit[this->cardHand.at(i).suit] + " ";  //valid card definition string output
		}
		else str += "** ";
	}
	return str;
}

ostream& operator<<(ostream &out, const Hand &h) { // non-member insertion operator uses ostream to print out contents returned by asString function
	out << h.asString() << endl;
	return out;
}

void operator<<(Hand &H, Deck &D) { // non-member insertion operator that removes one card from Deck object and then pass it to Hand object
	if (D.cardDeck.empty() == true) {
		throw static_cast<int>(ReturnType::notEnoughCardsGiven);  //not enough cards in the deck, cast because the catch block is catching int type and the return value of the whole program should be int type.
	}
	H.cardHand.push_back(D.cardDeck.back());
	D.cardDeck.pop_back();
	sort(H.cardHand.begin(), H.cardHand.end());
}

void addFromDeck(Hand &H, Deck &D) {
	if (D.cardDeck.empty() == true) {
		throw static_cast<int>(ReturnType::notEnoughCardsGiven);  //not enough cards in the deck, cast because the catch block is catching int type and the return value of the whole program should be int type.
	}
	H.cardHand.push_back(D.cardDeck.back());
	D.cardDeck.pop_back();
}

void Hand::add_card(Card &c) {
	cardHand.push_back(c);
}

void Hand::sortHand() {
	sort(cardHand.begin(), cardHand.end());
}

void Hand::push_hand() { //assign the enumeration of Hand objects
	int differentRank;
	const int twosame = 2;
	const int twodifferent = 2;
	const int threesame = 3;
	const int threedifferent = 3;
	const int foursame = 4;
	sort(this->cardHand.begin(), this->cardHand.end());

	if (checkConsecutive(this->cardHand) == true && checkSameSuit(this->cardHand) == true) {  // consecutive and same suit																				 
		this->handrank = HandRank::straightflush;
		return;
	}
	if (findMaxSameRank(differentRank, this->cardHand) == foursame) {    // four cards have the same rank
		this->handrank = HandRank::fourofakind;
		return;
	}
	if (findMaxSameRank(differentRank, this->cardHand) == threesame) {
		if (differentRank == twodifferent) {   // three cards have the same rank, other two have the same other rank
			this->handrank = HandRank::fullhouse;
			return;
		}
	}
	if (checkConsecutive(this->cardHand) == false && checkSameSuit(this->cardHand) == true) {  // not consecutive, but same suit
		this->handrank = HandRank::flush;
		return;
	}
	if (checkConsecutive(this->cardHand) == true && checkSameSuit(this->cardHand) == false) {  // consecutive, but not same suit
		this->handrank = HandRank::straight;
		return;
	}
	if (findMaxSameRank(differentRank, this->cardHand) == threesame) {
		if (differentRank == threedifferent) {  // three cards have the same rank, other two have different ranks
			this->handrank = HandRank::threeofakind;
			return;
		}
	}
	if (findMaxSameRank(differentRank, this->cardHand) == twosame) {
		if (differentRank == threedifferent) {  // two have the same rank, other two have the same other rank, one is different than any other card
			this->handrank = HandRank::twopairs;
			return;
		}
		else {  // only two have the same rank, all other three are different
			this->handrank = HandRank::onepair;
			return;
		}
	}
	this->handrank = HandRank::norank;
}

bool checkSameSuit(vector<Card> & fiveCard) {  // check whether five cards are same suit
	int suit = fiveCard.at(0).suit;
	for (size_t i = 1; i < fiveCard.size(); ++i) {
		if (suit != fiveCard.at(i).suit) {
			return false;
		}
	}
	return true;
}


int findMaxSameRank(int &differentRank, vector<Card> & fiveCard) {   // returns the maximize number of same rank cards
	int maxLength = 0;
	int tmpLength = 1;
	differentRank = 1;
	for (size_t i = 1; i < fiveCard.size(); ++i) {
		if (fiveCard.at(i).rank == fiveCard.at(i - 1).rank) {
			++tmpLength;
			if (tmpLength > maxLength) {  // if tmpLength bigger, then update maxLength
				maxLength = tmpLength;
			}
		}
		else {
			if (tmpLength > maxLength) {  // if tmpLength bigger, then update maxLength
				maxLength = tmpLength;
			}
			++differentRank;
			tmpLength = 1;
		}
	}
	return maxLength;
}


bool checkConsecutive(vector<Card> & fiveCard) {  // check whether five cards are consecutive
	const int start = 0;
	if (fiveCard.at(start).rank == Card::Rank::two && fiveCard.at(start + 1).rank == Card::Rank::three && fiveCard.at(start + 2).rank == Card::Rank::four &&
		fiveCard.at(start + 3).rank == Card::Rank::five && fiveCard.at(start + 4).rank == Card::Rank::ace) { // straight: ace, two, three, four, five
		return true;
	}
	for (size_t i = 1; i < fiveCard.size(); ++i) { // other cases of straight
		if (fiveCard.at(i).rank == fiveCard.at(i - 1).rank + 1) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}