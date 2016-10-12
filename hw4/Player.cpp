#include <string>
#include "Player.h"

using namespace std;

Player::Player(string name) {
	_name = name;
	_score = 0; 
	move_success = false;
}

Player::~Player() {
	set<Tile*>::iterator it; //iterator
	for(it = _hand.begin(); it != _hand.end(); ++it) {
		delete *it;
	}
}

void Player::setScore(int score) {
	_score = score;
}

int Player::getScore() {
	return _score;
}

void Player::movePass() { //do nothing
	moveChecker(true);
}

void Player::moveExchange(string old_tiles, Bag &bag) {
	transform(old_tiles.begin(), old_tiles.end(), old_tiles.begin(), ::toupper); //convert tiles to uppercase for
	
	bool exchange_success = true; //flag to check if exchange succeeded

	//checking if tiles user proposes to throw away are in his/her hand
	int num_tiles = (int) old_tiles.length(); //casting unsigned int of string size to int 
  	for(int i = 0; i < num_tiles; i++) { //loop through tiles 
  		
  		bool found = false; //flag to check if letter was found
  		set<Tile*>::iterator it; //iterator

  		for(it = _hand.begin(); it != _hand.end(); ++it) { //loop through hand
    		if((*it)->getLetter() == old_tiles[i]) { //if tile is found in set
    			found = true; //found tile in player's hand
    			break;
    		} 
		}

		if(found == false) { //if tile was not found in player's hand
			cout << "You do not have the tile " << old_tiles[i] << " in your hand, please enter a valid move:" << endl;
			exchange_success = false; //flag for successful command set to false
			moveChecker(exchange_success); //return false
			return;
		}
	}	
	
	//loop to add tiles back to bag and erase them from user's hand
	for(int j = 0; j < num_tiles; j++) {
		
		set<Tile*>::iterator it; //iterator
		it = _hand.begin();
  		while(it != _hand.end()) { //loop to grab tiles from player's hand 
  			if((*it)->getLetter() == old_tiles[j]) { //if tile is found in set
  				Tile* temp = new Tile(old_tiles[j], (*it)->getPoints()); //creating new tile object to add back to bag
    			bag.addTile(temp); //adding tile back to bag
    			_hand.erase(it++); //erasing tile from hand;
    			break;
    		}
    		else {
    			++it;
    		}	
  		}
  	}

	addToHand(bag.drawTiles(num_tiles)); //drawing same number of tiles as exchanged back into bag
	moveChecker(exchange_success);
}

void Player::movePlace(string dir, int row, int col, string tiles_to_place, Board &board, Dictionary dictionary, Bag &bag, int &count) {
	row = row-1; //accounting for the fact that board starts at index (1,1)
	col = col-1; //accounting for the fact that board starts at index (1,1)

	bool place_success = true; //flag to check if exchange succeeded

	int num_tiles = (int) tiles_to_place.length(); //casting unsigned int of string size to int 

	//If one or more of the tiles placed would be out of bounds of the board
	if(dir == "-") {
		if(row < 0 || row >= board.getRows() || col < 0 || col > board.getCols()-num_tiles+1) {
			cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return;
		}
	}
	else if(dir == "|") {
		if(row < 0 || row > board.getRows()-num_tiles+1 || col < 0 || col >= board.getCols()) {
			cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return;
		}
	}

	//If user is trying to place a word starting at a position that is already occupied by a tile. 
	if(isupper((board.getSquare(row,col))[0])) {
		cout << "You are trying to place a word starting at a position that is already occupied by a tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return;
	}

	transform(tiles_to_place.begin(), tiles_to_place.end(), tiles_to_place.begin(), ::toupper); //convert tiles to uppercase for
		
	vector<Tile*> word; //set of tiles which make up the word to place on the board

	bool question_mark = false;
	//Loop to check if user has desired tiles to place in his/her hand
	for(int i = 0; i < num_tiles; i++) { //loop through tiles to place
  		
  		bool found = false; //flag to check if letter was found
  		set<Tile*>::iterator it; //iterator

  		for(it = _hand.begin(); it != _hand.end(); it++) { //loop to grab tiles from player's hand
    		if((*it)->getLetter() == tiles_to_place[i]) { //if tile is found in set
    			if(tiles_to_place[i] == '?') { //if user wishes to use a blank tile
    				(*it)->useAs(tiles_to_place[i+1]); //use blank tile as the char which immediately follows it in input
    				Tile* newTile = new Tile((*it)->getUse(), 0); //creating tile for blank tile with point val of 0
    				word.push_back(newTile);
    				i++; //increment i 
    				found = true; //found tile
    				question_mark = true;
    				break;
    			}
    			else {
    				word.push_back(*it); //adding tile to word set
    				found = true; //found tile in player's hand
    				break;
    			}
    		} 
		}

		if(found == false) { //if tile was not found in player's hand
			cout << "You do not have the tile " << tiles_to_place[i] << " in your hand, please enter a valid move:" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return;
		}	
	}	

	//grabbing values of squares where user is proposing to place his/her word
	string old_squares = ""; //string of old squares to place tiles on
	int prev_placed_squares = 0;
	if(dir == "-") {
		for(int i = col; i < (col+num_tiles+prev_placed_squares); i++) {
			old_squares += board.getSquare(row, i); //add square from board to string of squares
			if(isupper((board.getSquare(row,i))[0])) { //if a previously placed tile is found
				prev_placed_squares++; //increment
			}
		}
	}
	else if(dir == "|") {
		for(int i = row; i < (row+num_tiles+prev_placed_squares); i++) {
			old_squares += board.getSquare(i, col); //add square from board to string of squares		
			if(isupper((board.getSquare(i, col))[0])) { //if a previously placed tile is found
				prev_placed_squares++; //increment
			}
		}
	}

	//Loop to add pre-existing letters on the board to word vector
	int string_len = (int) old_squares.length(); //casting unsigned int to int
	for(int j = 0; j < string_len; j++) {
		//if statement to find any pre-existing letters on the board
		if(old_squares[j] != '.' && old_squares[j] != '2' && old_squares[j] != '3' && old_squares[j] != 't' && old_squares[j] != 'd') {
			string temp = "";
			temp += old_squares[j]; //converting char to string
			Tile* newTile = new Tile(old_squares[j], board.getPointVal(temp)); //creating tile for letter already placed on board
			word.insert(word.begin()+j, newTile);  //inserting new tile to word vectior
		}
	}

	//Check for existing letters on the board which come BEFORE the tiles the user places
	int prefix = 0; //length of prefix string pre-existing on the board;
	if(dir == "-") {
		int i = col-1; 
		while(i > 0) {
			if(isupper((board.getSquare(row,i))[0])) {
				Tile* newTile = new Tile((board.getSquare(row,i))[0], board.getPointVal(board.getSquare(row,i))); //creating tile for letter already placed on board
				word.insert(word.begin(), newTile); //inserting new tile to word vectior
				i--;
				prefix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}
	else if(dir == "|") {
		int i = row-1; 
		while(i > 0) {
			if(isupper((board.getSquare(i,col))[0])) {
				Tile* newTile = new Tile((board.getSquare(i,col))[0], board.getPointVal(board.getSquare(i,col))); //creating tile for letter already placed on board
				word.insert(word.begin(), newTile); //inserting new tile to word vectior
				i--;
				prefix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}

	//Check for existing letters on the board which come AFTER the tiles the user places
	int suffix = 0; //length of suffix string pre-existing on the board;
	if(dir == "-") {
		int i = col+num_tiles+prev_placed_squares; 
		while(i != board.getCols()) {
			if(isupper((board.getSquare(row,i))[0])) {
				Tile* newTile = new Tile((board.getSquare(row,i))[0], board.getPointVal(board.getSquare(row,i))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				i++;
				suffix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}
	else if(dir == "|") {
		int i = row+num_tiles+prev_placed_squares; 
		while(i != board.getRows()) {
			if(isupper((board.getSquare(i,col))[0])) {
				Tile* newTile = new Tile((board.getSquare(i,col))[0], board.getPointVal(board.getSquare(i,col))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				i++;
				suffix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}

	//checking for adjacent word
	bool adjacent_word = false;
	if(prefix > 0 || suffix > 0) { //if potential word has pre-existing tiles before or after it
		adjacent_word = true; //adjacent word is true
	}
	//checking for adjacent word
	if(count > 0) {
		if(dir == "-") {
			for(int e = 0; e < num_tiles; e++) {
				if(row+e < board.getRows() || row-e >= 0) { 
					if(isupper((board.getSquare(row+e, col))[0])) {
						adjacent_word = true;
						break;
					}
					else if(isupper((board.getSquare(row-e, col))[0])) {
						adjacent_word = true;
						break;
					}
				}
			}
		}
		else if(dir == "|") {
			for(int e = 0; e < num_tiles; e++) {
				if(col+e < board.getCols() || col-e >= 0) { 
					if(isupper((board.getSquare(row, col+e))[0])) {
						adjacent_word = true;
						break;
					}
					else if(isupper((board.getSquare(row, col+e))[0])) {
						adjacent_word = true;
						break;
					}
				}
			}
		}
	}
	//checking for adjacent word
	if(count > 0 && !adjacent_word) {
		cout << "You tried to make a move that does not have at least one tile adjacent to a previously placed tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return;
	}

	string potential_word = ""; //word to check against dictionary
	for(unsigned int x = 0; x < word.size(); x++) {
		potential_word += (word[x])->getLetter();
	}

	//Check that first move of the game is on the start square
	int word_len = (int) potential_word.size();
	if(count == 0) {
		if(dir == "-") {
			if(row != board.getStartRow()) {
				cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return;
			}
			else if(col < board.getStartCol()) {
				if(col + word_len <= board.getStartCol()) {
					cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					place_success = false; //flag for successful command set to false
					moveChecker(place_success); //move failed
					return;
				}
			}
			else if(col > board.getStartCol()) {
				cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return;
			}
			else {
				count++;
			}
		}
		else if(dir == "|") {
			if(col != board.getStartCol()) {
				cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return;
			}
			else if(row < board.getStartRow()) {
				if(row + word_len <= board.getStartRow()) {
					cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					place_success = false; //flag for successful command set to false
					moveChecker(place_success); //move failed
					return;
				}
			}
			else if(row > board.getStartRow()) {
				cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return;
			}
			else {
				count++;
			}
		}
	}
	
	int score = 0; //score for given move
	bool double_word = false; //flag for double word tile
	bool triple_word = false; //flag for triple letter tile
	transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::tolower); //converting string to lowercase
	if(dictionary.validWord(potential_word)) { //if valid word
		transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::toupper); //converting string to uppercase
		num_tiles = (int) potential_word.size();
		if(dir == "-") { //if player wants to place horizontally
			if(prefix > 0) {
				for(int i = col; i < (col+num_tiles-prefix); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];
					//linking value of letter to board so it can be referenced if used in another word later
					board.linkSquare(temp, (word[i-col+prefix])->getPoints()); 

					if(board.getSquare(row, i) == "2") { //checking for double letter tile
						score += (((word[i-col+prefix])->getPoints())*2);
					}
					else if(board.getSquare(row, i) == "3") { //checking for triple letter tile
						score += (((word[i-col+prefix])->getPoints())*3);
					}
					else {
						score += (word[i-col+prefix])->getPoints();
					}

					if(board.getSquare(row, i) == "t") { //checking for triple word tile
						triple_word = true;
					}
					else if(board.getSquare(row,i) == "d") { //checking for double word tile
						double_word = true;
					}
					
					board.updateSquare(temp, row, i); //add word to board
				}
			}
			else {
				for(int i = col; i < (col+num_tiles); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];
					//linking value of letter to board so it can be referenced if used in another word later
					board.linkSquare(temp, (word[i-col+prefix])->getPoints()); 

					if(board.getSquare(row, i) == "2") { //checking for double letter tile
						score += (((word[i-col+prefix])->getPoints())*2);
					}
					else if(board.getSquare(row, i) == "3") { //checking for triple letter tile
						score += (((word[i-col+prefix])->getPoints())*3);
					} 
					else {
						score += (word[i-col+prefix])->getPoints();
					}
					
					if(board.getSquare(row, i) == "t") { //checking for triple word tile
						triple_word = true;
					}
					else if(board.getSquare(row,i) == "d") { //checking for double word tile
						double_word = true;
					}

					board.updateSquare(temp, row, i); //add word to board
				}
			}
		}	
		else if(dir == "|") { //if player wants to place vertically
			if(prefix > 0) {
				for(int i = row; i < (row+num_tiles-prefix); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];
					//linking value of letter to board so it can be referenced if used in another word later
					board.linkSquare(temp, (word[i-row+prefix])->getPoints()); 

					if(board.getSquare(i, col) == "2") { //checking for double letter tile
						score += (((word[i-row+prefix])->getPoints())*2);
					}
					else if(board.getSquare(i, col) == "3") { //checking for triple letter tile
						score += (((word[i-row+prefix])->getPoints())*3);
					}
					else {
						score += (word[i-row+prefix])->getPoints();
					}

					if(board.getSquare(i, col) == "t") { //checking for triple word tile
						triple_word = true;
					}
					else if(board.getSquare(i, col) == "d") { //checking for double word tile
						double_word = true;
					}
					
					board.updateSquare(temp, i, col); //add word to board
				}
			}
			else {
				for(int i = row; i < (row+num_tiles); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];
					//linking value of letter to board so it can be referenced if used in another word later
					board.linkSquare(temp, (word[i-row+prefix])->getPoints()); 

					if(board.getSquare(i, col) == "2") { //checking for double letter tile
						score += (((word[i-row+prefix])->getPoints())*2);
					}
					else if(board.getSquare(i, col) == "3") { //checking for triple letter tile
						score += (((word[i-row+prefix])->getPoints())*3);
					}
					else {
						score += (word[i-row+prefix])->getPoints();
					}

					if(board.getSquare(i, col) == "t") { //checking for triple word tile
						triple_word = true;
					}
					else if(board.getSquare(i, col) == "d") { //checking for double word tile
						double_word = true;
					}
					
					board.updateSquare(temp, i, col); //add word to board
				}
			}	
		}
	}
	else { //invalid 
		cout << "You entered an invalid word, please enter a valid move:" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return;
	}

	//loop to erase tiles from user's hand
	int length = tiles_to_place.length(); //casting unsigned int to int
	for(int j = 0; j < length; j++) {
		
		set<Tile*>::iterator it; //iterator
		it = _hand.begin();
  		while(it != _hand.end()) { //loop to grab tiles from player's hand 
  			if((*it)->getLetter() == tiles_to_place[j]) { //if tile is found in set
    			_hand.erase(it++); //erasing tile from hand;
    			break;
    		}
    		else {
    			++it;
    		}	
  		}
  	}

  	if(question_mark == true) {
  		addToHand(bag.drawTiles(tiles_to_place.length()-1)); //minus one to account for the letter the question mark is used as
  	}
  	else {
  		addToHand(bag.drawTiles(tiles_to_place.length())); //drawing same number of tiles as placed onto board
  	}
  	moveChecker(place_success);

  	if(prefix > 0) { //adding pre-existing tile's values to player's score
  		for(int a = 0; a < prefix; a++) {
  			string temp = "";
  			temp += potential_word[a];
  			score += board.getPointVal(temp);
  		}
  	}

  	if(double_word == true) { //if double word flag is true
  		score = score*2;
  	}
  	else if(triple_word == true) { //if triple word flag is true
  		score = score*3;
  	}

  	//cleaning up word vector
	for (unsigned int h = 0; h < word.size(); ++h) {
		delete word[h];
	}

  	setScore(getScore()+score); //adding to player's score

}

void Player::setHand(set<Tile*> tiles) {
	_hand = tiles;
}

int Player::handSize() {
	return _hand.size();
}

void Player::printHand() {
	set<Tile*>::iterator it; //iterator

  	cout << "Current hand:";
  	for(it = _hand.begin(); it != _hand.end(); it++) {
    	cout << " " << (*it)->getLetter(); //grabbing letter from tile
	}

  cout << endl;
}

void Player::addToHand(set<Tile*> tiles) {
	set<Tile*>::iterator it; //iterator
  	for(it = tiles.begin(); it != tiles.end(); it++) { //loop through set of tiles to add
  		_hand.insert(*it); //insert them to player's hand
  	}
}

void Player::moveChecker(bool flag) {
	move_success = flag;
}

bool Player::getMoveChecker() {
	return move_success;
}