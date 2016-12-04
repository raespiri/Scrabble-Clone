#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib> 
#include <algorithm>
#include <ctype.h>
#include "Hashtable.h"
using namespace std;

int main(int argc, char* argv[]) {
	/*string str = "predictable";
	stringstream ss; 
	//converting letters to int values
	for(int i = 0; i < (int) str.length(); i++) {
		int temp = str[i] - 97; //temp int to hold each letter to int conversion 
		ss << temp; //add temp to stringstream
		ss << " "; //add spacing between ints
	}
	int letters[30] = {}; //array of int representation of word
	for(int j = 30-((int)str.length()); j < 30; j++) {
		ss >> letters[j]; //outputing ss contents to each index of letters
	}
	for(int x = 0; x < 30; x++) {
		cout << letters[x] << " ";
		if(x == 5 || x == 11 || x == 17 || x == 23 || x == 29) {
			cout << endl;
		}
	}*/

	string data = "'908976857645!@#@#$#@##leT128937927's'";
	transform(data.begin(), data.end(), data.begin(), ::tolower);
	for(int i = 0; (int) i < data.size(); i++) {
		if(!islower(data[i])) {
			data.erase(i,1);
			i--;
		}
	}
	cout << data << endl;

	return 0;
}