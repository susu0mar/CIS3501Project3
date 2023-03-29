#include "hashNode.h"
#include "linearHashTable.h"
#include "chainedHashTable.h"
#include<iomanip>

//TODO: have metrics print to output then do testing and documentation!
//DELETE

//EVERYTHING SEEMS TO WORK SO FAR, NOW NEED TO PRINT OUT METRICS AND CHECK TO MAKE SURE THEY'RE OKAY! (HAMDELLA GOOD PROGRESS I THING :D)

int main() {

	//all metric variables for LINEAR open addressing
	string testCase;  //holds test case name
	int ans; //holds answer from user to decide if to use random num or file num
	int ans2 =0;
	string inputFileName; //holds input file name
	string outputFileName; //holds output file name

	//srand(time(0));  //THIS IS TO SEED RANDOM NUMBER GENERATOR, KEEPING IT COMMENTED OUT FOR TESTING PURPOSES BUT WILL UNCOMMENT AFTER

	ifstream inputFile;
	ofstream outputFile;

	//in linear insert method
	int numKeysLinear =0;
	int numDuplicateKeysLinear=0;
	int numUniqueKeysLinear=0;
	int totCollisions = 0;
	int totDistanceHome = 0;
	int numDirectInsert = 0;
	int numIndirectInsert = 0;
	int largestDistanceLinear = 0;

	//in linear search method
	int numComparisons = 0; //This is counts TOTAL comparisons done in the whole table (does not output for each individual input)
	int numDirectAccess = 0;
	int numIndirectAccess = 0; //If key not found, then it is NEITHER direct or indirect
	int largestComparison = 0;
	int numSearchesLinear = 0;

	//all metric variables for Chain Overflow

	//in chain insert method
	int CtotalDistanceHome =0;
	int numKeysChain = 0;
	int numDuplicateKeysChain = 0;
	int numUniqueKeysChain = 0;
	int totCollisionsChain = 0;
	int numDirectInsertChain = 0;
	int numIndirectInsertChain = 0;
	int largestDistanceChain = 0;


	//in chain search method
	int numComparisonsChain = 0;
	int numDirectAccessChain = 0;
	int numIndirectAccessChain = 0;
	int largestComparisonChain = 0;
	int numSearchesChain = 0;
	//HASH TABLES
	chainedHashTable chainedTable; //chained overflow instance
	linearHashTable linearTable; //Linear open addressing instance


	//SEARCH "QUEUE" aka vector
	vector<int> searchQueue;

	cout << "WELCOME TO THE HASH TABLE PROGRAM :D" << endl;
	cout << "PLEASE ENTER TEST CASE NAME" << endl;
	getline(cin, testCase);
	cout << "WOULD YOU LIKE TO USE AN INSERT FILE OR A RANDOM NUMBER GENERATOR (ANSWER 1 = INSERT FILE, ANSWER 2 = RANDOM)" << endl;

	cin >> ans; 

	//USING AN INPUT FILE
	if (ans == 1) {

		cout << "Enter input file name" << endl;

		cin >> inputFileName;

		inputFileName = inputFileName + ".txt";

		cout << "Enter output file name" << endl;

		cin >> outputFileName;

		outputFileName = outputFileName + ".txt";



		inputFile.open(inputFileName); //open input file 
		outputFile.open(outputFileName); //open output file

		if (!inputFile.is_open()) {

			//terminate program if file fails to open

			cout << "Unable to open File" << endl;

			return 1;
		}

		if (!outputFile.is_open()) {

			//terminate program if file fails to open

			cout << "Unable to open output File" << endl;

			return 1;

		}

		//If initial file is empty, no inserts are done, A message is output to user
		if(inputFile.peek() == EOF){
		
			cout << "Empty File. Hash Table is empty" << endl;
			outputFile << "Empty File. Hash Table is empty" << endl;

		}
		else {
			//JUST DOING INSERTS
			int number; //use this for inserts

			//iterate through the inputFile and insert all values within
			while (inputFile >> number) {
			
				cout << "Inserting " << number << endl;
				outputFile<< "Inserting " << number << endl;

				//insert into the hash tables
				linearTable.insert(number, numKeysLinear, numDuplicateKeysLinear, numUniqueKeysLinear, totCollisions, totDistanceHome, numDirectInsert, numIndirectInsert, largestDistanceLinear);
				chainedTable.insert(number, numKeysChain, numDuplicateKeysChain, numUniqueKeysChain, totCollisionsChain, CtotalDistanceHome, numDirectInsertChain, numIndirectInsert, largestDistanceChain);

				//insert the number into the search queue (TO BE SEARCHED LATER)
				searchQueue.push_back(number);

			}
			

			//Now Search (WORKS!)
			for (int i = 0; i < searchQueue.size(); i++) {
				
				cout << "Searching " << searchQueue[i] << endl;
				outputFile<< "Searching " << searchQueue[i] << endl;

				//Searching linear table first
				if (linearTable.search(searchQueue[i], numComparisons, numDirectAccess, numIndirectAccess, largestComparison, numSearchesLinear)) {
				
					cout << searchQueue[i] << " Found in Linear Table!" << endl;
					outputFile<< searchQueue[i] << " Found in Linear Table!" << endl;
				}
				else {
					
					cout << searchQueue[i] << " Not Found in Linear Table!" << endl;
					outputFile << searchQueue[i] << " Not Found in Linear Table!" << endl;
				}
				
				//Searching chained overflow table second

				if (chainedTable.search(searchQueue[i], numComparisonsChain, numDirectAccessChain, numIndirectAccessChain, largestComparisonChain, numSearchesChain)) {
					
					cout << searchQueue[i] << " Found in Chain Table!" << endl;
					outputFile << searchQueue[i] << " Found in Chain Table!" << endl;
					
				}
				else {
				
					cout << searchQueue[i] << " Not Found in Chain Table!" << endl;
					outputFile << searchQueue[i] << " Not Found in Chain Table!" << endl;

				}
			}

		}
		
		
	}
	
	else if (ans == 2) {
		cout << "Please choose between a linear hash table or a chained hash table (Linear = 1, Chained = 2): " << endl;
		cin >> ans2;

		int number;

		//For random number generator, it will continue to fill the hash tables until they are full(overflow is full or main linear table is full)

		if (ans2 == 1) {
			while (!linearTable.isFull(numUniqueKeysLinear)) {

				number = rand() % 10000; //generate random number from 0 to 9999

				cout << "Inserting " << number << endl;
				outputFile << "Inserting " << number << endl;


				//then insert into linear table
				linearTable.insert(number, numKeysLinear, numDuplicateKeysLinear, numUniqueKeysLinear, totCollisions, totDistanceHome, numDirectInsert, numIndirectInsert, largestDistanceLinear);

				//insert the number into search queue
				searchQueue.push_back(number);
				
			}
			

			//Now search
			for (int i = 0; i < searchQueue.size(); i++) {
				
				cout << "Searching " << searchQueue[i] << endl;
				outputFile << "Searching " << searchQueue[i] << endl;

				//Searching linear table (ONLY)
				if (linearTable.search(searchQueue[i], numComparisons, numDirectAccess, numIndirectAccess, largestComparison, numSearchesLinear)) {

					cout << searchQueue[i] << " Found in Linear Table!" << endl;
					outputFile << searchQueue[i] << " Found in Linear Table!" << endl;
				}
				else {

					cout << searchQueue[i] << " Not Found in Linear Table!" << endl;
					outputFile << searchQueue[i] << " Not Found in Linear Table!" << endl;
				}
				
			
			}

		}
		
		else if (ans2 == 2) {

			while (!chainedTable.isOverflowFull()) {
			
				number = rand() % 10000; //generate random number from 0 to 9999

				cout << "Inserting " << number << endl;
				outputFile << "Inserting " << number << endl;

				//then insert into chained overflow hash table
				chainedTable.insert(number, numKeysChain, numDuplicateKeysChain, numUniqueKeysChain, totCollisionsChain, CtotalDistanceHome, numDirectInsertChain, numIndirectInsert,largestDistanceChain);
			
				//insert the number into search queue
				searchQueue.push_back(number);

			}

			

			//Now Search!!
			for (int i = 0; i < searchQueue.size(); i++) {
			
				cout << "Searching " << searchQueue[i] << endl;
				outputFile << "Searching " << searchQueue[i] << endl;

				//Searching chained overflow table (ONLY)

				if (chainedTable.search(searchQueue[i], numComparisonsChain, numDirectAccessChain, numIndirectAccessChain, largestComparisonChain, numSearchesChain)) {

					cout << searchQueue[i] << " Found in Chain Table!" << endl;
					outputFile << searchQueue[i] << " Found in Chain Table!" << endl;

				}
				else {

					cout << searchQueue[i] << " Not Found in Chain Table!" << endl;
					outputFile << searchQueue[i] << " Not Found in Chain Table!" << endl;

				}
			
			}
		}
		else {

			cout << "Invalid answer. Ending Program" << endl;
			outputFile<< "Invalid answer. Ending Program" << endl;

			return 1;
		}
		
	}
	else { cout << "Invalid input. Ending program" << endl; return 1; }

	cout << endl<<"Linear Hash Table" << endl << endl;
	outputFile<<endl<< "Linear Hash Table" << endl << endl;

	linearTable.printHashTable(outputFile);

	cout << endl<<endl<<"Chained Overflow Hash Table" << endl << endl;
	outputFile<< endl<<endl<<"Chained Overflow Hash Table" << endl << endl;

	chainedTable.printHashTable(outputFile);


	//now print out metrics (to console and outputFile)
	cout << endl<<endl <<"METRICS FOR TEST: " << testCase << endl << endl;
	cout << "\t\t" << "\tLinear\t\t" << "Chained" << endl;

	//TOTAL NUMBER KEY VALUES INSERTED
	cout << "Total Keys Inserted:\t" << numKeysLinear << "\t\t" << numKeysChain << endl;
	//Unique Values
	cout << "Unique Values:\t\t" << numUniqueKeysLinear << "\t\t" << numUniqueKeysChain << endl;
	//Duplicate Values
	cout << "Duplicate Values:\t" << numDuplicateKeysLinear << "\t\t" << numDuplicateKeysChain << endl;
	cout << endl << endl;
	//Collisions
	cout << "Collisions" << endl;
	cout << "Number Collisions:\t" << totCollisions << "\t\t" << totCollisionsChain << endl;
	//TOTAL distance from home bucket 
	cout << "Distance from Home:\t" << totDistanceHome << "\t\t" << CtotalDistanceHome << endl;
	//Total direct inserts
	cout << "Direct Inserts:\t\t" << numDirectInsert<<" "<< fixed<<setprecision(3)<< static_cast<float>(numDirectInsert)/numUniqueKeysLinear << "%\t" << numDirectInsertChain<< " " << static_cast<float>(numDirectInsertChain) / numUniqueKeysChain <<"%" << endl; //used unique keys for precentage because direct and indirect DID NOT increment for duplicates!!
	//Total inDirect inserts
	cout << "Indirect Inserts:\t" << numIndirectInsert << " " << fixed << setprecision(3) << static_cast<float>( numIndirectInsert) / numUniqueKeysLinear << "%\t" << numIndirectInsertChain << " " << static_cast<float>(numIndirectInsertChain) / numUniqueKeysChain << "%" << endl;
	//average distance from home
	cout << endl;
	cout << "Average distance from Home" << endl;
	//AVERAGE DISTANCE INCLUDING DIRECT INSERTS!
	cout << "Including Direct Inserts:\t" << fixed << setprecision(3) << static_cast<float>(totDistanceHome) / numUniqueKeysLinear<< "\t\t"<< static_cast<float>(CtotalDistanceHome)/numUniqueKeysChain<<endl;
	//AVERAGE DISTANCE EXCLUDING DIRECT INSERTS
	cout << "Excluding Direct Inserts:\t";
	if (numUniqueKeysLinear <= numDirectInsert) {
		//all keys inserted directly into homebucket
		cout << 0 << "\t\t";
	}
	else {
	
		cout << fixed << setprecision(3)<< static_cast<float>(totDistanceHome) / (numUniqueKeysLinear - numDirectInsert) << "\t\t";
	}
	if (numUniqueKeysChain <= numDirectInsertChain) {
		//all keys inserted directly into homebucket
		cout << 0 << endl;
	}
	else {
	
		cout << fixed << setprecision(3)<< static_cast<float>(CtotalDistanceHome) / (numUniqueKeysChain - numDirectInsertChain) << endl;
	}
	//LARGEST DISTANCE!
	cout << "Largest Distance:\t\t" << largestDistanceLinear << "\t" << largestDistanceChain << endl;
	cout << endl << endl;
	//Searches
	cout << "Searches" << endl;
	//total searches
	cout << "Total Searches:\t\t\t" << numSearchesLinear <<"\t\t"<< numSearchesChain<<endl;
	//comparisons
	cout << "Total Number of Comparisons:\t" << numComparisons << "\t\t" << numComparisonsChain << endl;
	//num direct acccess
	cout << "Number of Direct Access:\t" << numDirectAccess<< " "<< fixed << setprecision(3) << static_cast<float>(numDirectAccess)/(numDirectAccess +numIndirectAccess) << "%\t" << numDirectAccessChain<< " "<< static_cast<float>(numDirectAccessChain)/(numDirectAccessChain+numIndirectAccessChain) <<"%" << endl;
	//num indirect access 
	cout << "Indirect Access" << endl;
	cout << "Number of Indirect Access:\t" << numIndirectAccess<<" "<< fixed << setprecision(3) << static_cast<float>(numIndirectAccess)/(numDirectAccess+ numIndirectAccess) << "%\t" << numIndirectAccessChain<< " "<< static_cast<float>(numIndirectAccessChain)/(numDirectAccessChain+ numIndirectAccessChain)<<"%" << endl;
	cout << "Number of Comparisons (Indirect): " << numComparisons - numDirectAccess << "\t\t" << numComparisonsChain - numDirectAccessChain << endl;
	//average comparisons
	//used the if statmenets because in random generator only 1 hash table is used, so it would divide by 0 for the table not uses
	if (ans2 == 1) { cout << "Average Comparison:\t\t" << numComparisons / numSearchesLinear << "\t\t" << 0 << endl; }
	else if(ans2==2){ cout << "Average Comparison:\t\t" << 0 << "\t\t" << numComparisonsChain / numSearchesChain << endl; }
	else { cout << "Average Comparison:\t\t" << 0 << "\t\t" << 0 << endl; }
	//cout << "Average Comparison:\t" << numComparisons / numSearchesLinear << "\t" << numComparisonsChain / numSearchesChain << endl;
	cout << "Largest Comparison:\t\t" << largestComparison << "\t\t" << largestComparisonChain << endl;

	cout << "Done! :D" << endl;







	//SAME THING TO OUTPUT FILE!!!!!!!!!!

	outputFile<< endl<<endl<<"METRICS FOR TEST: " << testCase << endl << endl;

	outputFile << "\t\t" << "\tLinear\t\t" << "Chained" << endl;

	//TOTAL NUMBER KEY VALUES INSERTED
	outputFile << "Total Keys Inserted:\t" << numKeysLinear << "\t\t" << numKeysChain << endl;
	//Unique Values
	outputFile << "Unique Values:\t\t" << numUniqueKeysLinear << "\t\t" << numUniqueKeysChain << endl;
	//Duplicate Values
	outputFile << "Duplicate Values:\t" << numDuplicateKeysLinear << "\t\t" << numDuplicateKeysChain << endl;
	outputFile << endl << endl;
	//Collisions
	outputFile << "Collisions" << endl;
	outputFile << "Number Collisions:\t" << totCollisions << "\t\t" << totCollisionsChain << endl;
	//TOTAL distance from home bucket 
	outputFile << "Distance from Home:\t" << totDistanceHome << "\t\t" << CtotalDistanceHome << endl;
	//Total direct inserts
	outputFile << "Direct Inserts:\t\t" << numDirectInsert << " " << fixed << setprecision(3) << static_cast<float>(numDirectInsert) / numUniqueKeysLinear << "%\t" << numDirectInsertChain << " " << static_cast<float>(numDirectInsertChain) / numUniqueKeysChain << "%" << endl; //used unique keys for precentage because direct and indirect DID NOT increment for duplicates!!
	//Total inDirect inserts
	outputFile << "Indirect Inserts:\t" << numIndirectInsert << " " << fixed << setprecision(3) << static_cast<float>(numIndirectInsert) / numUniqueKeysLinear << "%\t" << numIndirectInsertChain << " " << static_cast<float>(numIndirectInsertChain) / numUniqueKeysChain << "%" << endl;
	//average distance from home
	outputFile << endl;
	outputFile << "Average distance from Home" << endl;
	//AVERAGE DISTANCE INCLUDING DIRECT INSERTS!
	outputFile << "Including Direct Inserts:\t" << fixed << setprecision(3) << static_cast<float>(totDistanceHome) / numUniqueKeysLinear << "\t\t" << static_cast<float>(CtotalDistanceHome) / numUniqueKeysChain << endl;
	//AVERAGE DISTANCE EXCLUDING DIRECT INSERTS
	outputFile << "Excluding Direct Inserts:\t";
	if (numUniqueKeysLinear <= numDirectInsert) {
		//all keys inserted directly into homebucket
		outputFile << 0 << "\t\t";
	}
	else {

		outputFile << fixed << setprecision(3) << static_cast<float>(totDistanceHome) / (numUniqueKeysLinear - numDirectInsert) << "\t\t";
	}
	if (numUniqueKeysChain <= numDirectInsertChain) {
		//all keys inserted directly into homebucket
		outputFile << 0 << endl;
	}
	else {

		outputFile << fixed << setprecision(3) << static_cast<float>(CtotalDistanceHome) / (numUniqueKeysChain - numDirectInsertChain) << endl;
	}
	//LARGEST DISTANCE!
	outputFile << "Largest Distance:\t\t" << largestDistanceLinear << "\t" << largestDistanceChain << endl;
	outputFile << endl << endl;
	//Searches
	outputFile << "Searches" << endl;
	//total searches
	outputFile << "Total Searches:\t\t\t" << numSearchesLinear << "\t\t" << numSearchesChain << endl;
	//comparisons
	outputFile << "Total Number of Comparisons:\t" << numComparisons << "\t\t" << numComparisonsChain << endl;
	//num direct acccess
	outputFile << "Number of Direct Access:\t" << numDirectAccess << " " << fixed << setprecision(3) << static_cast<float>(numDirectAccess) / (numDirectAccess + numIndirectAccess) << "%\t" << numDirectAccessChain << " " << static_cast<float>(numDirectAccessChain) / (numDirectAccessChain + numIndirectAccessChain) << "%" << endl;
	//num indirect access 
	outputFile << "Indirect Access" << endl;
	outputFile << "Number of Indirect Access:\t" << numIndirectAccess << " " << fixed << setprecision(3) << static_cast<float>(numIndirectAccess) / (numDirectAccess + numIndirectAccess) << "%\t" << numIndirectAccessChain << " " << static_cast<float>(numIndirectAccessChain) / (numDirectAccessChain + numIndirectAccessChain) << "%" << endl;
	outputFile << "Number of Comparisons (Indirect): " << numComparisons - numDirectAccess << "\t\t" << numComparisonsChain - numDirectAccessChain << endl;
	//average comparisons
	//used the if statmenets because in random generator only 1 hash table is used, so it would divide by 0 for the table not uses
	if (ans2 == 1) { outputFile << "Average Comparison:\t\t" << numComparisons / numSearchesLinear << "\t\t" << 0 << endl; }
	else if (ans2 == 2) { outputFile << "Average Comparison:\t\t" << 0 << "\t\t" << numComparisonsChain / numSearchesChain << endl; }
	else { outputFile << "Average Comparison:\t\t" << 0 << "\t\t" << 0 << endl; }
	//cout << "Average Comparison:\t" << numComparisons / numSearchesLinear << "\t" << numComparisonsChain / numSearchesChain << endl;
	outputFile << "Largest Comparison:\t\t" << largestComparison << "\t\t" << largestComparisonChain << endl;

	outputFile << "Done! :D" << endl;






	return 0;
}