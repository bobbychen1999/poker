// PokerRank.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Implements poker ranking function that ranks hands from highest to lowest according to poker ranking definition.

#include "stdafx.h"
#include "Hand.h"
#include <algorithm>
using namespace std;

const int handCount = 5;
const int start = 0;
const int last = handCount - 1;
const int mid = (handCount - 1) / 2;

bool poker_rank(const Hand &h1, const Hand &h2) { //rank the hands according to poker ranking definition, return true when the first Hand object is larger than the second one
	if (h1 == h2) {
		return false;
	}
	if (h1.handrank < h2.handrank) { //compare the handrank of two Hands
		return true;
	}
	else if (h1.handrank > h2.handrank) {
		return false;
	}
	else { //two hands belong to the same rank group, then sort hands within a certain group
		if (h1.handrank == Hand::HandRank::straightflush) { //both are straightflush
			return rankstraight(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::fourofakind) { //both are four of a kind
			return rankfourofakind(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::fullhouse) { //both are full house
			return rankfullhouse(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::flush) { //both are flush
			return rankflushnorank(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::straight) { //both are straight
			return rankstraight(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::threeofakind) { //both are three of a kind
			return rankthreeofakind(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::twopairs) { //both are two pairs
			return ranktwopairs(h1, h2);
		}
		else if (h1.handrank == Hand::HandRank::onepair) { //both are one pair
			return rankonepair(h1, h2);
		}
		else {
			return rankflushnorank(h1, h2); //both are no rank
		}
	}
}

bool rankstraight(const Hand &h1, const Hand &h2) { //rank two straightflushes and two straights
	if (h1.cardHand.at(start).rank == Card::Rank::two && h1.cardHand.at(start + 1).rank == Card::Rank::three && h1.cardHand.at(start + 2).rank == Card::Rank::four &&
		h1.cardHand.at(start + 3).rank == Card::Rank::five && h1.cardHand.at(start + 4).rank == Card::Rank::ace) { // h1 is straight: ace, two, three, four, five
		return false;
	}
	else if (h2.cardHand.at(start).rank == Card::Rank::two && h2.cardHand.at(start + 1).rank == Card::Rank::three && h2.cardHand.at(start + 2).rank == Card::Rank::four &&
		h2.cardHand.at(start + 3).rank == Card::Rank::five && h2.cardHand.at(start + 4).rank == Card::Rank::ace) {  // h2 is straight: ace, two, three, four, five
		return true;
	}
	else {
		return (h1.cardHand.at(start + 4).rank > h2.cardHand.at(start + 4).rank);  // compare the card rank of the biggest card in a hand
	}
}

bool rankfourofakind(const Hand &h1, const Hand &h2) { // rank two 'four of a kind'
	if (h1.cardHand.at(mid).rank > h2.cardHand.at(mid).rank) { //compare the middle card (one of the 4 cards that have the same rank)
		return true;
	}
	else if (h1.cardHand.at(mid).rank < h2.cardHand.at(mid).rank) {
		return false;
	}
	else { //the middle card is equal, then compare the single card of both hands
		Card::Rank onerank1; //records the single card in Hand 1
		Card::Rank onerank2; //records the single card in Hand 2
		if (h1.cardHand.at(mid).rank != h1.cardHand.at(mid + 2).rank) {
			onerank1 = h1.cardHand.at(mid + 2).rank;
		}
		else {
			onerank1 = h1.cardHand.at(mid - 2).rank;
		}
		if (h2.cardHand.at(mid).rank != h2.cardHand.at(mid + 2).rank) {
			onerank2 = h2.cardHand.at(mid + 2).rank;
		}
		else {
			onerank2 = h2.cardHand.at(mid - 2).rank;
		}
		return (onerank1 > onerank2); //compare the single cards of two hands
	}
}

bool rankfullhouse(const Hand &h1, const Hand &h2) { //rank two full houses
	if (h1.cardHand.at(mid).rank > h2.cardHand.at(mid).rank) { //compare the middle card (one of the 3 cards that have the same rank)
		return true;
	}
	else if (h1.cardHand.at(mid).rank < h2.cardHand.at(mid).rank) {
		return false;
	}
	else {
		Card::Rank tworank1; //records the rank of the 2 cards that have the same rank of Hand 1
		Card::Rank tworank2; //records the rank of the 2 cards that have the same rank of Hand 2
		if (h1.cardHand.at(mid).rank != h1.cardHand.at(mid + 1).rank) {
			tworank1 = h1.cardHand.at(mid + 1).rank;
		}
		else {
			tworank1 = h1.cardHand.at(mid - 1).rank;
		}
		if (h2.cardHand.at(mid).rank != h2.cardHand.at(mid + 1).rank) {
			tworank2 = h2.cardHand.at(mid + 1).rank;
		}
		else {
			tworank2 = h2.cardHand.at(mid - 1).rank;
		}
		return (tworank1 > tworank2); //compare the the rank of the 2 cards that have the same rank
	}
}

bool rankflushnorank(const Hand &h1, const Hand &h2) { //rank two flushes and two 'norank' hands
	int i = handCount - 1;
	if (h1.cardHand.at(i).rank < h2.cardHand.at(i).rank) { //compare the highest cards of two hands
		return false;
	}
	else if (h2.cardHand.at(i).rank < h1.cardHand.at(i).rank) {
		return true;
	}
	else { //if equal, compare the second highest cards of two hands
		--i;
		if (h1.cardHand.at(i).rank < h2.cardHand.at(i).rank) {
			return false;
		}
		else if (h2.cardHand.at(i).rank < h1.cardHand.at(i).rank) {
			return true;
		}
		else {  //if equal, compare the third highest cards of two hands
			--i;
			if (h1.cardHand.at(i).rank < h2.cardHand.at(i).rank) {
				return false;
			}
			else if (h2.cardHand.at(i).rank < h1.cardHand.at(i).rank) {
				return true;
			}
			else {  //if equal, compare the fourth highest cards of two hands
				--i;
				if (h1.cardHand.at(i).rank < h2.cardHand.at(i).rank) {
					return false;
				}
				else if (h2.cardHand.at(i).rank < h1.cardHand.at(i).rank) {
					return true;
				}
				else {  //if equal, compare the fifth highest cards of two hands
					--i;
					return (h1.cardHand.at(i).rank > h2.cardHand.at(i).rank);
				}
			}
		}
	}
}

bool rankthreeofakind(const Hand &h1, const Hand &h2) { //rank two 'three of a kind'
	if (h1.cardHand.at(mid).rank > h2.cardHand.at(mid).rank) { // compare the middle card (one of the 3 cards that have the same rank)
		return true;
	}
	else if (h1.cardHand.at(mid).rank < h2.cardHand.at(mid).rank) {
		return false;
	}
	else { // middle cards are equal
		Card::Rank fourth1; //records the fourth card of Hand 1, the smaller single card
		Card::Rank fifth1;  //records the fifth card of Hand 1, the bigger single card
		Card::Rank fourth2; //records the fourth card of Hand 2, the smaller single card
		Card::Rank fifth2; //records the fifthth card of Hand 2, the bigger single card

		if (h1.cardHand.at(mid).rank != h1.cardHand.at(mid + 1).rank) { //find fourth1, fifth1
			fourth1 = h1.cardHand.at(mid + 1).rank;
			fifth1 = h1.cardHand.at(last).rank;
		}
		else if (h1.cardHand.at(mid).rank != h1.cardHand.at(mid - 1).rank) {
			fourth1 = h1.cardHand.at(start).rank;
			fifth1 = h1.cardHand.at(start + 1).rank;
		}
		else {
			fourth1 = h1.cardHand.at(start).rank;
			fifth1 = h1.cardHand.at(last).rank;
		}

		if (h2.cardHand.at(mid).rank != h2.cardHand.at(mid + 1).rank) { //find fourth2, fifth2
			fourth2 = h2.cardHand.at(mid + 1).rank;
			fifth2 = h2.cardHand.at(last).rank;
		}
		else if (h2.cardHand.at(mid).rank != h2.cardHand.at(mid - 1).rank) {
			fourth2 = h2.cardHand.at(start).rank;
			fifth2 = h2.cardHand.at(start + 1).rank;
		}
		else {
			fourth2 = h2.cardHand.at(start).rank;
			fifth2 = h2.cardHand.at(last).rank;
		}

		if (fifth1 > fifth2) { //rank by bigger card of two hands
			return true;
		}
		else if (fifth1 < fifth2) {
			return false;
		}
		else { //if equal, then rank by smaller card of two hands
			return (fourth1 > fourth2);
		}
	}
}

bool ranktwopairs(const Hand &h1, const Hand &h2) {  // rank two "two pairs"
	Card::Rank onecard1; // record the single card of Hand 1
	Card::Rank onecard2; // record the single card of Hand 2
	Card::Rank bigpair1; // record the bigger pair of Hand 1
	Card::Rank bigpair2; // record the bigger pair of Hand 2
	Card::Rank smallpair1; // record the smaller pair of Hand 1
	Card::Rank smallpair2; // record the smaller pair of Hand 2

	if (h1.cardHand.at(start).rank != h1.cardHand.at(start + 1).rank) { //find onecard1, bigpair1, smallpair1
		onecard1 = h1.cardHand.at(start).rank;
		smallpair1 = h1.cardHand.at(start + 1).rank;
		bigpair1 = h1.cardHand.at(last).rank;
	}
	else if (h1.cardHand.at(last).rank != h1.cardHand.at(last - 1).rank) {
		onecard1 = h1.cardHand.at(last).rank;
		smallpair1 = h1.cardHand.at(start).rank;
		bigpair1 = h1.cardHand.at(last - 1).rank;
	}
	else {
		onecard1 = h1.cardHand.at(mid).rank;
		smallpair1 = h1.cardHand.at(start).rank;
		bigpair1 = h1.cardHand.at(last).rank;
	}

	if (h2.cardHand.at(start).rank != h2.cardHand.at(start + 1).rank) { //find onecard2, bigpair2, smallpair2
		onecard2 = h2.cardHand.at(start).rank;
		smallpair2 = h2.cardHand.at(start + 1).rank;
		bigpair2 = h2.cardHand.at(last).rank;
	}
	else if (h2.cardHand.at(last).rank != h2.cardHand.at(last - 1).rank) {
		onecard2 = h2.cardHand.at(last).rank;
		smallpair2 = h2.cardHand.at(start).rank;
		bigpair2 = h2.cardHand.at(last - 1).rank;
	}
	else {
		onecard2 = h2.cardHand.at(mid).rank;
		smallpair2 = h2.cardHand.at(start).rank;
		bigpair2 = h2.cardHand.at(last).rank;
	}

	if (bigpair1 > bigpair2) { //rank by bigger pair first
		return true;
	}
	else if (bigpair1 < bigpair2) {
		return false;
	}
	else { //if equal, rank by smaller pair
		if (smallpair1 > smallpair2) {
			return true;
		}
		else if (smallpair1 < smallpair2) {
			return false;
		}
		else { //if equal, rank by single card
			return (onecard1 > onecard2);
		}
	}
}

bool rankonepair(const Hand &h1, const Hand &h2) { //rank two "one pair"
	Card::Rank third1; //records the third card of Hand 1, the smallest single card
	Card::Rank third2; //records the third card of Hand 2, the smallest single card
	Card::Rank fourth1; //records the fourth card of Hand 1, the middle value single card
	Card::Rank fourth2; //records the fourth card of Hand 2, the middle value single card
	Card::Rank fifth1; //records the fifth card of Hand 1, the biggest single card
	Card::Rank fifth2; //records the fifth card of Hand 2, the biggest single card
	Card::Rank pair1; //record the pair of Hand 1
	Card::Rank pair2; //record the pair of Hand 2

	if (h1.cardHand.at(start).rank == h1.cardHand.at(start + 1).rank) { // find pair1, third1, fourth1, fifth1
		pair1 = h1.cardHand.at(start).rank;
		third1 = h1.cardHand.at(mid).rank;
		fourth1 = h1.cardHand.at(mid + 1).rank;
		fifth1 = h1.cardHand.at(last).rank;
	}
	else if (h1.cardHand.at(start + 1).rank == h1.cardHand.at(mid).rank) {
		pair1 = h1.cardHand.at(mid).rank;
		third1 = h1.cardHand.at(start).rank;
		fourth1 = h1.cardHand.at(mid + 1).rank;
		fifth1 = h1.cardHand.at(last).rank;
	}
	else if (h1.cardHand.at(mid).rank == h1.cardHand.at(mid + 1).rank) {
		pair1 = h1.cardHand.at(mid).rank;
		third1 = h1.cardHand.at(start).rank;
		fourth1 = h1.cardHand.at(start + 1).rank;
		fifth1 = h1.cardHand.at(last).rank;
	}
	else {
		pair1 = h1.cardHand.at(last).rank;
		third1 = h1.cardHand.at(start).rank;
		fourth1 = h1.cardHand.at(start + 1).rank;
		fifth1 = h1.cardHand.at(mid).rank;
	}

	if (h2.cardHand.at(start).rank == h2.cardHand.at(start + 1).rank) { //find pair2, third2, fourth2, fifth2
		pair2 = h2.cardHand.at(start).rank;
		third2 = h2.cardHand.at(mid).rank;
		fourth2 = h2.cardHand.at(mid + 1).rank;
		fifth2 = h2.cardHand.at(last).rank;
	}
	else if (h2.cardHand.at(start + 1).rank == h2.cardHand.at(mid).rank) {
		pair2 = h2.cardHand.at(mid).rank;
		third2 = h2.cardHand.at(start).rank;
		fourth2 = h2.cardHand.at(mid + 1).rank;
		fifth2 = h2.cardHand.at(last).rank;
	}
	else if (h2.cardHand.at(mid).rank == h2.cardHand.at(mid + 1).rank) {
		pair2 = h2.cardHand.at(mid).rank;
		third2 = h2.cardHand.at(start).rank;
		fourth2 = h2.cardHand.at(start + 1).rank;
		fifth2 = h2.cardHand.at(last).rank;
	}
	else {
		pair2 = h2.cardHand.at(last).rank;
		third2 = h2.cardHand.at(start).rank;
		fourth2 = h2.cardHand.at(start + 1).rank;
		fifth2 = h2.cardHand.at(mid).rank;
	}

	if (pair1 > pair2) { //compare the pair first
		return true;
	}
	else if (pair1 < pair2) {
		return false;
	}
	else { //if equal, compare the highest single card of two hands
		if (fifth1 > fifth2) {
			return true;
		}
		else if (fifth1 < fifth2) {
			return false;
		}
		else { //if equal, compare the second highest single card of two hands
			if (fourth1 > fourth2) {
				return true;
			}
			else if (fourth1 < fourth2) {
				return false;
			}
			else { //if equal, compare the smallest single card of two hands
				return (third1 > third2);
			}
		}
	}
}