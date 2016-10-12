/*
 * Dictionary.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "Dictionary.h"

using namespace std;

Dictionary::Dictionary (string dictionary_file_name)
{
	ifstream dictFile (dictionary_file_name.c_str());
	string word;

	if (dictFile.is_open())
	{
		int dict_index = 1; //starting index for dictionary words;
		while (getline (dictFile, word))
		{			
			// What do you want to do with the word?
			dict.insert(make_pair(word, dict_index)); //insert word to map
			dict_index++; //move to next index in dictionary
		}
		dictFile.close ();
	}
	else throw invalid_argument("Cannot open file: " + dictionary_file_name);
}

bool Dictionary::validWord(string word) {
	map<string, int>::iterator it; //iterator for dictionary
	it = dict.find(word); //try to find word in dictionary
	if(it == dict.end()) { 
		return false; //false if not found
	}
	else {
		return true; //true if found
	}

}
