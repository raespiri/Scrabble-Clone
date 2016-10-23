/*
 * Bag.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// DO NOT EDIT!

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "Bag.h"
#include "Tile.h"

using namespace std;

Bag::Bag (const string bag_file_name, int random_seed)
{
	srand (random_seed);
	ifstream bagFile (bag_file_name.c_str());

	if (bagFile.is_open())
	{
		string line;
		while (getline (bagFile, line))
		{
			stringstream ss (line);
			char c;
			int points;
			int count;
			ss >> c >> points >> count;
			for (int i = 0; i < count; ++ i)
			{
				Tile *t = new Tile (toupper(c), points);
				addTile (t);
			}
		}
		bagFile.close ();
	}
	else throw invalid_argument ("Cannot open file: " + bag_file_name);

}

Bag::~Bag ()
{
	for (unsigned int i = 0; i < _tiles.size(); ++i)
		delete _tiles [i];
}

void Bag::addTile (Tile *tile)
{
	_tiles.push_back (tile);
	int j = rand() % _tiles.size();
	_tiles[_tiles.size()-1] = _tiles[j];
	_tiles[j] = tile;
}

void Bag::addTiles (vector<Tile*> tiles)
{
	for (vector<Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
		addTile (*it);
}

std::set<Tile*> Bag::drawTiles (int number)
{
	set<Tile*> tileSet;
	for (int i = 0; i < number && tilesRemaining() > 0; ++i)
	{
		tileSet.insert (_tiles.back());
		_tiles.pop_back();
	}
	return tileSet;
}

int Bag::tilesRemaining () const
{ return _tiles.size(); }
