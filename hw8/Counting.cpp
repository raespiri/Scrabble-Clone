#include <fstream>
#include "Hashtable.h"
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 6){
    	cout << "Please provide valid command line arguments" << endl;
    	return 1;
  	}

  	clock_t start; //clock to time hashtable
    double duration; //duration of program
	
	// PREPROCESSING
	ifstream input(argv[1]); //getting input file
  	ofstream output(argv[2]); //getting output file
	int x = atoi(argv[3]); //int to determine which type of collision avoidance to use
	int d = atoi(argv[4]); //int to determine debug mode flag
	int r = atoi(argv[5]); //int to determine how often to repeat program
	
	bool debug;
	if(d == 0) {
		debug = false; //debug mode false if d is 0
	}
	else {
		debug = true; //debug mode true if d is 1
	}
	
	vector<string> words; //vector of words from input file
	string curr; //string to hold current line in input file
  	
  	while(getline(input, curr)) { //loop through lines of the input file
  		stringstream ss;
	  	ss << curr; //parse current line through stringstream
	  	string word;
	  	while(ss >> word) { //loop to grab a word from the current line from ss
	  		transform(word.begin(), word.end(), word.begin(), ::tolower); //convert word to lower
			for(int i = 0; i < (int) word.size(); i++) { //loop through word
				if(!islower(word[i])) { //if char is not lowercase
					word.erase(i,1); //erase it
					i--; //decrement i to account for a removal
				}
			}
			if(word != " " && word != "") { //if resulting word is not the empty string
				words.push_back(word); //add new word to vector
			}
		}
  	}

  	// START CLOCK
    start = clock();

    for(int j = 0; j <= r; j++) { //loop program for determined number of repeats
    	Hashtable htable(debug, x); //create a new hashtable

    	for(int y = 0; y < (int) words.size(); y++) { //loop through vector of words
    		htable.add(words[y]); //add word to hashtable
    	} 
    	htable.reportAll(output); //print table to output file
    }

    duration = (clock() - start) / (double) CLOCKS_PER_SEC; //record duration of the program

	cout << duration/(r+1) << endl; //print duration 

	return 0;
}