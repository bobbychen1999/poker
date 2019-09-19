// FiveCardDraw.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Source file. Define methods related to FiveCardDraw class.

#include "stdafx.h"
#include "FiveCardDraw.h"
#include "ReturnType.h"
#include "Card.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <random>

const int fiveCard = 5;
const int start = 0;

FiveCardDraw::FiveCardDraw() {
	// default constructor of FiveCardDraw class
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

int FiveCardDraw::before_turn(Player &p) {
	// print out information of the player
	if (p.fold) {
		return success;
	}
	cout << endl;
	cout << p.name << endl;
	cout << p.cards;
	cout << "Which, if any, cards would you like to discard? Please enter the position indices (range from 0 to 4) of those cards on a line separated by whitespace. Sample Input: 0 1. Press 'Enter' to skip." << endl; // prompting
	bool tmp = true;
	while (tmp == true) {
		string response;
		vector<string> res;
		getline(cin, response);	
		istringstream str(response);
		string tmpstr;
		while (str >> tmpstr) {
			res.push_back(tmpstr);
		}
		if (res.size() > fiveCard) { //discard indices greater than the number of cards
			cout << "You can only discard 1 to 5 cards." << endl;
			cout << endl;
			res.clear();
			tmp = true;
			cout << p.name << endl;
			cout << p.cards;
			cout << "Which, if any, cards would you like to discard? Please enter the position indices (range from 0 to 4) of those cards on a line separated by whitespace. Sample Input: 0 1. Press 'Enter' to skip." << endl;
			continue;
		}
		bool valid = true;
		for (size_t k = 0; k < res.size(); ++k) {  //check whether the input are valid
			if (res[k] != "0" && res[k] != "1" && res[k] != "2" && res[k] != "3" && res[k] != "4") {
				valid = false;
				break;
			}
		}
		for (size_t j = 0; j < res.size(); ++j) {
			for (size_t k = j + 1; k < res.size(); ++k) {
				if (res[j] == res[k]) {
					valid = false;
					break;
				}
			}
		}
		if (valid == false) {   //invalid indices, reprompt
			cout << "Input contains invalid indices." << endl;
			cout << endl;
			cout << p.name << endl;
			cout << p.cards;
			cout << "Which, if any, cards would you like to discard? Please enter the position indices (range from 0 to 4) of those cards on a line separated by whitespace. Sample Input: 0 1. Press 'Enter' to skip." << endl;
			continue;
		}
		sort(res.begin(), res.end());

		int i = res.size() - 1;
		while (i >= 0) {
			size_t index = static_cast<size_t>(stoi(res[i]));
			discardDeck.add_card(p.cards[index]); //add to discard deck
			p.cards.remove_card(index);
			--i;
		}

		tmp = false;
	}
	cout << endl;
	return success;
}

int FiveCardDraw::turn(Player &p) {
	int num = p.cards.getsize();
	while (num < fiveCard) {
		if (mainDeck.getsize() == 0) {
			// if the main deck is empty, shuffle the discard deck and use it to deal cards to the player
			if (discardDeck.getsize() == 0) {
				// if neither deck has any card remaining, throw the exception
				return static_cast<int>(emptyDecks);
			}
			discardDeck.shuffleCards();
			p.cards << discardDeck;
			++num;
			continue;
		}
		p.cards << mainDeck;
		++num;
	}
	return success;
}

int FiveCardDraw::after_turn(Player &p) {
	// print the player's information
	cout << p.name << " " << p.cards;
	return success;
}


int FiveCardDraw::before_round() {
	const int allfold = 1; //all fold other than 1 player
	for (shared_ptr<Player> t : players) {
		(*t).numChipBet = 0;
		(*t).numChips -= 1;
		(*t).fold = false;
		numFold = 0;
		chipPot += 1;
	}
	mainDeck.shuffleCards();
	// deal cards until each player has five cards
	// starting from the player just past w, which is the dealer

	for (size_t i = w + 1; i < players.size(); ++i) {
		Player &p = *players[i];
		int r = turn(p);
		if (r != 0) throw r;
	}
	for (size_t i = 0; i <= w; ++i) {
		Player &p = *players[i];
		int r = turn(p);
		if (r != 0) throw r;
	}
	int temp = bettingPhase();
	if (temp == allfold) {
		return allfold;
	}
	// calling before_turn on each player
	for (size_t i = w + 1; i < players.size(); ++i) {
		Player &p = *players[i];
		before_turn(p);
	}
	for (size_t i = 0; i <= w; ++i) {
		Player &p = *players[i];
		before_turn(p);
	}
	return success;
}

int FiveCardDraw::round() {
	size_t i = w + 1;
	const int allFold = 1; //all fold except one player
	// calling turn and then after_turn on each player
	cout << "Hands after discarding cards:" << endl;
	if (i < players.size()) {
		while (i < players.size()) {
			Player &p = *players[i];
			int temp = turn(p);
			if (temp != 0) return temp;
			after_turn(p);
			++i;
		}
		i = 0;
		while (i < w + 1) {
			Player &p = *players[i];
			int temp = turn(p);
			if (temp != 0) return temp;
			after_turn(p);
			++i;
		}
	}
	else if (i >= players.size()) {
		i = 0;
		while (i < w + 1) {
			Player &p = *players[i];
			int temp = turn(p);
			if (temp != 0) return temp;
			after_turn(p);
			++i;
		}
	}
	int ret = bettingPhase();
	if (ret == allFold) return allFold;
	cout << endl;
	return success;
}

int FiveCardDraw::after_round() { //operations after the round
	const int allfold = 1; //all fold other than 1
	cout << endl;
	vector<shared_ptr<Player>> plays;
	bool equivalent = true;
	size_t s = 0;
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*(players[i])).fold == false) plays.push_back(players[i]);
	}
	for (size_t k = 0; k < plays.size(); ++k) {
		Player &p = *plays[k];
		p.cards.push_hand();
	}
	sort(plays.begin(), plays.end(), player_rank);
	cout << "After ranking: (From Highest Hand Rank to Lowest Hand Rank)" << endl;
	for (size_t i = 0; i < plays.size(); ++i) {
		cout << (*plays[i]).name << " " << (*plays[i]).cards << endl;
	}
	cout << endl;
	int countWinner = 0;
	while (equivalent == true) {
		Player &p = *plays[s];
		++p.numWon;
		p.lastRound = 1; // updates lastRound of winners
		++countWinner;
		if (s <= plays.size() - 2) {
			if (player_rank(plays[s], plays[s + 1]) == false) {
				++s; 
				continue;
			}
			else {
				equivalent = false;
				++s;
			}
		}
		else {
			equivalent = false;
		}
	}

	for (size_t j = s; j < plays.size(); ++j) {
		Player &tmp = *plays[j];
		++tmp.numLost;
	}
	equivalent = true;
	s = plays.size() - 1;
	while (equivalent == true) {
		Player &p = *plays[s];
		p.lastRound = 2; // updates lastRound of losers
		if (s >= 1) {
			if (player_rank(plays[s - 1], plays[s]) == true) {
				--s;
				equivalent = false;
				continue;
			}
			else {
				--s;
				continue;
			}
		}
		else {
			equivalent = false;
		}
	}
	for (size_t i = 0; i < players.size(); ++i) { //divide the chips
		if ((*(players[i])).lastRound == 1) {
			(*(players[i])).numChips += chipPot / countWinner;
			chipPot -= chipPot / countWinner;
		}
	}
	cout << "Results: (Output format: Name, Number of Wins, Number of Losses, Hand)" << endl;
	for (size_t i = 0; i < players.size(); ++i) {
		cout << (*(players[i])); //output the players' information
		if ((*(players[i])).fold == true) {
			cout << " (this player folded)" << endl;
		}
		else {
			cout << " " << (*(players[i])).cards;
		}
	}
	for (size_t j = 0; j < plays.size(); ++j) {
		Player &cardplayer = *plays[j];
		for (int k = cardplayer.cards.getsize() - 1; k >= 0; --k) {
			mainDeck.add_card(cardplayer.cards[k]); //add cards back to the main deck
			cardplayer.cards.remove_card(k);
		}
	}

	while (discardDeck.getsize() > 0) {
		mainDeck << discardDeck; //add cards back to the main deck
	}
	for (auto s = players.begin(); s != players.end();) {
		Player &p = *(*s);
		if (p.numChips == 0) {
			bool t = p.zeroChip();
			if (t == false) {
				ofstream fst; //store players' informtion into files
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

		ofstream fst; //store players' data to the files
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

bool player_rank(shared_ptr<Player> p1, shared_ptr<Player> p2) {
	if (p1 == 0) {
		// if the first pointer is singular
		return false;
	}
	else if (p2 == 0) {
		// if the second pointer is singular
		return true;
	}
	else {
		Player &player1 = *p1;
		Player &player2 = *p2;
		// calling the poker_rank function, using each player's Hand as parameters
		return poker_rank(player1.cards, player2.cards);
	}
}


int FiveCardDraw::bettingPhase() { //betting phase after a round
	const int oneinput = 1;
	const int allfold = 1; //all fold other than 1
	cout << endl;
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
		if (betAlready == false) { //tell whether the player folds or not
			if ((curr == (w + 1)%players.size()) && roundStarts) return success;
			if (numFold == players.size() - 1) { //if all players fold except 1
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
				return allfold;
			}
			if ((*players[curr]).numChips == 0) {
				++curr;
				if (curr >= players.size()) curr = 0;
				continue;
			}
			bool valid = false;
			while (valid == false) {
				cout << (*players[curr]).name << endl;
				cout << (*players[curr]).cards;
				cout << endl;
				cout << "What do you want to do? Type 'check' to check. Type '1' to bet 1 chip. Type '2' to bet 2 chips." << endl;
				cout << "The current highest bet is: " << betMax << " chips" << endl;
				cout << "You have currently bet " << (*players[curr]).numChipBet << " chips" << endl;
				cout << "You have: " << (*players[curr]).numChips << " chips" << endl;
				string temp;
				getline(cin, temp); //read input from the users
				istringstream strread(temp);
				string tmpstr;
				vector<string> input;
				while (strread >> tmpstr) {
					input.push_back(tmpstr);
				}
				if (input.size() != oneinput) {
					valid = false;
					cout << endl;
					continue;
				}
				if (input[start] == "check") { //player types check
					valid = true;
					cout << endl;
				}
				else if (input[start] == "1") { //player bets 1
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
				else if (input[start] == "2") { //player bets 2
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
			if ((*(players[curr])).fold == true) { //skip the player who folds
				++curr;
				if (curr >= players.size()) curr = 0;
				continue;
			}
			if (numFold == players.size() - 1) { //if all players fold except 1
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
				return allfold;
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
			while (valid == false) {
				const int oneinput = 1;
				cout << (*players[curr]).name << endl;
				cout << (*players[curr]).cards << endl;
				cout << "What do you want to do? Type 'fold' to fold. Type 'call' to call. Type '1' to raise 1. Type '2' to raise 2." << endl;
				cout << "The current highest bet is: " << betMax << " chips" << endl;
				cout << "You have currently bet " << (*players[curr]).numChipBet << " chips" << endl;
				cout << "You have: " << (*players[curr]).numChips << " chips" << endl;
				string temp;
				getline(cin, temp); // read input from users
				istringstream strread(temp);
				string tmpstr;
				vector<string> input;
				while (strread >> tmpstr) {
					input.push_back(tmpstr);
				}
				if (input.size() != oneinput) {
					valid = false;
					cout << endl;
					continue;
				}
				if (input[start] == "fold") { // the player folds
					int num = (*(players[curr])).numChipBet;
					(*(players[curr])).numChipBet = 0;
					chipPot += num;
					valid = true;
					(*(players[curr])).fold = true;
					cout << endl;
					++numFold;
				}
				else if (input[start] == "call") { // the player calls
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
				else if (input[start] == "1") { // the player bets 1
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
				else if (input[start] == "2") { // the player bets 2
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

int FiveCardDraw::after_round_special_case() {
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
	while (discardDeck.getsize() > 0) {
		mainDeck << discardDeck;
	}
	for (auto i = players.cbegin(); i != players.cend(); ) {
		if ((*(*i)).numChips == 0) {
			bool temp = (*(*i)).zeroChip();
			if (!temp) {
				ofstream fst; // store players' data to files
				fst.open((*(*i)).name);
				fst << p;
				fst.close();
				i = players.erase(i);
			}
			else ++i;
		}
		else ++i;
	}
	bool leave = true;
	while (leave == true) {
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
				strname.push_back(strinput);
			}
		}

		ofstream fst; //store players' data to files
		for (size_t i = 0; i < strname.size(); ++i) {
			vector<Player>::const_iterator ci;
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
	while (join == true) {
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
				strname.push_back(strinput);
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