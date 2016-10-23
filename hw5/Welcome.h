#ifndef WELCOME_H 
#define WELCOME_H 

#include <string>
#include <QObject> 
#include <QPushButton> 
#include <QLineEdit> 
#include <QHBoxLayout> 
#include <QVBoxLayout> 
#include <QGridLayout> 
#include <QLabel> 
#include <QComboBox> 
#include <iostream> 
#include <vector>
#include <QWidget>
#include <Headers.h>

using namespace std;

class Welcome : public QWidget { 

Q_OBJECT 

public slots: 
	void addPlayer();
	void openNewWindow();

public: 
	Welcome(QApplication *app, Board* scrabble_board, Bag* scrabble_bag, Dictionary* scrabble_dict, unsigned int scrabble_tiles); 
	~Welcome(); 
	
private: 
	QVBoxLayout *overallLayout; 
	QLabel *banner; //banner for welcome window
	QLabel *names; //player names label
	QPushButton *addButton; //button to add player
	QPushButton *startButton; //button to start the game
	QLineEdit *nameInput; //player name input
	Scrabble *mainWindow; //main window for scrabble game
	vector<string> player_names; //vector of player names
	QApplication *app;
	Board* board_pointer; //pointer to config board
	Bag* bag_pointer; //pointer to config bag
	Dictionary* dict_pointer; //pointer to config dictionary
	unsigned int num_tiles; //number of tiles to draw
};

#endif // WELCOME_H