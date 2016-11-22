#include "avlbst.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <map>

using namespace std;

// Data Structure for range of floors
class Floors {
public:
	Floors(long long start, long long end) : _start(start), _end(end) { }
	~Floors() { }
	const long long getStart() const { return _start; } 
	const long long getEnd() const { return _end; }
	long long getStart() { return _start; } 
	long long getEnd() { return _end; }
	void setStart(long long start) const { _start = start; }
	void setEnd(long long end) const { _end = end; }
	
	// Overloading "<"" operator for Floors class

private:
	mutable long long _start;
	mutable long long _end;
};

bool operator<(const Floors& r, const Floors& l) { return (r.getStart() < l.getStart()); }
bool operator<=(const Floors& r, const Floors& l) { return (r.getStart() <= l.getStart()); }
bool operator>(const Floors& r, const Floors& l) { return (r.getStart() > l.getStart()); }
bool operator>=(const Floors& r, const Floors& l) { return (r.getStart() >= l.getStart()); }
bool operator==(const Floors& r, const Floors& l) { return (r.getStart() == l.getStart() && r.getEnd() == l.getEnd()); }
bool operator!=(const Floors& r, const Floors& l) { return (r.getStart() != l.getStart() || r.getEnd() != l.getEnd()); }

AVLNode<Floors, string>* findNode(const long long height, AVLNode<Floors, string>* node) { //modified internal find function to find a node
	AVLNode<Floors, string>* curr = node;
	while(curr) {
		if (curr->getKey().getStart() <= height && height <= curr->getKey().getEnd()) { //if passed height is in the floor range of node
			return curr;
		} 
		else if (height < curr->getKey().getStart()) { //else if passed height is smaller than floor range
			curr = curr->getLeft();
		} 
		else {
			curr = curr->getRight();
		}
	}
	return NULL; //return NULL if floor range wasn't found
}

int main(int argc, char* argv[]) {
	if(argc < 3){
    	cout << "Please provide an input and output file." << endl;
    	return 1;
  	}
	
	ifstream input(argv[1]); //getting input file
  	ofstream output(argv[2]); //getting output file

  	map<Floors, string> myMap; //declaring map to store floors
  	Floors temp(1, static_cast<long long>(std::pow(2, 63)) - 1); //creating a new Floors Key
  	myMap.insert(make_pair(temp, "white")); //initially the skyscraper contains all white floors

  	string curr; //string to hold current line in input file

  	while(getline(input, curr)) { //loop through lines of the input file
		stringstream ss;
	  	ss << curr;
	  	string command;
	  	ss >> command;
	  	if(command == "RECOLOR") {
		  	long long x,y;
			string c;
		  	ss >> x;
		  	ss >> y;
		  	ss >> c;
		  	string extra; //variable to check for incorrect command
		  	if (ss.fail() || y < x || ss >> extra) {
			  	output << "Error - incorrect command" << endl;
		  	}
		  	else {
		  		for(map<Floors, string>::iterator it = myMap.begin(); 
		  			it != myMap.end(); ++it) { //loop through AVL tree
  					//partial overlap found
  					if((*it).first.getStart() < x && (*it).first.getEnd() > x && (*it).first.getEnd() <= y) { //if found range's start is less than inputed range's start
  						(*it).first.setEnd(x-1); //update the start range of found node
  						break;
  					}

  					 //partial overlap found
  					if(x <= (*it).first.getStart() && y < (*it).first.getEnd() && y > (*it).first.getStart()) { //if inputed range's start is less than found range's start
  						(*it).first.setStart(y+1); //update the start range of found node
  						break;
  					}

  					//if range matches perfectly
  					if(x == (*it).first.getStart() && (*it).first.getEnd() == y) {
  						(*it).second = c; //update the start range of found node
  						break;
  					}

  					//found a range that is fully contained within the input range
  					if(x <= (*it).first.getStart() && (*it).first.getEnd() <= y) {
  						myMap.erase(it); //remove the found node
  						break;
  					}

  					//found a range that fully contains the input range
  					if((*it).first.getStart() < x && y < (*it).first.getEnd()) {
  						Floors temp(y+1, (*it).first.getEnd()); //create a new Floors key for second half
  						(*it).first.setEnd(x-1); //update end range of first half
  						myMap.insert(make_pair(temp, (*it).second)); //insert node with new floors key
  						break;
  					}
				}
				Floors temp(x, y); //create a new Floors key for the inputed value
  				myMap.insert(make_pair(temp, c)); //insert node with new floors key and inputed color
		  	}
	  	}
	  	else if(command == "COLOR") {
	  		long long x;
			ss >> x;
			string extra; //variable to check for incorrect command
			if (ss.fail() || ss >> extra) {
				output << "Error - incorrect command" << endl;
			}
	  		else {
	  			for(map<Floors, string>::iterator it = myMap.begin(); it != myMap.end(); ++it) {
  					if((*it).first.getStart() <= x && x <= (*it).first.getEnd()) {
  					output << (*it).second << endl;
  					}
  				}
	  		}
	  	}
  	  	else {
  	  		output << "Error - incorrect command" << endl;
  	  	}
  	}
	return 0;
}