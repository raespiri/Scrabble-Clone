#include <string>
#include "Player.h"

using namespace std;

Player::Player(string name) {
	_name = name;
	_score = 0; 
	move_success = false;
}

Player::Player(string name, Dictionary* temp_dict) {
	_name = name;
	_score = 0; 
	move_success = false;
	player_dict = temp_dict;
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

	set<Tile*> temp_hand(_hand); //copy of current user's hand

	//checking if tiles user proposes to throw away are in his/her hand
	int num_tiles = (int) old_tiles.length(); //casting unsigned int of string size to int 
  	for(int i = 0; i < num_tiles; i++) { //loop through tiles 
  		
  		bool found = false; //flag to check if letter was found
  		set<Tile*>::iterator it; //iterator

  		for(it = temp_hand.begin(); it != temp_hand.end(); ++it) { //loop through hand
    		if((*it)->getLetter() == old_tiles[i]) { //if tile is found in set
    			found = true; //found tile in player's hand
    			temp_hand.erase(it); //erase it from the temp hand
    			break;
    		} 
		}

		if(found == false) { //if tile was not found in player's hand
			//cout << "You do not have the tile " << old_tiles[i] << " in your hand, please enter a valid move:" << endl;
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
	
	moveChecker(exchange_success);
	addToHand(bag.drawTiles(num_tiles)); //drawing same number of tiles as exchanged back into bag
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
		for(int i = col; i < (col+num_tiles+prev_placed_squares) && i < board.getCols(); i++) {
			old_squares += board.getSquare(row, i); //add square from board to string of squares
			if(isupper((board.getSquare(row,i))[0])) { //if a previously placed tile is found
				prev_placed_squares++; //increment
			}
		}
	}
	else if(dir == "|") {
		for(int i = row; i < (row+num_tiles+prev_placed_squares) && i < board.getRows(); i++) {
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
		if(dir == "|") {
			for(int e = 0; e < num_tiles; e++) {
				if(row+e < board.getRows() && row-e >= 0) { 
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
		else if(dir == "-") {
			for(int e = 0; e < num_tiles; e++) {
				if(col+e < board.getCols() && col-e >= 0) { 
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

  	count++;
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

string Player::getName() {
	return _name;
}

bool Player::AImovePlace(string dir, int row, int col, string tiles_to_place, Board &board, int &count) {
	
	int num_tiles = (int) tiles_to_place.length(); //casting unsigned int of string size to int 

	bool place_success = true; //flag to check if exchange succeeded

	//If one or more of the tiles placed would be out of bounds of the board
	if(dir == "-") {
		if(row < 0 || row >= board.getRows() || col < 0 || col > board.getCols()-num_tiles+1) {
			//cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}
	}
	else if(dir == "|") {
		if(row < 0 || row > board.getRows()-num_tiles+1 || col < 0 || col >= board.getCols()) {
			//cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}
	}

	//If user is trying to place a word starting at a position that is already occupied by a tile. 
	if(isupper((board.getSquare(row,col))[0])) {
		//cout << "You are trying to place a word starting at a position that is already occupied by a tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

	//If user is trying to place a word starting at a position that is already occupied by a tile. 
	if(isupper((board.getSquare(row,col))[0])) {
		//cout << "You are trying to place a word starting at a position that is already occupied by a tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

	transform(tiles_to_place.begin(), tiles_to_place.end(), tiles_to_place.begin(), ::toupper); //convert tiles to uppercase for
		
	vector<Tile*> word; //set of tiles which make up the word to place on the board

	bool question_mark = false;	

	string temp_hand = "";
	set<Tile*>::iterator it; //iterator
	for(it = _hand.begin(); it != _hand.end(); it++) {
		temp_hand += (*it)->getLetter();
	}
	//Loop to check if user has desired tiles to place in his/her hand
	bool found;
	for(int i = 0; i < num_tiles; i++) { //loop through tiles to place
  		
  		found = false; //flag to check if letter was found
  		set<Tile*>::iterator it; //iterator
  		for(int j = 0; j < (int) temp_hand.size(); j++) { //loop to grab tiles from player's hand
    		if(temp_hand[j] == tiles_to_place[i]) { //if tile is found in set
    			if(tiles_to_place[i] == '?') { //if user wishes to use a blank tile
    				return false;
    			}
    			else {
    				set<Tile*>::iterator it; //iterator
    				for(it = _hand.begin(); it != _hand.end(); ++it) {
    					if((*it)->getLetter() == tiles_to_place[i]) {
    						word.push_back(*it); //adding tile to word set
    						break;
    					}
    				}
    				temp_hand.erase(j,1);
    				found = true; //found tile in player's hand
    				break;
    			}
    		} 
		}

		if(found == false) { //if tile was not found in player's hand
			//cout << "You do not have the tile " << tiles_to_place[i] << " in your hand, please enter a valid move:" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}	
	}	

	//grabbing values of squares where user is proposing to place his/her word
	string old_squares = ""; //string of old squares to place tiles on
	int prev_placed_squares = 0;
	if(dir == "-") {
		for(int i = col; i < (col+num_tiles+prev_placed_squares) && i < board.getCols(); i++) {
			old_squares += board.getSquare(row, i); //add square from board to string of squares
			if(isupper((board.getSquare(row,i))[0])) { //if a previously placed tile is found
				prev_placed_squares++; //increment
			}
		}
	}
	else if(dir == "|") {
		for(int i = row; i < (row+num_tiles+prev_placed_squares) && i < board.getRows(); i++) {
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
		while(i < board.getCols()) {
			if(isupper((board.getSquare(row,i))[0])) {
				Tile* newTile = new Tile((board.getSquare(row,i))[0], board.getPointVal(board.getSquare(row,i))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				++i;
				suffix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}
	else if(dir == "|") {
		int i = row+num_tiles+prev_placed_squares; 
		while(i < board.getRows()) {
			if(isupper((board.getSquare(i,col))[0])) {
				Tile* newTile = new Tile((board.getSquare(i,col))[0], board.getPointVal(board.getSquare(i,col))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				++i;
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
		if(dir == "|") {
			for(int e = 0; e < num_tiles; e++) {
				if(row+e < board.getRows() && row-e >= 0) { 
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
		else if(dir == "-") {
			for(int e = 0; e < num_tiles; e++) {
				if(col+e < board.getCols() && col-e >= 0) { 
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
		//cout << "You tried to make a move that does not have at least one tile adjacent to a previously placed tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
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
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
			else if(col < board.getStartCol()) {
				if(col + word_len <= board.getStartCol()) {
					//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					return false;
				}
			}
			else if(col > board.getStartCol()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
		}
		else if(dir == "|") {
			if(col != board.getStartCol()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
			else if(row < board.getStartRow()) {
				if(row + word_len <= board.getStartRow()) {
					//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					place_success = false; //flag for successful command set to false
					moveChecker(place_success); //move failed
					return false;
				}
			}
			else if(row > board.getStartRow()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
		}
	}
	
	int score = 0; //score for given move
	bool double_word = false; //flag for double word tile
	bool triple_word = false; //flag for triple letter tile
	transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::tolower); //converting string to lowercase
	if(player_dict->validWord(potential_word)) { //if valid word
		transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::toupper); //converting string to uppercase
		num_tiles = (int) potential_word.size();
		if(dir == "-") { //if player wants to place horizontally
			if(prefix > 0) {
				for(int i = col; i < (col+num_tiles-prefix) && i < board.getCols(); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];

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
					
				}
			}
			else {
				for(int i = col; i < (col+num_tiles) && i < board.getCols(); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];

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
				}
			}
		}	
		else if(dir == "|") { //if player wants to place vertically
			if(prefix > 0) {
				for(int i = row; i < (row+num_tiles-prefix) && i < board.getRows(); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];

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
					
				}
			}
			else {
				for(int i = row; i < (row+num_tiles) && i < board.getRows(); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];

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
					
				}
			}	
		}
	}
	else { //invalid 
		//cout << "You entered an invalid word, please enter a valid move:" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

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

  	AIsetScore(score); //updating AI score
  	return true;
}

string Player::getHand() {
	set<Tile*>::iterator it; //iterator
	string hand = ""; //empty hand

  	for(it = _hand.begin(); it != _hand.end(); it++) {
    	hand += (*it)->getLetter(); //grabbing letter from tile and adding it to hand string
	}
	return hand;
}

void Player::AIsetScore(int score) {
	AIscore = score;
}

int Player::AIgetScore() {
	return AIscore;
}

Move* Player::AIgetMove() {
	Move* best_move; //string of tiles for best_move

	int score = -1; //max score
	int best_index = -1; //index of best move
	for(int i = 0; i < (int) AImoves.size(); i++) { //iterate through set of possible moves
		if(AImoves[i].score > score) {
			score = AImoves[i].score;
			best_index = i; 
		}
	}
	best_move = &AImoves[best_index];

	return best_move;
}

void Player::AIsolveHelper(int row, int col, string letters, string hand, Board &board, int &count) {
 	//base case if hand is empty
	if(hand.size() == 0) {
		return;
	}
	//base case if square is already occupied
	if(board.getSquare(row,col) != "d" && board.getSquare(row,col) != "t" && 
	   board.getSquare(row,col) != "2" && board.getSquare(row,col) != "3" && 
	   board.getSquare(row,col) != ".") {
		return;
	}
	else {
		string temp = letters; //string of initial letters
		string temp_hand = hand;
		
		for(int i=0; i < (int) hand.size(); i++) { //loop through hand size
			letters = temp;
			letters += hand[i]; //draw first tile from hand
			
			hand = temp_hand;
			hand.erase(0,1); //erase from hand
			
			transform(letters.begin(), letters.end(), letters.begin(), ::tolower);
			string temp = letters;
			transform(letters.begin(), letters.end(), letters.begin(), ::toupper);
			
			if(player_dict->validPrefix(temp) && used_prefixes.find(temp) == used_prefixes.end()) { //if unused valid prefix
 				used_prefixes.insert(temp); //add to set of prefixes
				if(AImovePlace("-", row, col, letters, board, count)) { //check if it can be placed at current square
					Move temp_move("-", row, col, letters, AIgetScore());
					AImoves.push_back(temp_move); //add to vector of valid moves
				}
				if(AImovePlace("|", row, col, letters, board, count)) { //check if it can be placed to the left
					Move temp_move("|", row, col, letters, AIgetScore());
					AImoves.push_back(temp_move); //add to vector of valid moves
				}
				if((int) letters.size() != handSize()) { //add next letter from hand to current string if there are tiles left
					AIsolveHelper(row, col, letters, hand, board, count); //recursive call to add on to word
				}
			}
		}
		used_prefixes.clear();
		return;
	}
}

void Player::AIsolve(Board &board, int &count) {
	if(AImoves.size() != 0) { //clears vector of AImoves if still full;
		AImoves.clear();
	}
	for(int i = 0; i < board.getCols(); i++) {
		for(int j = 0; j < board.getRows(); j++) {
			AIsolveHelper(i, j, "", getHand(), board, count);
		}
	}
}

int Player::getAImoves_size() {
	return AImoves.size();
}


void Player::AIsetScore2(int score) {
	AIscore2 = score;
}

int Player::AIgetScore2() {
	return AIscore2;
}

Move* Player::AIgetMove2() {
	Move* best_move; //string of tiles for best_move

	int score = -1; //max score
	int best_index = -1; //index of best move
	for(int i = 0; i < (int) AImoves2.size(); i++) { //iterate through set of possible moves
		if(AImoves2[i].score > score) {
			score = AImoves2[i].score;
			best_index = i; 
		}
	}
	best_move = &AImoves2[best_index];

	return best_move;
}

void Player::AIsolveHelper2(int row, int col, string letters, string hand, Board &board, int &count) {
 	//base case if hand is empty
	if(hand.size() == 0) {
		return;
	}
	//base case if square is already occupied
	if(board.getSquare(row,col) != "d" && board.getSquare(row,col) != "t" && 
	   board.getSquare(row,col) != "2" && board.getSquare(row,col) != "3" && 
	   board.getSquare(row,col) != ".") {
		return;
	}
	else {
		string temp = letters; //string of initial letters
		string temp_hand = hand;
		
		for(int i=0; i < (int) hand.size(); i++) { //loop through hand size
			letters = temp;
			letters += hand[i]; //draw first tile from hand
			
			hand = temp_hand;
			hand.erase(0,1); //erase from hand
			
			transform(letters.begin(), letters.end(), letters.begin(), ::tolower);
			string temp = letters;
			transform(letters.begin(), letters.end(), letters.begin(), ::toupper);
			
			if(player_dict->validPrefix(temp) && used_prefixes2.find(temp) == used_prefixes2.end()) { //if unused valid prefix
 				used_prefixes2.insert(temp); //add to set of prefixes
				if(AImovePlace2("-", row, col, letters, board, count)) { //check if it can be placed at current square
					Move temp_move("-", row, col, letters, AIgetScore2());
					AImoves2.push_back(temp_move); //add to vector of valid moves
				}
				if(AImovePlace2("|", row, col, letters, board, count)) { //check if it can be placed to the left
					Move temp_move("|", row, col, letters, AIgetScore2());
					AImoves2.push_back(temp_move); //add to vector of valid moves
				}
				if((int) letters.size() != handSize()) { //add next letter from hand to current string if there are tiles left
					AIsolveHelper2(row, col, letters, hand, board, count); //recursive call to add on to word
				}
			}
		}
		used_prefixes2.clear();
		return;
	}
}

void Player::AIsolve2(Board &board, int &count) {
	if(AImoves2.size() != 0) { //clears vector of AImoves if still full;
		AImoves2.clear();
	}
	for(int i = 0; i < board.getCols(); i++) {
		for(int j = 0; j < board.getRows(); j++) {
			AIsolveHelper2(i, j, "", getHand(), board, count);
		}
	}
}

int Player::getAImoves_size2() {
	return AImoves2.size();
}

bool Player::AImovePlace2(string dir, int row, int col, string tiles_to_place, Board &board, int &count) {
	
	int num_tiles = (int) tiles_to_place.length(); //casting unsigned int of string size to int 

	bool place_success = true; //flag to check if exchange succeeded

	//If one or more of the tiles placed would be out of bounds of the board
	if(dir == "-") {
		if(row < 0 || row >= board.getRows() || col < 0 || col > board.getCols()-num_tiles+1) {
			//cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}
	}
	else if(dir == "|") {
		if(row < 0 || row > board.getRows()-num_tiles+1 || col < 0 || col >= board.getCols()) {
			//cout << "One or more of the tiles placed would be out of bounds of the board" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}
	}

	//If user is trying to place a word starting at a position that is already occupied by a tile. 
	if(isupper((board.getSquare(row,col))[0])) {
		//cout << "You are trying to place a word starting at a position that is already occupied by a tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

	//If user is trying to place a word starting at a position that is already occupied by a tile. 
	if(isupper((board.getSquare(row,col))[0])) {
		//cout << "You are trying to place a word starting at a position that is already occupied by a tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

	transform(tiles_to_place.begin(), tiles_to_place.end(), tiles_to_place.begin(), ::toupper); //convert tiles to uppercase for
		
	vector<Tile*> word; //set of tiles which make up the word to place on the board

	bool question_mark = false;	

	string temp_hand = "";
	set<Tile*>::iterator it; //iterator
	for(it = _hand.begin(); it != _hand.end(); it++) {
		temp_hand += (*it)->getLetter();
	}
	//Loop to check if user has desired tiles to place in his/her hand
	bool found;
	for(int i = 0; i < num_tiles; i++) { //loop through tiles to place
  		
  		found = false; //flag to check if letter was found
  		set<Tile*>::iterator it; //iterator
  		for(int j = 0; j < (int) temp_hand.size(); j++) { //loop to grab tiles from player's hand
    		if(temp_hand[j] == tiles_to_place[i]) { //if tile is found in set
    			if(tiles_to_place[i] == '?') { //if user wishes to use a blank tile
    				return false;
    			}
    			else {
    				set<Tile*>::iterator it; //iterator
    				for(it = _hand.begin(); it != _hand.end(); ++it) {
    					if((*it)->getLetter() == tiles_to_place[i]) {
    						word.push_back(*it); //adding tile to word set
    						break;
    					}
    				}
    				temp_hand.erase(j,1);
    				found = true; //found tile in player's hand
    				break;
    			}
    		} 
		}

		if(found == false) { //if tile was not found in player's hand
			//cout << "You do not have the tile " << tiles_to_place[i] << " in your hand, please enter a valid move:" << endl;
			place_success = false; //flag for successful command set to false
			moveChecker(place_success); //move failed
			return false;
		}	
	}	

	//grabbing values of squares where user is proposing to place his/her word
	string old_squares = ""; //string of old squares to place tiles on
	int prev_placed_squares = 0;
	if(dir == "-") {
		for(int i = col; i < (col+num_tiles+prev_placed_squares) && i < board.getCols(); i++) {
			old_squares += board.getSquare(row, i); //add square from board to string of squares
			if(isupper((board.getSquare(row,i))[0])) { //if a previously placed tile is found
				prev_placed_squares++; //increment
			}
		}
	}
	else if(dir == "|") {
		for(int i = row; i < (row+num_tiles+prev_placed_squares) && i < board.getRows(); i++) {
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
		while(i < board.getCols()) {
			if(isupper((board.getSquare(row,i))[0])) {
				Tile* newTile = new Tile((board.getSquare(row,i))[0], board.getPointVal(board.getSquare(row,i))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				++i;
				suffix++; //increment length of prefix string
			}
			else {
				break;
			}
		}
	}
	else if(dir == "|") {
		int i = row+num_tiles+prev_placed_squares; 
		while(i < board.getRows()) {
			if(isupper((board.getSquare(i,col))[0])) {
				Tile* newTile = new Tile((board.getSquare(i,col))[0], board.getPointVal(board.getSquare(i,col))); //creating tile for letter already placed on board
				word.insert(word.end(), newTile); //inserting new tile to word vectior
				++i;
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
		if(dir == "|") {
			for(int e = 0; e < num_tiles; e++) {
				if(row+e < board.getRows() && row-e >= 0) { 
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
		else if(dir == "-") {
			for(int e = 0; e < num_tiles; e++) {
				if(col+e < board.getCols() && col-e >= 0) { 
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
		//cout << "You tried to make a move that does not have at least one tile adjacent to a previously placed tile" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
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
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
			else if(col < board.getStartCol()) {
				if(col + word_len <= board.getStartCol()) {
					//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					return false;
				}
			}
			else if(col > board.getStartCol()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
		}
		else if(dir == "|") {
			if(col != board.getStartCol()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
			else if(row < board.getStartRow()) {
				if(row + word_len <= board.getStartRow()) {
					//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
					place_success = false; //flag for successful command set to false
					moveChecker(place_success); //move failed
					return false;
				}
			}
			else if(row > board.getStartRow()) {
				//cout << "For the first move of the game, one of your tiles must be on the start square" << endl;
				place_success = false; //flag for successful command set to false
				moveChecker(place_success); //move failed
				return false;
			}
		}
	}
	
	int score = 0; //score for given move
	bool double_word = false; //flag for double word tile
	bool triple_word = false; //flag for triple letter tile
	transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::tolower); //converting string to lowercase
	if(player_dict->validWord(potential_word)) { //if valid word
		transform(potential_word.begin(), potential_word.end(), potential_word.begin(), ::toupper); //converting string to uppercase
		num_tiles = (int) potential_word.size();
		if(dir == "-") { //if player wants to place horizontally
			if(prefix > 0) {
				for(int i = col; i < (col+num_tiles-prefix) && i < board.getCols(); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];

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
					
				}
			}
			else {
				for(int i = col; i < (col+num_tiles) && i < board.getCols(); i++) {
					string temp = "";
					temp += potential_word[i-col+prefix];

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
				}
			}
		}	
		else if(dir == "|") { //if player wants to place vertically
			if(prefix > 0) {
				for(int i = row; i < (row+num_tiles-prefix) && i < board.getRows(); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];

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
					
				}
			}
			else {
				for(int i = row; i < (row+num_tiles) && i < board.getRows(); i++) {
					string temp = "";
					temp += potential_word[i-row+prefix];

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
					
				}
			}	
		}
	}
	else { //invalid 
		//cout << "You entered an invalid word, please enter a valid move:" << endl;
		place_success = false; //flag for successful command set to false
		moveChecker(place_success); //move failed
		return false;
	}

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

  	AIsetScore2((int) tiles_to_place.size()); //updating AI score
  	return true;
}