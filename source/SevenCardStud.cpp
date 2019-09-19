// SevenCardStud.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Source file. Define methods related to SevenCardStud class.

#include "stdafx.h"
#include "FiveCardDraw.h"
#include "ReturnType.h"
#include "Card.h"
#include "SevenCardStud.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

const int start = 0;

void SevenCardStud::dealCard(int j) {  // deal cards to the players
	for (size_t i = w + 1; i < players.size(); ++i) {
		if (mainDeck.getsize() > 0) {
			Card &temp = mainDeck.dealCardHelper();
			if (j == 0) temp.faceUp = false;
			else temp.faceUp = true;
			addFromDeck((*(players[i])).cards, mainDeck);
		}
		else throw static_cast<int>(emptyDecks);
	}
	for (size_t i = 0; i <= w; ++i) {
		if (mainDeck.getsize() > 0) {
			Card &temp = mainDeck.dealCardHelper();
			if (j == 0) temp.faceUp = false;
			else temp.faceUp = true;
			addFromDeck((*(players[i])).cards, mainDeck);
		}
		else throw static_cast<int>(emptyDecks);
	}
}

SevenCardStud::SevenCardStud() {
	w = 0;
	const int numSuit = 4;
	const int numRank = 13;
	for (int i = 0; i < numRank; i++) {
		// add the 52 distinct cards to the main deck
		for (int j = 0; j < numSuit; j++) {
			Card temp;
			temp.rank = (Card::Rank)i;
			temp.suit = (Card::Suit)j;
			mainDeck.add_card(temp);
		}
	}
	mainDeck.shuffleCards(); // shuffle the deck
}

int SevenCardStud::bettingPhase() {  //betting phase after rounds
	const int oneInput = 1;
	size_t curr = w + 1;
	if (curr >= players.size()) {
		curr = 0;
	}
	betMax = 0;
	bool betAlready = false;
	bool roundStarts = false;
	while (true) {
		if ((*(players[curr])).fold == true) {
			++curr;
			if (curr >= players.size()) curr = 0;
			continue;
		}
		if (betAlready == false) { //tell whether the player bet or not
			if ((curr == (w + 1) % players.size()) && roundStarts) return success;
			if ((*players[curr]).numChips == 0) {
				++curr;
				if (curr >= players.size()) curr = 0;
				continue;
			}
			bool valid = false;
			while (valid == false) {
				cout << (*players[curr]).name << endl;
				cout << (*players[curr]).cards << endl;
				printOthers(curr); //print the hands of others
				cout << "What do you want to do? Type 'check' to check. Type '1' to bet 1 chip. Type '2' to bet 2 chips." << endl;
				cout << "The current highest bet is: " << betMax << " chips" << endl;
				cout << "You have currently bet " << (*players[curr]).numChipBet << " chips" << endl;
				cout << "You have: " << (*players[curr]).numChips << " chips" << endl;
				string temp;
				getline(cin, temp);
				istringstream strread(temp);
				string tmpstr;
				vector<string> input;
				while (strread >> tmpstr) { //read strings from the users
					input.push_back(tmpstr);
				}
				if (input.size() != oneInput) {
					valid = false;
					cout << endl;
					continue;
				}
				if (input[start] == "check") { //player checks
					valid = true;
					cout << endl;
				}
				else if (input[start] == "1") { //player bets one
					if ((*players[curr]).numChips >= 1) {
						(*(players[curr])).numChips -= 1;
						(*(players[curr])).numChipBet += 1;
						if (betMax < (*(players[curr])).numChipBet) {
							betMax = (*(players[curr])).numChipBet;
						}
						maxPosition = curr;
						betAlready = true;
						valid = true;
						cout << endl;
					}
					else {
						cout << "You don't have enough chips to do this action" << endl;
						continue;
					}
				}
				else if (input[start] == "2") { //player bets two
					if ((*players[curr]).numChips >= 2) {
						(*(players[curr])).numChips -= 2;
						(*(players[curr])).numChipBet += 2;
						betMax = (*(players[curr])).numChipBet;
						maxPosition = curr;
						betAlready = true;
						valid = true;
						cout << endl;
					}
					else {
						cout << "You don't have enough chips to do this action" << endl;
						continue;
					}
				}
				else {
					cout << endl;
					continue;
				}
			}
			roundStarts = true;
			++curr;
			curr = curr % players.size();
		}
		else {
			if ((*(players[curr])).fold == true) { // skip the player who folds
				++curr;
				if (curr >= players.size()) curr = 0;
				continue;
			}
			if (numFold == players.size() - 1) { 
				for (size_t i = 0; i < players.size(); ++i) {
					int num = (*(players[i])).numChipBet;
					chipPot += num;
					(*(players[i])).numChipBet = 0;
				}
				(*(players[curr])).numChips += chipPot;
				(*(players[curr])).numWon += 1;
				for (size_t i = 0; i < players.size(); ++i) {
					if (i != curr) ++(*(players[i])).numLost;
				}
				chipPot = 0;
				return 1;
			}
			if (curr == maxPosition) {
				if ((*(players[curr])).numChipBet == betMax) {
					for (size_t i = 0; i < players.size(); ++i) {
						int num = (*(players[i])).numChipBet;
						chipPot += num;
						(*(players[i])).numChipBet = 0;
					}
					return 0; // all have called the same amount
				}
			}
			if ((*(players[curr])).numChips == 0) {
				++curr;
				if (curr >= players.size()) curr = 0;
				continue;
			}
			bool valid = false;
			while (valid == false) { // ask user to bet 1, bet 2, fold, or call
				cout << (*players[curr]).name << endl;
				cout << (*players[curr]).cards << endl;
				printOthers(curr);
				cout << "What do you want to do? Type 'fold' to fold. Type 'call' to call. Type '1' to raise 1. Type '2' to raise 2." << endl;
				cout << "The current highest bet is: " << betMax << " chips in this betting phase" << endl;
				cout << "You have currently bet " << (*players[curr]).numChipBet << " chips" << endl;
				cout << "You have: " << (*players[curr]).numChips << " chips" << endl;
				string temp;
				getline(cin, temp);
				istringstream strread(temp);
				string tmpstr;
				vector<string> input;
				while (strread >> tmpstr) { //read inputs from users
					input.push_back(tmpstr);
				}
				if (input.size() != oneInput) {
					valid = false;
					cout << endl;
					continue;
				}
				if (input[start] == "fold") { //player folds
					int num = (*(players[curr])).numChipBet;
					(*(players[curr])).numChipBet = 0;
					chipPot += num;
					valid = true;
					(*(players[curr])).fold = true;
					cout << endl;
					++numFold;
				}
				else if (input[start] == "call") { //player calls
					unsigned int t = betMax - (*(players[curr])).numChipBet;
					if ((*players[curr]).numChips >= t) {
						(*(players[curr])).numChips -= t;
						(*(players[curr])).numChipBet = betMax;
						valid = true;
						cout << endl;
					}
					else {
						cout << "You don't have enough chips to do this action" << endl;
						continue;
					}
				}
				else if (input[start] == "1") { // player bets 1
					unsigned int t = betMax - (*(players[curr])).numChipBet + 1;
					if ((*players[curr]).numChips >= t) {
						(*(players[curr])).numChips -= t;
						(*(players[curr])).numChipBet = betMax + 1;
						++betMax;
						maxPosition = curr;
						valid = true;
						cout << endl;
					}
					else {
						cout << "You don't have enough chips to do this action" << endl;
						continue;
					}
				}
				else if (input[start] == "2") { //player bets 2
					unsigned int t = betMax - (*(players[curr])).numChipBet + 2;
					if ((*players[curr]).numChips >= t) {
						(*(players[curr])).numChips -= t;
						(*(players[curr])).numChipBet = betMax + 2;
						betMax += 2;
						maxPosition = curr;
						valid = true;
						cout << endl;
					}
					else {
						cout << "You don't have enough chips to do this action" << endl;
						continue;
					}
				}
				else {
					cout << endl;
					continue;
				}
			}
			++curr;
			curr = curr % players.size();
		}
	}
}

int SevenCardStud::round() { //deal cards to players according to the rules
	const int allFold = 1; //all fold other than 1 player
	int temp = 0;
	mainDeck.shuffleCards();
	for (size_t i = 0; i < players.size(); ++i) {
		Player &p = *players[i];
		p.numChipBet = 0;
		p.fold = false;
		p.numChips -= 1;
		chipPot += 1;
	}
	dealCard(0);
	dealCard(0);
	dealCard(1);
	temp = bettingPhase();
	if (temp == allFold) return allFold;
	dealCard(1);
	temp = bettingPhase();
	if (temp == allFold) return allFold;
	dealCard(1);
	temp = bettingPhase();
	if (temp == allFold) return allFold;
	dealCard(1);
	temp = bettingPhase();
	if (temp == allFold) return allFold;
	dealCard(0);
	temp = bettingPhase();
	if (temp == allFold) return allFold;
	return success;
}

void SevenCardStud::printOthers(size_t i) {  //print the cards of other players
	for (size_t j = 0; j < players.size(); ++j) {
		if (j != i) {
			if ((*players[j]).fold == true) {
				cout << (*players[j]).name << " folded" << endl;
				continue;
			}
			cout << (*players[j]).name << " ";
			cout << (*players[j]).cards.printOthers() << endl;
		}
	}
}

int SevenCardStud::after_round() { //operations after a round
	vector<Hand> playerHands;
	vector<shared_ptr<Player>> plays;
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*players[i]).fold == false) {
			plays.push_back(players[i]);
			playerHands.push_back((*players[i]).bestHand());
		}
	}
	for (size_t i = 0; i < playerHands.size(); ++i) {
		for (size_t j = i + 1; j < playerHands.size(); ++j) {
			if (poker_rank(playerHands[j], playerHands[i])) { //tell which player has high hand
				auto temp1 = plays[i];
				plays[i] = plays[j];
				plays[j] = temp1;
				auto temp2 = playerHands[i];
				playerHands[i] = playerHands[j];
				playerHands[j] = temp2;
			}
		}
	}
	cout << "After ranking: (From Highest Hand Rank to Lowest Hand Rank)" << endl;
	for (size_t i = 0; i < plays.size(); ++i) {
		cout << (*plays[i]).name << " " << playerHands[i] << endl;
	}
	size_t i = 0;
	bool equivalent = true;
	int countWinner = 0;
	while (equivalent) {
		Player &p = *plays[i];
		Hand &h = playerHands[i];
		++p.numWon;
		p.lastRound = 1; // updates lastRound of winners
		++countWinner;
		if (i <= plays.size() - 2) {
			if (poker_rank(playerHands[i], playerHands[i + 1]) == false) {
				++i;
				continue;
			}
			else {
				equivalent = false;
				++i;
			}
		}
		else {
			equivalent = false;
		}
	}
	size_t temp = i;
	for (size_t j = temp; j < plays.size(); ++j) {
		Player &tmp = *plays[j];
		++tmp.numLost;
		tmp.lastRound = 2;
	}
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*(players[i])).lastRound == 1) {
			(*(players[i])).numChips += chipPot / countWinner;
			chipPot -= chipPot / countWinner;
		}
	}
	cout << "Results: (Output format: Name, Number of Wins, Number of Losses, Hand)" << endl;
	for (size_t i = 0; i < players.size(); ++i) {
		cout << (*(players[i]));
		if ((*(players[i])).fold == true) {
			cout << " (this player folded)" << endl;
		}
		else {
			cout << " " << (*(players[i])).cards;
		}
	}
	for (size_t j = 0; j < players.size(); ++j) {
		Player &cardplayer = *players[j];
		for (int k = cardplayer.cards.getsize() - 1; k >= 0; --k) {
			mainDeck.add_card(cardplayer.cards[k]);
			cardplayer.cards.remove_card(k);
		}
	}
	for (auto s = players.begin(); s != players.end();) { //store players' data to the file
		Player &p = *(*s);
		if (p.numChips == 0) {
			bool t = p.zeroChip();
			if (t == false) {
				ofstream fst;
				fst.open(p.name);
				fst << p;
				fst.close();
				s = players.erase(s);
			}
			else ++s;
		}
		else ++s;
	}
	bool leave = true;
	while (leave == true) { // reprompt until user says no
		cout << endl;
		cout << "Which human player wants to leave the game? (Please input the names on a line separated by whitespace. Enter 'no' to skip)" << endl;
		string strinput;
		vector<string> strname;
		vector<string> strnametmp;
		getline(cin, strinput);
		istringstream strread(strinput);
		string tmpstr;
		while (strread >> tmpstr) { //read input from users
			strnametmp.push_back(tmpstr);
		}
		for (size_t i = 0; i < strnametmp.size(); ++i) {
			if (strnametmp[i] == "no") {
				leave = false;
				break;
			}
			else {
				strname.push_back(strnametmp[i]);
			}
		}

		ofstream fst; //store players' data to files
		for (size_t i = 0; i < strname.size(); ++i) {
			for (auto ci = players.cbegin(); ci != players.cend();) {
				Player &p = *(*ci);
				if (p.name == strname[i]) {
					fst.open(strname[i]);
					fst << p;
					fst.close();
					ci = players.erase(ci);
				}
				else ++ci;
			}
		}
	}
	bool join = true;
	while (join == true) { //reprompt until user says "no"
		cout << endl;
		cout << "Who wants to join the game? (Please input the names on a line separated by whitespace. Enter 'no' to skip)" << endl;
		string strinput;
		vector<string> strname;
		vector<string> strnametmp;
		getline(cin, strinput);
		istringstream strread(strinput);
		string tmpstr;
		while (strread >> tmpstr) { //read input from users
			strnametmp.push_back(tmpstr);
		}
		for (size_t i = 0; i < strnametmp.size(); ++i) {
			if (strnametmp[i] == "no") {
				join = false;
				break;
			}
			else {
				strname.push_back(strnametmp[i]);
			}
		}

		for (size_t i = 0; i < strname.size(); ++i) {
			try {
				add_player(strname[i]);
			}
			catch (...) {
				continue;
			}
		}

	}

	if (w >= players.size() - 1) {
		w = 0;
	}
	else {
		++w;
	}

	return success;
}

int SevenCardStud::before_turn(Player &)
{
	return 0;
}

int SevenCardStud::turn(Player &)
{
	return 0;
}

int SevenCardStud::after_turn(Player &)
{
	return 0;
}

int SevenCardStud::before_round()
{
	return 0;
}

int SevenCardStud::after_round_special_case() { //all players fold except one
	cout << "Results: (Output format: Name, Number of Wins, Number of Losses, Hand)" << endl;
	for (size_t i = 0; i < players.size(); ++i) {
		cout << (*(players[i]));
		if ((*(players[i])).fold == true) {
			cout << " (this player folded)" << endl;
		}
		else {
			cout << " " << (*(players[i])).cards;
		}
	}
	for (size_t j = 0; j < players.size(); ++j) {
		Player &cardplayer = *players[j];
		for (int k = cardplayer.cards.getsize() - 1; k >= 0; --k) {
			mainDeck.add_card(cardplayer.cards[k]);
			cardplayer.cards.remove_card(k);
		}
	}
	bool leave = true;
	while (leave == true) { // reprompt until user says no
		cout << endl;
		cout << "Which human player wants to leave the game? (Please input the names on a line separated by whitespace. Enter 'no' to skip)" << endl;
		string strinput;
		vector<string> strname;
		vector<string> strnametmp;
		getline(cin, strinput);
		istringstream strread(strinput);
		string tmpstr;
		while (strread >> tmpstr) { //read input from user
			strnametmp.push_back(tmpstr);
		}
		for (size_t i = 0; i < strnametmp.size(); ++i) {
			if (strnametmp[i] == "no") {
				leave = false;
				break;
			}
			else {
				strname.push_back(strnametmp[i]);
			}
		}

		ofstream fst; //store players' information into files
		for (size_t i = 0; i < strname.size(); ++i) {
			for (auto ci = players.cbegin(); ci != players.cend();) {
				Player &p = *(*ci);
				if (p.name == strname[i]) {
					fst.open(strname[i]);
					fst << p;
					fst.close();
					ci = players.erase(ci);
				}
				else ++ci;
			}
		}
	}
	bool join = true;
	while (join == true) { //reprompt until user says "no"
		cout << endl;
		cout << "Who wants to join the game? (Please input the names on a line separated by whitespace. Enter 'no' to skip)" << endl;
		string strinput;
		vector<string> strname;
		vector<string> strnametmp;
		getline(cin, strinput);
		istringstream strread(strinput);
		string tmpstr;
		while (strread >> tmpstr) { //read input from users
			strnametmp.push_back(tmpstr);
		}
		for (size_t i = 0; i < strnametmp.size(); ++i) {
			if (strnametmp[i] == "no") {
				join = false;
				break;
			}
			else {
				strname.push_back(strnametmp[i]);
			}
		}

		for (size_t i = 0; i < strname.size(); ++i) {
			try {
				add_player(strname[i]);
			}
			catch (...) {
				continue;
			}
		}
	}
	return success;
}