/*
 * Dictionary.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <string>
#include <map>
#include <set> 

class Dictionary {
public:
	Dictionary (std::string dictionary_file_name);

	// what else will this need?
	bool validWord(std::string word);
	bool validPrefix(std::string prefix);

private:
	std::map<std::string, int> dict; //map to hold dictionary words
	std::set<std::string> prefixes; //set to hold dictionary word prefixes 
};


#endif /* DICTIONARY_H_ */
