// ReturnType.h Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Header file. Declare enumeration for success and error return values. Declare usage message function.

#ifndef RETURNTYPE_H
#define RETURNTYPE_H

#include <cstring>
#include <string>
using namespace std;

enum ReturnType { success = 0, incorrectNumOfcmdArgument = 1, invalidFileName = 2, threecmdArgumentsWithoutShuffle = 3, twocmdArgumentsContainingShuffle = 4, notEnoughCardsGiven = 5, unknown = 6, indexOutOfBound = 7, instance_not_available = 8, game_already_started = 9, unknown_game = 10, no_game_in_progress = 11, already_playing = 12, emptyDecks = 13}; // defines enum ReturnType

void usageMessage(char *program, string messageContent);

#endif /* RETURNTYPE_H */