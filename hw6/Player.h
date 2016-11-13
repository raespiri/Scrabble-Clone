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

struct Move
{
	string dir;
	int row;
	int col;
	string tiles;
	int score;
	Move() {
		score = -1;
	}
	Move(string _dir, int _row, int _col, string _tiles, int _score) {
		dir = _dir;
		row = _row;
		col = _col;
		tiles = _tiles;
		score = _score;
	}
};

class Player {

public:
	Player(string name);
	Player(string name, Dictionary* temp_dict);
	~Player();
	void setScore(int score); //updates Player's score 
	int getScore(); //returns Player's score
	void movePass(); //pass turn
	void moveExchange(string old_tiles, Bag &bag); //exchange tiles func, returns true if successful
	void movePlace(string dir, int row, int col, string tiles_to_place, Board &board, Dictionary dictionary, Bag &bag, int &count); //place tiles func, returns true if successful
	void setHand(set<Tile*> tiles); //sets player's hand
	int handSize(); //returns hand size
	void printHand(); //print current hand
	void addToHand(set<Tile*> tiles); //adds tiles to player's hand
	void moveChecker(bool flag); //updates move_success variable if move worked/failed
	bool getMoveChecker(); //returns value of move checker
	string getName(); //returns player's name
	string getHand(); //returns hand as a string of tile letters

	// FUNCTIONS FOR CPUS
	bool AImovePlace(string dir, int row, int col, string tiles_to_place, Board &board, int &count); //place tiles func, returns true if successful
	void AIsolveHelper(int row, int col, string letters, string hand, Board &board, int &count); //permutates possible combinations of hand on a given square
	void AIsolve(Board &board, int &count); //loops through squares of board and calls AIsolveHelper
	void AIsetScore(int score); //updates AI's score 
	int AIgetScore(); //returns AI's score
	Move* AIgetMove();
	int getAImoves_size();

	set<string> used_prefixes;

	// FUNCTIONS FOR CPUL
	bool AImovePlace2(string dir, int row, int col, string tiles_to_place, Board &board, int &count); //place tiles func, returns true if successful
	void AIsolveHelper2(int row, int col, string letters, string hand, Board &board, int &count); //permutates possible combinations of hand on a given square
	void AIsolve2(Board &board, int &count); //loops through squares of board and calls AIsolveHelper
	void AIsetScore2(int score); //updates AI's score 
	int AIgetScore2(); //returns AI's score
	Move* AIgetMove2();
	int getAImoves_size2();

	set<string> used_prefixes2;

private:
	string _name;
	int _score;
	set<Tile*> _hand; //set of tiles in player's hand
	bool move_success;
	vector<Move> AImoves; //set of CPUS AI moves
	vector<Move> AImoves2; //set of CPUL AI moves
	int AIscore; //score of a CPUS move
	int AIscore2; //score of a CPUL move
	Dictionary* player_dict;
}; 

#endif /* PLAYER_H_ */
