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
#include "Headers.h"
#include "Welcome.h"

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
	
		QApplication app(nargs, args); 
		Welcome w(&app, &board, &bag, &dict, numTiles); 
		w.show();
		return app.exec(); 
	}
	catch (invalid_argument & e)
	{ cout << "Fatal Error! " << e.what(); }

	return 1;
}


