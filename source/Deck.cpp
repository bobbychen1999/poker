// Deck.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Defines constructors and functions related to Deck objects. Read cards from files, print out cards in a deck, etc.

#include "stdafx.h"
#include "Deck.h"
#include "Card.h"
#include "Hand.h"
#include "ReturnType.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <iterator>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

const int start = 0;

Deck::Deck() { //default constructor
}

Deck::Deck(char *file) { //constructor takes input file name
	load(file);  //call load function to push cards into private vector member variable
}

void Deck::add_card(Card c) {
	cardDeck.push_back(c);
}

void Deck::load(char *file) {
	ifstream in(file);
	if (in.is_open()) {
		string strline = "";
		while (getline(in, strline)) {   // get a line and store in strline
			string strlineupdate = "";
			if (strline.find_first_of("//") != string::npos) {
				strlineupdate = strline.substr(0, strline.find_first_of("//"));   // find the substring that ignores the content after "comment" symbol
			}
			else {
				strlineupdate = strline;
			}
			istringstream iss(strlineupdate);
			string strinput = "";
			while (iss >> strinput) {	// read individual card form a line of input
				for (size_t i = 0; i < strinput.length(); ++i) {
					strinput[i] = toupper(strinput[i]);   // convert all alphabets to uppercase letters
				}
				if (check_str(strinput) == true) {       // valid card definition string
					pushCard(this->cardDeck, strinput);    // push valid card into vector
				}
			}
		}
	}
	else {
		throw static_cast<int>(ReturnType::invalidFileName); //file open error, cast because the catch block is catching int type and the return value of the whole program should be int type.
	}
}

void Deck::shuffleCards() {  //shuffle the deck of cards into random order
	std::random_device random;
	std::mt19937 rg(random());
	std::shuffle(this->cardDeck.begin(), this->cardDeck.end(), rg); //call std shuffle method, 'rg' is the random generator
}

int Deck::getsize() const { //returns the size of private vector member variable
	return static_cast<int>(this->cardDeck.size()); //safer to have cast because it is converting from size_t to int
}

ostream& operator<<(ostream &out, const Deck &D) { //output the cards in the deck object using ostream
	string output = "";
	for (size_t i = 0; i < D.cardDeck.size(); ++i) {
		const string cardrank[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
		const char cardsuit[] = { 'C','D','H','S' };
		output += cardrank[D.cardDeck.at(i).rank] + cardsuit[D.cardDeck.at(i).suit] + "\n";  //valid card definition string output
	}
	out << output << endl;
	return out;
}

void operator<<(Deck &d1, Deck &d2) {
	Card temp = d2.cardDeck[d2.cardDeck.size() - 1];
	d2.cardDeck.pop_back();
	d1.cardDeck.push_back(temp);
}

void pushCard(vector<Card> & cardstructs, string strCard) {  // push valid card into vector
	Card card;
	if (strCard[start] == '2' || strCard[start] == '3' || strCard[start] == '4' || strCard[start] == '5' ||
		strCard[start] == '6' || strCard[start] == '7' || strCard[start] == '8' || strCard[start] == '9') {
		int i = strCard[start] - '0' - 2;    // enum index card rank
		card.rank = static_cast<Card::Rank>(i); //converts i from 'int' to 'Card::Rank' because card.rank is of 'Card::Rank' type
	}
	else if (strCard[start] == 'J') {
		card.rank = Card::Rank::jack;
	}
	else if (strCard[start] == 'Q') {
		card.rank = Card::Rank::queen;
	}
	else if (strCard[start] == 'K') {
		card.rank = Card::Rank::king;
	}
	else if (strCard[start] == 'A') {
		card.rank = Card::Rank::ace;
	}
	else {
		card.rank = Card::Rank::ten;
	}

	if (strCard[strCard.length() - 1] == 'C') {    //check last letter of a valid card definition string
		card.suit = Card::Suit::club;
	}
	else if (strCard[strCard.length() - 1] == 'D') {
		card.suit = Card::Suit::diamond;
	}
	else if (strCard[strCard.length() - 1] == 'H') {
		card.suit = Card::Suit::heart;
	}
	else {
		card.suit = Card::Suit::spade;
	}
	cardstructs.push_back(card);   // push valid card definition string into vector
}


bool check_str(string str) {   // check whether the input string is a Card type
	const int lengthtwo = 2;
	const int lengththree = 3;
	if (str.length() == lengthtwo) {   // string length of 2
		return ((str[start] == '2' || str[start] == '3' || str[start] == '4' || str[start] == '5' || str[start] == '6' || str[start] == '7' ||
			str[start] == '8' || str[start] == '9' || str[start] == 'J' || str[start] == 'Q' || str[start] == 'K' || str[start] == 'A') &&
			(str[start + 1] == 'C' || str[start + 1] == 'D' || str[start + 1] == 'H' || str[start + 1] == 'S'));
	}
	else if (str.length() == lengththree) {  // string length of 3
		return (str[start] == '1' && str[start + 1] == '0' && (str[start + 2] == 'C' || str[start + 2] == 'D' || str[start + 2] == 'H' || str[start + 2] == 'S'));
	}
	else
		return false;
}

Card & Deck::dealCardHelper() {
	Card &temp = cardDeck[cardDeck.size() - 1];
	return temp;
}

void operator<<(vector<Card> &c, Deck &d) {
	c.push_back(d.cardDeck[d.getsize() - 1]);
	d.cardDeck.pop_back();
}