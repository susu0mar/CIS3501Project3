#pragma once
#include "hashNode.h"

//class for linear open addressing hash table
class linearHashTable
{
private:
	hashNode table[TABLE_SIZE]; //creating array for hash table //size based on global variable TABLE_SIZE

public:

	//default constructor
	linearHashTable() {
		
		for (int i = 0; i < TABLE_SIZE; i++) {
			
			table[i].keyValue = -1; //-1 indicates empty spot
			table[i].keyCount = 0;
			table[i].chainIndex = -1; //-1 indicates not used or end of chain
		
		}
		
	}

	//method for hash function
	int hashFunction(int key) { return key % TABLE_SIZE; }



	//TODO? I THINK I HAVE ALL METRICS DONE!
	void insert(int key, int& numKeys, int& numDuplicateKeys, int& numUniqueKeys, int& Collisions, int& totalDistanceHome, int& numDirectInsert, int& numIndirectInsert, int& largestDistance ) {
		
		int index = hashFunction(key); //index to hold hash address of the value
		int distance = 0;
		int localCollision = 0; //use this var to determine when to stop loop (PREVENT INFINITE LOOP)

		while (table[index].keyValue != -1) { //TODO: MAY NEED TO ADD AN EXTRA CONDITION TO PREVENT INFINITE LOOP!!!
			if (table[index].keyValue == key) { //if keys are the same
				table[index].keyCount++; //increment key count

				numDuplicateKeys++; //increment TOTAL DUPLICATES IN TABLE
				numKeys++; //increment TOTAL KEYS IN TABLE
				totalDistanceHome+=distance;
				if (distance > largestDistance) { largestDistance = distance; }
				
				return;
			}
			distance++;
			index = hashFunction(index + 1); //linear probing
			Collisions++;
			localCollision++;

			//THIS WORKS :D
			if (localCollision >= TABLE_SIZE) { //MORE THAN 50% COLLISIONS, ASSUME TABLE IS FULL AND DON'T INSERT IT!! THIS PREVENTS INFINITE LOOPS WHEN TABLE IS FULL!!!
				//cout << "Unable to insert " << key << ". Table is most likely full" << endl;
				return;
			}

		}
		/*if (numUniqueKeys == TABLE_SIZE) {

			cout << "Table Full, cannot insert " << key << endl << endl;
			return;
		}*/
		table[index].keyValue = key;
		table[index].keyCount = 1;
		totalDistanceHome += distance;
		if (distance == 0) {
			numDirectInsert++;
		}
		else {
			numIndirectInsert++;
		}
		numKeys++;
		numUniqueKeys++; //increment number of unique keys (FIRST INSTANCE CONSIDERED UNIQUE, OTHER INSTANCES AFTER FIRST INSERT IS CONSIDERED DUPLICATE)
		//check if distance from home bucket is greater than the current largest (if so change largest to be distance)
		if (distance > largestDistance) { largestDistance = distance; }
	
	}

	//this works so far :D
	//TODO: NEED TO PRINT TO OUTPUT FILE ASWELL!
	void printHashTable(ofstream& outputFile) {
		
		for (int i = 0; i < TABLE_SIZE; i++) {
			
			cout << "Index" << i << ":";
			outputFile << "Index" << i << ":";

			if (table[i].keyValue == -1) {
				
				cout << "EMPTY" << endl;
				outputFile << "EMPTY" << endl;
			}
			else {
				
				cout << " Key: " << table[i].keyValue << "  Key Count:  " << table[i].keyCount  << endl;
				outputFile << " Key: " << table[i].keyValue << "  Key Count:  " << table[i].keyCount << endl;
			}

		
		}
	
	
	}


	//Search method with metrics: number of comparisons(Total for whole table), number of direct accesses(TOTAL), num of indirect accesses (TOTAL), and largest comparison 
	bool search(int key, int& numComparisons, int& numDirectAccesses, int& numIndirectAccesses, int& largestComparison, int& totSearches) {
	
		int index = hashFunction(key);
		int comparisons = 0; //local variable which will eveuntally be added to the TOTAL COMPARISONS COUNT(for whole table)
		int numProbes = 0; //local variable used to prevent infinite loop! 
		 totSearches++;

		while (numProbes < TABLE_SIZE && table[index].keyValue != -1) { //the numProbes prevents infinite loop I THINK IT WORKS :D
		
			if (table[index].keyValue == key) {
			
				numComparisons += comparisons + 1; //Technically 1 comparison done (but local comparison isnt incremented for the purpose of the following if-else)

				if (comparisons == 0) { //no comparisons done means it was directly found!!
					
					numDirectAccesses++;
				}
				else { // 1 or more comparisons means that it needed to move from home bucket to find key
				
					numIndirectAccesses++;
				}
				if (comparisons + 1 > largestComparison) { largestComparison = comparisons + 1; }
				return true;
			}
			comparisons++; //increment comparisons
			index = hashFunction(index + 1); //linear prob index
			numProbes++;

		}

		//if not found...
		numComparisons += comparisons+1;
		
		//to find the largest comparison done!!!
		if (comparisons+1 > largestComparison) {
			
			largestComparison = comparisons+1;  
		}
		
		//NOTE: IF NOT FOUND THEN IT ISN'T CONSIDERED A DIRECT OR INDIRECT ACCESS 
		return false;
	}


	// USE THIS TO DETERMINE IF LINEAR ARRAY IF FULL (for random num generator loop)
	
	bool isFull(int& numUniqueKeys) {
	
		return numUniqueKeys >= TABLE_SIZE;
	}
};

