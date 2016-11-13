/*
 * Tile.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// DO NOT EDIT!

#ifndef TILE_H_
#define TILE_H_

class Tile {
public:
	Tile (char letter, int points)
	{ _letter = letter; _points = points; _use = letter; }

	char getLetter () const { return _letter; }

	int getPoints () const { return _points; }

	bool isBlank () const { return (_letter == '?'); }

	char getUse () const { return _use; }
	void useAs (char use) { _use = use; }

private:
	char _letter;
	char _use; // for blank tiles only
	int _points;
};


#endif /* TILE_H_ */
