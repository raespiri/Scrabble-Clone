#include "Headers.h" 
#include <iostream>

using namespace std;

Scrabble::Scrabble(QApplication *app, Board* scrabble_board, Bag* scrabble_bag, Dictionary* scrabble_dict, unsigned int numTiles) { 
	bag_pointer = scrabble_bag; //pointer to scrabble bag
	board_pointer = scrabble_board; //pointer to board
	dict_pointer = scrabble_dict; //pointer to dict
	number_of_tiles = numTiles; //amount of tiles users have in hand
	turn_counter = 0; //zero turns have passed
	pass_counter = 0;

	errors = new QString[6];
	errors[0] = "One or more of the tiles placed would be out of bounds of the board";
	errors[1] = "You are trying to place a word starting at a position that is already occupied by a tile";
	errors[2] = "At least one of the tiles you wish to exchange is not in your hand";
	errors[3] = "You tried to make a move that does not have at least one tile adjacent to a previously placed tile"; 
	errors[4] = "For the first move of the game, one of your tiles must be on the start square";
	errors[5] = "You entered an invalid word, please enter a valid move";

	int rows = scrabble_board->getRows(); //getting number of rows from board class
	int cols = scrabble_board->getCols();	//getting number of cols form board class
	this->app = app; 
	_rows = rows;
	_cols = cols;
	h0 = new QHBoxLayout;
	h1 = new QHBoxLayout; 
	v1 = new QVBoxLayout; 
	moves = new QComboBox; 
	
	hand = new QLabel(""); 	
	QFont handFont;
    handFont.setBold(true);
    hand->setFont(handFont);

	g = new QGridLayout; 
	board = new QPushButton**[rows]; 
	for (int i = 0; i < rows; i++) { 
		board[i] = new QPushButton*[cols]; 
		for (int j = 0; j < cols; j++) { 
			board[i][j] = new QPushButton;
			board[i][j]->setStyleSheet("border-style: inset; border-width: 2px; border-width: 6px"); //adding shape to the board squares
			board[i][j]->setFont(handFont);
			string str = scrabble_board->getSquare(i,j); //getting board square string from board.h
			if(str == ".") { //converting dots to blank spaces
				str = " ";
				board[i][j]->setStyleSheet("background-color: white");
			}
			else if(str == "d") {
				str = "2W";
				board[i][j]->setStyleSheet("background-color: red");
			}
			else if(str == "t") {
				str = "3W";
				board[i][j]->setStyleSheet("background-color: magenta");
			}
			else if(str == "2") {
				str = "2L";
				board[i][j]->setStyleSheet("background-color: cyan");
			}
			else if(str == "3") {
				str = "3L";
				board[i][j]->setStyleSheet("background-color: darkCyan");
			}
			QString qstr = QString::fromStdString(str); //converting to qstring
			board[i][j]->setText(qstr); //updating display board squares to show point values
			g->addWidget(board[i][j],i+1,j+1); 
			QObject::connect(board[i][j], SIGNAL(clicked()), this, SLOT(act())); 
		} 
	} 
	setLayout(v1);
	v1->addLayout(g); 
	moves->addItem("Place Horizontal");
	moves->addItem("Place Vertical"); 
	moves->addItem("Exchange"); 
	moves->addItem("Pass"); 
	tiles = new QLineEdit();

	playerLabel = new QLabel("");
	QFont playerFont;
    playerFont.setBold(true);
    playerFont.setPointSize(14);
    playerLabel->setFont(playerFont);

	play = new QPushButton("Act");
	connect(play, SIGNAL(clicked()), this, SLOT(act()));

	v1->addLayout(h0);
	v1->addLayout(h1); 
	h1->addWidget(playerLabel);
	h1->addWidget(hand); 
	h1->addWidget(moves); 
	h1->addWidget(tiles); 
	h1->addWidget(play); 
}

Scrabble::~Scrabble() { 
	delete moves; 
	for (int i = 0; i < _rows; i++) { 
		for (int j = 0; j < _cols; j++) { 
			delete board[i][j]; 
		} 
		delete board[i]; 
	} 
	delete board; 
	delete g; 
	delete tiles; 
	delete hand; 
	delete play; 
	delete h1; 
	delete v1; 
}


void Scrabble::act() {
	if(bag_pointer->tilesRemaining() < 1 && emptyHand(playerList)) { //if there are no more tiles and playerlist it empty
		return;
	}
	if(moves->currentText() == "Exchange" && (QPushButton*) QObject::sender() == play && tiles->text() != "") { //object that emitted the signal was the play button
		QString qstr = tiles->text(); // grabbing tiles from input field
		string str = qstr.toStdString(); // converting to str
		playerList[curr_player]->moveExchange(str, *bag_pointer); //calling exchange function
			
		if(playerList[curr_player]->getMoveChecker() == false) { //if there is an error in the user's exchange
			QMessageBox msgBox; //create popup
			msgBox.setWindowTitle("Error");
			msgBox.setText("At least one of the tiles you wish to exchange is not in your hand");
			msgBox.exec(); //show popup
		}
		else { //else no error
			int size = (int) playerList.size();
			if(curr_player+1 < size) { //if not the last player in the game
				curr_player++; //move index to next player
			}
			else {
				curr_player = 0; //else set curr_player index to beginning
			}
			tiles->setText(""); //clearing text from tiles line edit
			setHandLabel(curr_player); //update hand display to next player
			setPlayerLabel(curr_player); //update player label to next player
			pass_counter = 0; //setting pass counter to 0;
			return;
		}
	}	
	else if(moves->currentText() == "Pass" && (QPushButton*) QObject::sender() == play) { //object that emitted the signal was the play button
		int size = (int) playerList.size();
		if(curr_player+1 < size) { //if not the last player in the game
			curr_player++; //move index to next player
		}
		else {
			curr_player = 0; //else set curr_player index to beginning
		}
		tiles->setText(""); //clearing text from tiles line edit
		setHandLabel(curr_player); //update hand display to next player
		setPlayerLabel(curr_player); //update player label to next player
		pass_counter++; //increment pass counter
		
		if(pass_counter == size) { // if all users pass
			QMessageBox msgBox; //create popup
			msgBox.setWindowTitle("Error");
			msgBox.setText("All users passed, game over"); //call correct error index
			vector<int> player_winners(getWinner()); //copy of winning player indices
			stringstream ss; //declaring stringstream
			for(unsigned int i = 0; i < player_winners.size(); i++) {
				ss << playerList[player_winners[i]]->getName(); //grabbing player's hand from player vector
				ss << " won the game!";
				ss << '\n'; //adding new line character
			}
			string str = ss.str(); //converting to string
			QString temp = QString::fromStdString(str);
			msgBox.setInformativeText(temp);
			msgBox.exec(); //show popup
			this->close(); //close window
		}

		return;
	}
	else if(moves->currentText() == "Place Horizontal" && (QPushButton*) QObject::sender() != play && tiles->text() != "") { //object that emitted the signal wasn't the play button
		QString qstr = tiles->text(); // grabbing tiles from input field
		string str_tiles = qstr.toStdString(); // converting to str

		//Loop to find the tile that the user clicked on the board
		QPushButton *called = (QPushButton*) QObject::sender(); 
		int start_row = 0,start_col = 0; 
		for (int y = 0; y < _rows; y++) { 
			for (int x = 0; x < _cols; x++) { 
				if (board[y][x] == called) { 
					start_row = y; //starting spot of word
					start_col = x; //starting spot of word
				} 
			} 
		} 

		//calling Place function
		playerList[curr_player]->movePlace("-", start_row+1, start_col+1, str_tiles, *board_pointer, *dict_pointer, *bag_pointer, turn_counter, &error);
		
		if(playerList[curr_player]->getMoveChecker() == false) { //if there is an error in the user's exchange
			QMessageBox msgBox; //create popup
			msgBox.setWindowTitle("Error");
			msgBox.setText(errors[error-1]); //call correct error index
			msgBox.exec(); //show popup
		}
		else {
			updateQtBoard(board_pointer);
			updateScoreLabel();

			// Moving to next player index
			int size = (int) playerList.size();
			if(curr_player+1 < size) { //if not the last player in the game
				curr_player++; //move index to next player
			}
			else {
				curr_player = 0; //else set curr_player index to beginning
			}
			setHandLabel(curr_player);
			setPlayerLabel(curr_player);
			turn_counter++; //increment turn
			pass_counter = 0; //setting pass counter to 0
			if(bag_pointer->tilesRemaining() < 1 && emptyHand(playerList)) {
				QMessageBox msgBox; //create popup
				msgBox.setWindowTitle("Game Over");
				msgBox.setText("Game Over!"); //call correct error index
				vector<int> player_winners(getWinner()); //copy of winning player indices
				stringstream ss; //declaring stringstream
				for(unsigned int i = 0; i < player_winners.size(); i++) {
					ss << playerList[player_winners[i]]->getName(); //grabbing player's hand from player vector
					ss << " won the game!";
					ss << '\n'; //adding new line character
				}
				string str = ss.str(); //converting to string
				QString temp = QString::fromStdString(str);
				msgBox.setInformativeText(temp);
				msgBox.exec(); //show popup
				this->close(); //close window
			}
			return;
		}
	}
	else if(moves->currentText() == "Place Vertical" && (QPushButton*) QObject::sender() != play && tiles->text() != "") { //object that emitted the signal wasn't the play button
		QString qstr = tiles->text(); // grabbing tiles from input field
		string str_tiles = qstr.toStdString(); // converting to str

		//Loop to find the tile that the user clicked on the board
		QPushButton *called = (QPushButton*) QObject::sender(); 
		int start_row = 0,start_col = 0; 
		for (int y = 0; y < _rows; y++) { 
			for (int x = 0; x < _cols; x++) { 
				if (board[y][x] == called) { 
					start_row = y; //starting spot of word
					start_col = x; //starting spot of word
				} 
			} 
		} 

		//calling Place function
		playerList[curr_player]->movePlace("|", start_row+1, start_col+1, str_tiles, *board_pointer, *dict_pointer, *bag_pointer, turn_counter, &error);
		
		if(playerList[curr_player]->getMoveChecker() == false) { //if there is an error in the user's exchange
			QMessageBox msgBox; //create popup
			msgBox.setWindowTitle("Error");
			msgBox.setText(errors[error-1]); //call correct error index
			msgBox.exec(); //show popup
		}
		else {
			updateQtBoard(board_pointer);
			updateScoreLabel();

			// Moving to next player index
			int size = (int) playerList.size();
			if(curr_player+1 < size) { //if not the last player in the game
				curr_player++; //move index to next player
			}
			else {
				curr_player = 0; //else set curr_player index to beginning
			}
			setHandLabel(curr_player);
			setPlayerLabel(curr_player);
			turn_counter++; //increment turn
			pass_counter = 0; //setting pass counter to 0
			if(bag_pointer->tilesRemaining() < 1 && emptyHand(playerList)) {
				QMessageBox msgBox; //create popup
				msgBox.setWindowTitle("Game Over");
				msgBox.setText("Game Over!"); //call correct error index
				vector<int> player_winners(getWinner()); //copy of winning player indices
				stringstream ss; //declaring stringstream
				for(unsigned int i = 0; i < player_winners.size(); i++) {
					ss << playerList[player_winners[i]]->getName(); //grabbing player's hand from player vector
					ss << " won the game!";
					ss << '\n'; //adding new line character
				}
				string str = ss.str(); //converting to string
				QString temp = QString::fromStdString(str);
				msgBox.setInformativeText(temp);
				msgBox.exec(); //show popup
				this->close(); //close window
			}
			return;
		}
	}	
}
	
void Scrabble::setPlayers(vector<string> new_players) {
	players = new_players;

	// Player setup
	for(unsigned int i = 0; i < players.size(); i++) {
			Player* new_player = new Player(players[i]);
			new_player->setHand(bag_pointer->drawTiles(number_of_tiles)); //drawing tiles from bag to set new player's hand
			playerList.push_back(new_player); //adding new player to vector*/
	}
}

void Scrabble::setHandLabel(int pos) {
	stringstream ss;
	ss << playerList[pos]->getHand(); //grabbing player's hand from player vector
	string str = ss.str();
	QString temp = QString::fromStdString(str);

	hand->setText(temp);
}

void Scrabble::setPlayerLabel(int pos) {
	stringstream ss;
	ss << playerList[pos]->getName(); //grabbing player's name from player vector
	ss << "'s hand:";
	string str = ss.str();
	QString temp = QString::fromStdString(str); //converting str to qstr
	playerLabel->setText(temp);
	curr_player = pos;
}

bool Scrabble::emptyHand(vector<Player*> player_list) { //bool function to check if a player has an empty hand
	for(unsigned int i = 0; i < player_list.size(); i++) {
		if(player_list[i]->handSize() == 0) { //if a player has a handsize of 0 return true
			return true;
		}
	}
	return false;
}

void Scrabble::setScoreLabel() {
	scoreLabel = new QLabel*[0];
	for(unsigned int i = 0; i < playerList.size(); i++) {
		stringstream ss;
		ss << playerList[i]->getName(); //grabbing player's name from Player vector
		ss << "'s score: ";
		ss << playerList[i]->getScore(); //grabbing player's score from Player vector
		string str = ss.str(); //converting ss to str
		QString name = QString::fromStdString(str); //converting player name from str to qstr
		scoreLabel[i] = new QLabel(name);
		
		// Bolding font
		QFont playerScoreFont;
    	playerScoreFont.setBold(true);
    	playerScoreFont.setPointSize(14);
    	(scoreLabel[i])->setFont(playerScoreFont);
		
		h0->addWidget(scoreLabel[i]); //adding score display to layout
	}
}

void Scrabble::updateScoreLabel() {
	for(unsigned int i = 0; i < playerList.size(); i++) {
		stringstream ss;
		ss << playerList[i]->getName(); //grabbing player's name from Player vector
		ss << "'s score: ";
		ss << playerList[i]->getScore(); //grabbing player's score from Player vector
		string str = ss.str(); //converting ss to str
		QString name = QString::fromStdString(str); //converting player name from str to qstr
		(scoreLabel[i])->setText(name);
	}
}

void Scrabble::updateQtBoard(Board* scrabbleBoard) {
	for (int i = 0; i < _rows; i++) { 
		for (int j = 0; j < _cols; j++) { 
			string str = scrabbleBoard->getSquare(i,j); //getting board square string from board.h
			if(str == ".") { //converting dots to blank spaces
				str = " ";
				board[i][j]->setStyleSheet("background-color: white");
			}
			else if(str == "d") {
				str = "2W";
				board[i][j]->setStyleSheet("background-color: red");
			}
			else if(str == "t") {
				str = "3W";
				board[i][j]->setStyleSheet("background-color: magenta");
			}
			else if(str == "2") {
				str = "2L";
				board[i][j]->setStyleSheet("background-color: cyan");
			}
			else if(str == "3") {
				str = "3L";
				board[i][j]->setStyleSheet("background-color: darkCyan");
			}
			else {
				board[i][j]->setStyleSheet("background-color: beige");
			}
			QString qstr = QString::fromStdString(str); //converting to qstring
			board[i][j]->setText(qstr); //updating display board squares to show point values
		} 
	} 
}

vector<int> Scrabble::getWinner() {
	vector<int> winners; //vector of winning indices
	winners.push_back(0); //base case pushback first player
	int max = 0;
	for(unsigned int i = 0; i < playerList.size(); i++) {
		if(playerList[i]->getScore() > playerList[max]->getScore()) {
			max = i; //update max index
			winners.clear(); //clear old winner
			if(i != 0) { //avoiding doubling up index 0
				winners.push_back(max); //add new winner
			}
		}
		else if(playerList[i]->getScore() == playerList[max]->getScore()) { //if there is a tie return arbitrary value
			if(i != 0) { //avoiding doubling up index 0
				winners.push_back(i); //add new winner
			}
		}
	}

	return winners;
}

int Scrabble::emptyHandIndex(vector<Player*> player_list) { //bool function to check if a player has an empty hand
	for(unsigned int i = 0; i < player_list.size(); i++) {
		if(player_list[i]->handSize() == 0) { //if a player has a handsize of 0 return true
			return i;
		}
	}
	return -1;
}