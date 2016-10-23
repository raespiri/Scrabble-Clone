#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include "Bag.h"
#include "Tile.h"
#include <iostream>
#include <algorithm>
#include "Board.h"
#include <vector>
#include "Dictionary.h"
#include <algorithm>

using namespace std;

class Player {

public:
	Player(string name);
	~Player();
	void setScore(int score); //updates Player's score 
	int getScore(); //returns Player's score
	void movePass(); //pass turn
	void moveExchange(string old_tiles, Bag &bag); //exchange tiles func, returns true if successful
	void movePlace(string dir, int row, int col, string tiles_to_place, Board &board, Dictionary dictionary, Bag &bag, int &count, int* error); //place tiles func, returns true if successful
	void setHand(set<Tile*> tiles); //sets player's hand
	int handSize(); //returns hand size
	void printHand(); //print current hand
	void addToHand(set<Tile*> tiles); //adds tiles to player's hand
	void moveChecker(bool flag); //updates move_success variable if move worked/failed
	bool getMoveChecker(); //returns value of move checker
	string getHand(); //returns hand as a string of tile letters
	string getName(); //returns player's name

private:
	string _name;
	int _score;
	set<Tile*> _hand; //set of tiles in player's hand
	bool move_success;
}; 

#endif /* PLAYER_H_ */
