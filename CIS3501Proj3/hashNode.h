#pragma once

#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdlib>
#include<ctime> //use this to seed random number generator!

using namespace std;

const int TABLE_SIZE = 100; //global variable for main table size //TODO: MAY HAVE SEPARATE VARIABLES FOR THE MAIN TABLES FOR CHAINED AND LINEAR

const int OVERFLOW_SIZE = 30; //global variable for overflow array size

//struct for a node in a hash table
struct hashNode {

	int keyValue;
	int keyCount;
	int chainIndex; //-1 indicates not used or end of chain

};