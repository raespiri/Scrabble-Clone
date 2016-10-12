/*
 * Board.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <cstring>
#include <map>
#include <iomanip>

class Board {
public:
	Board (std::string board_file_name);

	// What else will this need?
	~Board();
	void printBoard();
	void updateSquare(std::string letter, int row, int col); //update square of board
	std::string getSquare(int row, int col); //grab current letter of board square
	void linkSquare(std::string letter, int val); //adds a newly letter on the board to the map
	int getPointVal(std::string letter); //returns point value of a given letter on the board
	int getCols(); //returns number of cols
	int getRows(); //returns number of rows
	bool isLetter(int row, int col); //check if square is a letter
	int getStartCol();
	int getStartRow();

private:
	int _x, _y; //col, row
	int _startx, _starty;

	// What else will this need?
	std::string** board; //used for 2d dynamic array
	std::map<std::string,int> _squares; //map to track placed letters along with their point values
};


#endif /* BOARD_H_ */
