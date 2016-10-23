#ifndef HEADERS_H 
#define HEADERS_H 

#include <QObject> 
#include <QPushButton> 
#include <QLineEdit> 
#include <QHBoxLayout> 
#include <QVBoxLayout> 
#include <QGridLayout> 
#include <QApplication> 
#include <QLabel> 
#include <QComboBox> 
#include <iostream> 
#include "Board.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Bag.h"
#include <QMessageBox>
#include <sstream>
#include <Dictionary.h>

using namespace std;

class Scrabble : public QWidget { 

Q_OBJECT 

public slots: 
	void act(); 

public: 
	Scrabble(QApplication *app, Board* scrabble_board, Bag* scrabble_bag, Dictionary* scrabble_dict, unsigned int numTiles); 
	~Scrabble(); 
	void setPlayers(vector<string> new_players); //creating vector of player names
	void setHandLabel(int pos); //updates hand label to given players hand
	void setPlayerLabel(int pos); //updates player label to given players name
	bool emptyHand(vector<Player*> player_list); //bool function to check if a player has an empty hand
	void setScoreLabel(); //sets intial player scores
	void updateScoreLabel(); //updates score label
	void updateQtBoard(Board* scrabbleBoard); //updates QT board
	vector<int> getWinner(); //vector of indices of winning players
	int emptyHandIndex(vector<Player*> player_list); //gets index of player with empty hand

private: 
	QApplication *app; 
	QPushButton ***board, *play; 
	QLineEdit *tiles; 
	QLabel *hand; 
	QLabel *playerLabel;
	QLabel **scoreLabel;
	QComboBox *moves; 
	QHBoxLayout *h0; 
	QHBoxLayout *h1; 
	QVBoxLayout *v1; 
	QGridLayout *g; 
	int _rows; //board rows
	int _cols; //board cols
	vector<string> players; //vector to store player's names
	vector<Player*> playerList; //vector of player pointers
	Bag* bag_pointer; //pointer to bag
	Board* board_pointer; //pointer to board
	Dictionary* dict_pointer; //pointer to dict
	unsigned int number_of_tiles; //number of tiles to draw
	int curr_player; //current index of players
	int turn_counter; //turn counter
	int error; //error number
	QString* errors; //list of errors
	int pass_counter; //tracks if everyone passes
};

#endif // HEADERS_H