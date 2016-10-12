/*
 * Scrabble.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include "Player.h"
#include "Tile.h"
#include "Dictionary.h"
#include "Bag.h"
#include "Board.h"
#include <limits>
#include <ctime>

using namespace std;

void readConfigFile (string config_file_name,
					 string & dictionary_file_name,
					 string & board_file_name,
					 string & bag_file_name,
					 unsigned int & hand_size)
{
	ifstream configFile (config_file_name.c_str());
	string line;
    bool number = false, board = false, tiles = false, dictionary = false;

	if (!configFile.is_open())
		throw invalid_argument("Cannot open file: " + config_file_name);
	while (getline (configFile, line))
	{
		stringstream ss (line);
		string parameter;
		ss >> parameter;
		if (parameter == "NUMBER:")
			{ ss >> hand_size; number = true; }
		else if (parameter == "BOARD:")
		    { ss >> board_file_name; board = true; }
		else if (parameter == "TILES:")
			{ ss >> bag_file_name; tiles = true; }
		else if (parameter == "DICTIONARY:")
			{ ss >> dictionary_file_name; dictionary = true; }
	}
	if (!number)
		throw invalid_argument("Hand size not specified in config file");
	if (!board)
		throw invalid_argument("Board file name not specified in config file");
	if (!tiles)
		throw invalid_argument("Bag file name not specified in config file");
	if (!dictionary)
		throw invalid_argument("Dictionary file name not specified in config file");
}

bool emptyHand(vector<Player*> player_list) { //bool function to check if a player has an empty hand
	for(unsigned int i = 0; i < player_list.size(); i++) {
		if(player_list[i]->handSize() == 0) { //if a player has a handsize of 0 return true
			return true;
		}
	}
	return false;
}

int main (int nargs, char **args)
{
	if (nargs < 2 || nargs > 2)
		cout << "Usage: Scrabble <config-filename>\n";
	try {
		string dictionaryFileName, boardFileName, bagFileName;
		unsigned int numTiles;

		readConfigFile (args[1],
						dictionaryFileName, boardFileName, bagFileName,
						numTiles);

		Dictionary dict (dictionaryFileName);
		Board board (boardFileName);
		Bag bag (bagFileName, time(0)); // second argument is random seed using time to get a distinctive random val

		// Good luck!

		int num_players;
		cout << "How many players (between 1 - 8) do you wish to have?" << endl;
		cin >> num_players; //getting user input on number of plays
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear the new line character from stream

		vector<Player*> playerList; //vector of player pointers

		// PLAYER NAME SETUP
		for(int i = 0; i < num_players; i++) {
			string name;
			cout << "Please enter player " << i+1 << "'s name:" << endl;
			getline(cin, name); //getting user input for each player's name
			Player* new_player = new Player(name);
			new_player->setHand(bag.drawTiles(numTiles)); //drawing tiles from bag to set new player's hand
			playerList.push_back(new_player); //adding new player to vector
		}

		// SCRABBLE GAME LOOP UNTIL SOMEONE WINS 
		int turn_counter = 0; //counter for number of turns, used for start square check
		while(bag.tilesRemaining() > 0 && !emptyHand(playerList)) { //while there are still tiles in the bag and no one has an empty hand
			
			int all_pass_counter = 0; //counter to check if all the users pass in the same turn

			for(unsigned int i = 0; i < playerList.size(); i++) { //loop through all the players
				cout << "Player " << i+1 << " it's your turn!" << endl;
				cout << "Current state of the board:" << endl;
				board.printBoard(); //printing current state of the board
				for(unsigned int j = 0; j < playerList.size(); j++) {
					cout << "Player " << j+1 << "'s score is: " << playerList[j]->getScore() << endl; //printing out the score for each player
				}
				int player_index = (int) i; //casting unsigned int to int
				playerList[player_index]->printHand(); //printing curr player's hand
				
				// QUERYING THE USER FOR MOVE INPUT
				while(playerList[i]->getMoveChecker() == false) { //while move input failed
					cout << "Please enter your move:" << endl;
					string move; //string to hold player move
					cin >> move;
					transform(move.begin(), move.end(), move.begin(), ::toupper); //converting move to uppercase for consistency 
				
					if(move == "PASS") { //if move is pass
						playerList[i]->movePass();
						all_pass_counter++; //increment pass counter
					}
					else if(move == "EXCHANGE") { //if move is exchange
						string letters; //letters to exchange
						cin >> letters;
						playerList[i]->moveExchange(letters, bag);
					}
					else if(move == "PLACE") { //if move is place
						string dir; //direction to place letters
						int row; //row to place letters
						int col; //col to place letters
						string letters; //letter to place
						cin >> dir;
						cin >> row;
						cin >> col;
						cin >> letters;
						playerList[i]->movePlace(dir, row, col, letters, board, dict, bag, turn_counter);
						if(letters.size() == numTiles) { //if player used all the tiles in his/her hand
							playerList[i]->setScore(playerList[i]->getScore() + 50); //add 50 to the score
						}
					}
				}
				playerList[i]->moveChecker(false); //setting move checker back to false after tunr is over
				//if you use k tiles that's an extra 50 points
			}
			
			if(all_pass_counter == num_players) { //if all the players passed
				return 0; //end the game
			}
		}	

		//cleaning up playerlist vector
		for (unsigned int j = 0; j < playerList.size(); ++j) {
			delete playerList[j];
		}

		return 0;
	}
	catch (invalid_argument & e)
	{ cout << "Fatal Error! " << e.what(); }

	

	return 1;
}
