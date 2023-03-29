#pragma once
#include "hashNode.h"

//class for chained overflow hash table
class chainedHashTable
{
private:
	hashNode table[TABLE_SIZE]; //for main table
	hashNode overflow[OVERFLOW_SIZE]; //for overflow table
	int nextFreeNode; //index of next free slot in overflow array


public:

	//default constructor
	chainedHashTable() {
		
		//initialize all values of main and overflow table
		for (int i = 0; i < TABLE_SIZE; i++) {
			table[i].keyValue = -1;
			table[i].keyCount = 0;
			table[i].chainIndex = -1;
			
		
		}
		for (int i = 0; i < OVERFLOW_SIZE; i++) {
		
			overflow[i].keyValue = -1;
			overflow[i].keyCount = 0;
			overflow[i].chainIndex = -1;
		
		}
	
		nextFreeNode = 0;
	}

	int hashFunction(int key) { return key % TABLE_SIZE; }

	
	// I THINK TOTAL DISTANCE HOME WORKS!
	//for distance from homebucket will count how far from orginal overflow index (ex: if not inserted in main table, then +1 distance, then every subsequent visit from that original chain index is +1)
	void insert(int key,int& numKeys, int& numDuplicatekeys, int& numUniquekeys, int& Collisions ,int& totalDistanceHome, int& numDirectInsert, int& numIndirectInsert, int& largestDistance) {
		
		int index = hashFunction(key);
		int distance = 0;
		numKeys++;

		//maybe if table is full (like have a separate method to check if overflow array is full) then do numKeys-- and maybe exit insert method

		if (table[index].keyValue == -1) { //spot is empty so insert!
			
			table[index].keyValue = key;
			table[index].keyCount = 1;
			numDirectInsert++;
			numUniquekeys++;
		
		}
		else if (table[index].keyValue == key) {
			//if the key in able is equal to key to be inserted

			table[index].keyCount++;
			numDuplicatekeys++;

			//not going to consider duplicates a direct/indirect insert
		}
		else {
			//slot in main table is occupied, check overflow table
			//Collisions++;
			int chainIndex = table[index].chainIndex; //have local chain index variable
			distance = 1;
			while (chainIndex != -1) { //THIS LOOP MAY BE INFINITE!!!! 
			
				//if key already in overflor
				if (overflow[chainIndex].keyValue == key) {
					
					overflow[chainIndex].keyCount++;
					numDuplicatekeys++;
					totalDistanceHome += distance; //even if duplicate, its distance still contributes to the total
					if (distance > largestDistance) {
						//change largest if distance is larger(for current insert)
						largestDistance = distance;
					}
					return;
				
				}
				//continue traversing chain to find it
				chainIndex = overflow[chainIndex].chainIndex;
				distance++;
			}
			
			//key not found in main table or overflow table, insert to overflow

			if (nextFreeNode >= OVERFLOW_SIZE) {
			
				cout << "Overflow table full!" << endl;
				//i think i should do numKeys -- because key isn't inserted
				return;
			}
			

			overflow[nextFreeNode].keyValue = key;
			overflow[nextFreeNode].keyCount = 1;
			overflow[nextFreeNode].chainIndex = -1;
			//table[index].chainIndex = nextFreeNode;

			//trying to figure this out (CHAIN INDEX NOT CORRECT SOMETIMES) YAY IT WORKS NOW, MAY NEED TO ADDRESS POSSIBLE OVERFLOW!!!
			numIndirectInsert++;
			if (table[index].chainIndex == -1) {
				table[index].chainIndex = nextFreeNode;
				
				Collisions++;
			}
			else {
				int prevChainIndex = table[index].chainIndex;

				while (overflow[prevChainIndex].chainIndex != -1) {
					prevChainIndex = overflow[prevChainIndex].chainIndex;

					//distance++;
				}
				
				overflow[prevChainIndex].chainIndex = nextFreeNode;
				//distance++;
				
				Collisions++;
			}
			nextFreeNode++;
			numUniquekeys++;
		}

		totalDistanceHome += distance;
		if (distance > largestDistance) {
			//change largest if distance is larger(for current insert)
			largestDistance = distance;
		}
	}


	void printHashTable(ofstream& outputFile) {
	
		cout << "Main Table: " << endl;
		outputFile << "Main Table: " << endl;
		for (int i = 0; i < TABLE_SIZE; i++) {
		
			cout << "Index: "<< i <<":";
			outputFile << "Index: " << i << ":";

			if (table[i].keyValue == -1) {

				cout << "EMPTY" << endl;
				outputFile << "EMPTY" << endl;
			}
			else {

				cout << " Key: " << table[i].keyValue << "  Key Count:  " << table[i].keyCount <<" Chain Index: "<< table[i].chainIndex<< endl;
				outputFile << " Key: " << table[i].keyValue << "  Key Count:  " << table[i].keyCount << " Chain Index: " << table[i].chainIndex << endl;
			}



		}
		
		cout << endl << endl;
		outputFile << endl << endl;

		cout << "Overflow Table: " << endl;
		outputFile << "Overflow Table: " << endl;
		for (int i = 0; i < OVERFLOW_SIZE; i++) {
		
			cout << "Index: " << i << ":";
			outputFile << "Index: " << i << ":";

			if (overflow[i].keyValue == -1) {
			
				cout << "EMPTY" << endl;
				outputFile << "EMPTY" << endl;
			}
			else {
			
				cout << " Key: " << overflow[i].keyValue << " Key Count:  " << overflow[i].keyCount << " Chain Index: " << overflow[i].chainIndex << endl;
				outputFile << " Key: " << overflow[i].keyValue << " Key Count:  " << overflow[i].keyCount << " Chain Index: " << overflow[i].chainIndex << endl;
			}
		}
	}



	
	bool search(int key, int& numComparisons, int& numDirectAccess, int& numIndirectAccess, int& largestComparisons, int& totSearches) {
	
		int index = hashFunction(key);
		int chainIndex = table[index].chainIndex; //use this to traverse overflow table (if necessary)
		int ComparisonsLocal = 1; //to count local comparisons, at end it will be added to total comparisons //MIGHT DELETE!
		int numIterations = 0; //count number of iterations to terminate while loop so it isnt infinite!!!!
		totSearches++;

		if (table[index].keyValue == key) { //if key is in main table
			
			numComparisons += ComparisonsLocal;
			numDirectAccess++;
			return true;

		}
		
		//Traverse overflow chain 
		while (chainIndex != -1 && numIterations < OVERFLOW_SIZE) { //MAY BE INFINITE, ADDED ITERATION COUNTER!
			ComparisonsLocal++;
			if (overflow[chainIndex].keyValue == key) {
				numComparisons += ComparisonsLocal;
				numIndirectAccess++;
				if (ComparisonsLocal > largestComparisons) {
					largestComparisons = ComparisonsLocal;
				}
				return true;
			}

			chainIndex = overflow[chainIndex].chainIndex;
			numIterations++;

		}


		//key is not found :(
		numComparisons += ComparisonsLocal;
		//update largest if necessary
		if (ComparisonsLocal > largestComparisons) {
			largestComparisons = ComparisonsLocal;
		}
		return false;
	}

	//USE THIS TO DETERMINE IF OVERFLOW IS FULL (FOR RANDOM NUM GENERATOR)
	
	bool isOverflowFull() {
		
		return nextFreeNode >= OVERFLOW_SIZE;
	}
	


};

