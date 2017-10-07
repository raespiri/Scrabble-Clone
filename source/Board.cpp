/*
 * Board.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Board.h"
#include <algorithm>

using namespace std;

Board::Board (string board_file_name)
{
	ifstream boardFile (board_file_name.c_str());
	string row;

	_x = _y = _startx = _starty = 0; // to appease compiler
	if (!boardFile.is_open())
		throw invalid_argument("Cannot open file: " + board_file_name);
	getline (boardFile, row);
	stringstream s1 (row);
	s1 >> _x >> _y;
	//2d array for the board
	board = new string*[_y];
	for(int i = 0; i < _y; ++i) {
		board[i] = new string[_x];
	}

	getline (boardFile, row);
	stringstream s2 (row);
	s2 >> _startx >> _starty;
	_startx --; _starty --;  // coordinates from 0 in array

	// Anything else you need to initialize?

	for (int i = 0 ; i < _y; ++ i)
	{
		getline (boardFile, row);
		for (int j = 0; j < _x; ++ j)
		{
			//converting row[j] to string
			string temp = "";
			temp += row[j];
			// Fill in the following based on how you store the board.
			if (i == _starty && j == _startx) { }
			else switch (row[j]) {
			case '.' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case '2' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case '3' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case 'd' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case 't' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			default:
				string error = "Improper character in Board file: ";
				throw invalid_argument(error + row[j]);
			}
		}
	}
	board[_starty][_startx] = "."; //setting starting point of the board to be a period
	boardFile.close ();
}

Board::Board (string board_file_name, string init_file_name, int &turn_counter)
{
	ifstream boardFile (board_file_name.c_str());
	string row;

	_x = _y = _startx = _starty = 0; // to appease compiler
	if (!boardFile.is_open())
		throw invalid_argument("Cannot open file: " + board_file_name);
	getline (boardFile, row);
	stringstream s1 (row);
	s1 >> _x >> _y;
	//2d array for the board
	board = new string*[_y];
	for(int i = 0; i < _y; ++i) {
		board[i] = new string[_x];
	}

	getline (boardFile, row);
	stringstream s2 (row);
	s2 >> _startx >> _starty;
	_startx --; _starty --;  // coordinates from 0 in array

	// Anything else you need to initialize?

	for (int i = 0 ; i < _y; ++ i)
	{
		getline (boardFile, row);
		for (int j = 0; j < _x; ++ j)
		{
			//converting row[j] to string
			string temp = "";
			temp += row[j];
			// Fill in the following based on how you store the board.
			if (i == _starty && j == _startx) { }
			else switch (row[j]) {
			case '.' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case '2' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case '3' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case 'd' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			case 't' :
			board[i][j] = temp; //setting board square equal to row[j]'s val'
			break;
			default:
				string error = "Improper character in Board file: ";
				throw invalid_argument(error + row[j]);
			}
		}
	}
	board[_starty][_startx] = "."; //setting starting point of the board to be a period
	boardFile.close ();

	ifstream initFile (init_file_name.c_str());
	string init_row;
	for (int i = 0 ; i < _y; ++ i)
	{
		getline (initFile, init_row);
		if(init_row.size() == 0) { //if there is no init_file
			break;
		}
		else {
			turn_counter++;
		}
		for (int j = 0; j < (3*_x); ++j)
		{
			//converting row[j] to string
			string temp = "";
			temp += init_row[j];
			if(temp != "." && temp != "0"
				&& temp != "1" && temp != "2"
				&& temp != "3" && temp != "4"
				&& temp != "5" && temp != "6"
				&& temp != "7" && temp != "8"
				&& temp != "9") { //if the char is a letter tile
				
				int score; //variable to hold init tile's score
				if(init_row[j+1] == '0') { //if one digit score
					stringstream ss;
					ss << init_row[j+2];
					ss >> score;	
				}
				else { //else two digit score
					stringstream ss;
					ss << init_row[j+1];
					ss << init_row[j+2];
					ss >> score;
				}
				transform(temp.begin(), temp.end(), temp.begin(), ::toupper); //convert tile to uppercase
				if(j == 0) { //if init tile is in first col
					board[i][j] = temp; //update board
					linkSquare(temp, score); //linking square and tile score to board
				}
				else {
					int x = j/3; //dividing col by 3
					board[i][x] = temp;
					linkSquare(temp, score); //linking square and tile score to board
				}
			}
		}
	}
	initFile.close();
}

Board::~Board() {
	//clean-up for dynamic 2d array
	for(int i = 0; i < _y; ++i) {
    	delete [] board[i];
	}
	delete [] board;
}

void Board::printBoard() {
	//loop through board and print it's values
	cout << "   ";
	for (int i = 0 ; i < _y; ++ i) {
		for (int j = 0; j < _x; ++ j) {
			cout << board[i][j] << setw(4);
		}
		cout << endl;
	}
}

void Board::updateSquare(string letter, int row, int col) {
	board[row][col] = letter;
}

std::string Board::getSquare(int row, int col) {
	return board[row][col];
}

void Board::linkSquare(string letter, int val) {
	map<string,int>::iterator it; //iterator for _squares map
	it = _squares.find(letter); //setting iterator equal to letter key
	if (it == _squares.end()) { //if no val was found
		_squares.insert(pair<string,int>(letter, val));  //adding newly placed letter on board to map with its point value
	}
}

int Board::getPointVal(string letter) {
	map<string,int>::iterator it; //iterator for _squares map
	it = _squares.find(letter); //setting iterator equal to letter key
	if (it != _squares.end()) { //if key was found with value
    	return it->second; //return its value
	}
}

int Board::getCols() {
	return _x;
}

int Board::getRows() {
	return _y;
}

bool Board::isLetter(int row, int col) {
	if(isupper((board[row][col])[0])) { //if square is an uppercase letter
		return true; //return true
	}
	else {
		return false;
	}
}

int Board::getStartCol() {
	return _startx;
}

int Board::getStartRow() {
	return _starty;
}

string Board::getWord(string dir, int row, int col) {
	string word = "";
	if(dir == "|") {
		while(isupper((board[row][col])[0])) { //if square is an uppercase letter
			word += (board[row][col])[0]; //add char to word
			row--; //move to previous col
		}
	}
	else {
		while(isupper((board[row][col])[0])) { //if square is an uppercase letter
			word += (board[row][col])[0]; //add char to word
			col--; //move to previous col
		}
	}
	reverse(word.begin(), word.end()); //reversing word for letters to be in correct order
	transform(word.begin(), word.end(), word.begin(), ::tolower); //making word lowercase
	return word;
}