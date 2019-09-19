// ReturnType.cpp Lab4
//
// Author: 
// Jianxing(Jason) Liao   ID: 457819  Email: liaojianxing@wustl.edu
// Bobby Chen   ID: 451980  Email: chen.jiazhi@wustl.edu
// Yipeng Wang   ID: 458204  Email: wang.yipeng@wustl.edu
//
// Purpose: Define usageMessage function used to print out instructions while facing errors.

#include "stdafx.h"
#include "ReturnType.h"
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

void usageMessage(char *program, string messageContent) { //print out error message to users
	cout << program << " " << messageContent << endl;
}