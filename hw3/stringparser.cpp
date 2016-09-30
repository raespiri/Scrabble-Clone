#include "stackstring.h"
#include "lliststring.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctype.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3){
    	cerr << "Please provide an input and output file." << endl;
    	return 1;
  	}
  	ifstream input(argv[1]);
  	ofstream output(argv[2]);

  	string expression; //current expression
  	while(getline(input, expression)) { //loop until there are no more lines
  		//malform checker
  		bool malformed  = false; //bool flag for malformed expressions
  		int open_count = 0; //count for closed parentheses
  		int closed_count = 0; //count for open parentheses 
  		int char_count = 0; //count to ensure at least one char is present
  		int expressionLen = expression.length();
  		for(int i = 0; i < expressionLen; i++) { //loop through entire expression
  			if(expression[i] == '(') {
  				open_count++; //when ( is found update count
  			}
  			else if(expression[i] == ')') {
  				closed_count++; //when ) is found update count
  			}
  			if(islower(expression[i])) {
  				char_count++;
  			}
  		}
  		if(open_count == 0 || closed_count == 0) { //a string is followed by a > or <
  			if(expression[expressionLen-1] == '>' || expression[expressionLen-1] == '<') {
  				malformed = true;
  			}
  		}
  		//check for whitespace between two letters
  		for(int i = 0; i < expressionLen-2; i++) {
  			if(islower(expression[i]) && islower(expression[i+2]) && expression[i+1] == ' ') {
  				malformed = true; //whitespace found in middle of string of letters
  			}
  		}
  		//check to ensure an expression with more than one word does not start with a lowercase letter
  		if((open_count > 0 || closed_count > 0) && islower(expression[0])) { 
  			malformed = true;
  		}
  		if(open_count != closed_count) { //malformed if unequal number of ( and )
  			malformed = true;
  		}
  		if(char_count == 0) { //no lowercase characters found in expression
  			malformed = true;
  		} 
  		StackString stack; //declaring stack
  		stringstream ss; 
  		ss << expression;
  		char c; //individual character holder
  		while(ss >> c) {
  			//if char is not a lowercase character	
  			if(!islower(c) && c != ')' && c != '(' && c != '+' && c != '-' && c != '>' && c != '<') { 
  				malformed = true; //malformed flag is true
  				break;
  			}
  			//converting char to string
			string s = "";
			s += c;
  			if(s != ")") { //if new character isn't a closing parentheses 
  				stack.push(s);
  			}
  			else if(s == ")") { //new charater is a closing parentheses
  				stringstream ss2;
  				while(stack.top() != "(") { //pop until (
  					if(stack.top() == "<") { //remove end operator appears
  						string temp;
  						temp = ss2.str();
  						if(temp.length() == 0) { //check for malformed expression with < at the end of a string
  							malformed = true; 
  							break;
  						}
  						int minus = -1; //place holder for index of + operator
  						int plus = -1; //place holder for index of - operator
  						int wordLen = temp.length();
  						for(int i = 0; i < wordLen; i++) { //loop through current string to find index of +,-
  							if(temp[i] == '+') {
  								plus = i; //update index of plus
  							}
  							else if(temp[i] == '-') {
  								minus = i; //update index of minus
  							}
  						}
  						if(minus == -1 && plus == -1) { //if no +,- operators are present
  							temp.erase(temp.begin()); //remove from end
  							stack.pop(); //now pop "<"
  							ss2.str(""); //clearing ss2
							ss2.clear(); //clearing any flags.
							ss2 << temp; //place temp back in ss2
  						}
  						else if(minus != -1) {
  							temp.erase(temp.begin()+minus+1); //removing from index right after operator
  							stack.pop(); //now pop "<"
  							ss2.str(""); //clearing ss2
							ss2.clear(); //clearing any flags.
							ss2 << temp; //place temp back in ss2
  						}
  						else if(plus != -1) {
  							temp.erase(temp.begin()+plus+1); //removing from index right after operator
  							stack.pop(); //now pop "<"
  							ss2.str(""); //clearing ss2
							ss2.clear(); //clearing any flags.
							ss2 << temp; //place temp back in ss2
  						}
  					}
  					else if(stack.top() == ">") { //remove start operator appears
  						string temp = ss2.str();
  						if(temp.length() == 0) { //check for malformed expression with > at the end of a string
  							malformed = true; 
  							break;
  						}
  						temp.erase(temp.end()-1); //erase last index
  						stack.pop(); //now pop ">"
  						ss2.str(""); //clearing ss2
						ss2.clear(); //clearing any flags.
						ss2 << temp; //place temp back in ss2
  					}
  					else { //no > or  < operators
  						ss2 << stack.top(); //place top in character in an ss
  						stack.pop(); //pop top character
  					}	
  				}
  				string word;
  				ss2 >> word; //current word/expression is in reveresed order
  				reverse(word.begin(), word.end()); //use reverse function to get it in correct 
  				int minus = -1; //place holder for location of minus sign
  				int plus = -1; //place holder for location of plus sign
  				int minus_counter = 0; //cpunter for number of -
  				int plus_counter = 0; //counter for number of +
  				int wordLen = word.length();
  				for(int i = 0; i < wordLen; i++) { //loop to find + or - sign locations
  					if(word[i] == '-') {
  						minus = i;
  						minus_counter++;
  						string left(word, 0, minus); //copy word left of minus sign
  						if(left.length() < 1) {
  							malformed = true; //subtracting from empty string
  						}
  						string right(word, minus+1, word.length()-minus-1); //copy word right of minus sign
  						if(right.length() > left.length()) { //if right word is longer than left word
	  						int rightLen = right.length();
	  						bool other_operators = false;
	  						for(int i = 0; i < rightLen; i++) { //loop to check if other operators exist in the expression
	  							if(right[i] == '-' || right[i] == '+') {
	  								other_operators = true;
	  								malformed = true; //no other operators besides one minus sign can exist within 2 parentheses
	  							}
	  						}
	  						if(other_operators == false) { //if no other operators exist in the expression
	  							word = left; //drop word to the right of the operator
	  						}
  						}
  						else {
  							int leftLen = left.length();
  							int rightLen = right.length();
  							bool erase = false; //bool to check if erasing occurred
  							for(int i = 0; i <= leftLen - rightLen; i++) { //looping through substrings of left
  								string temp(left, i, right.length()); //temp is a substring of left 
  								if(temp == right) { //compare temp to right word
  									word = left;
  									word.erase(i, right.length()); //remove temp substring from word
  									erase = true; //erasing occurred
  									break;
  								}
  							}
  							if(erase == false) { //if no substring was found erase remains false
  								word = left; //we keep the left word
  							}
  						}
  					}
  					else if(word[i] == '+') {
  						plus = i;
  						plus_counter++;
  						string left(word, 0, plus); //copy word left of plus sign
  						if(left.length() < 1) {
  							malformed = true; //subtracting from empty string
  						}
  						string right(word, plus+1, word.length()-plus-1); //copy word right of plus sign
  						word = left + right; //updated to contain concatenation of left and right words
  					}
  					wordLen = word.length(); //updating length variable	
  				}
  				if(minus_counter != 0 && plus_counter != 0) {
  					malformed = true; //MALFORMED if both + - are present within parentheses
  					break;
  				}
  				if(minus_counter > 1) {
  					malformed = true; //MALFORMED if more than one - sign, you need parentheses
  					break;
  				}
  				stack.pop(); //pop opening parentheses
  				int wordLen2 = word.length();
  				for(int i = 0; i < wordLen2; i++) {
  					string s = "";
  					s += word[i]; //converting letters of word to string
  					stack.push(s); //pushing simplified expression back onto stack
  				}
  			}
  		}
  		if(malformed) { //if malformed flag is true
  			output << "MALFORMED" << endl; //output Malformed
  		}
  		else {
  			string final; //final string
  			while(!stack.empty()) { //while stack isn't empty
  				final += stack.top(); //add each char to final
  				stack.pop(); //remove from stack
  			}
  			reverse(final.begin(), final.end()); //put final in right order
  			while(final[0] == '>' || final[0] == '<') { //checking for leftover > or < operators until first char is a letter
  				if(final[0] == '>') { //remove from start found
  					int finalLen = final.length();
  					for(int i = 0; i < finalLen; i++) { //looping through
  						if(final[i] != '>' && final[i] != '<') { //if a lowercase letter is found
  							final.erase(final.begin()+i); //erase it
  							final.erase(final.begin()); //erase operator from beginning of string
  							break;
  						}
  					}
  				}
  				else if(final[0] == '<') { //remove from end found
  					final.erase(final.end()-1); //erase it
  					final.erase(final.begin()); //erase operator from beginning of string
  				}
  			}
  			output << final << endl;
  		}
  	}

	return 0;
}