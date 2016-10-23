/*
 * Bag.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// DO NOT EDIT!

#ifndef BAG_H_
#define BAG_H_

#include <vector>
#include <string>
#include <set>
#include "Tile.h"

class Bag {

public:
	Bag (const std::string bag_file_name, int random_seed);
	~Bag ();

	void addTile (Tile *tile);
	void addTiles (std::vector<Tile*> tiles);
	std::set<Tile*> drawTiles (int number);
	int tilesRemaining () const;

private:
	std::vector<Tile*> _tiles;
};


#endif /* BAG_H_ */
