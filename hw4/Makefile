CC       = g++
CPPFLAGS = -Wall -g

all: Scrabble

Player.o: Player.h Player.cpp
	$(CXX) $(CPPFLAGS) -c Player.cpp -o Player.o

Bag.o: Bag.h Bag.cpp
	$(CXX) $(CPPFLAGS) -c Bag.cpp -o Bag.o

Board.o: Board.h Board.cpp
	$(CXX) $(CPPFLAGS) -c Board.cpp -o Board.o

Dictionary.o: Dictionary.h Dictionary.cpp
	$(CXX) $(CPPFLAGS) -c Dictionary.cpp -o Dictionary.o

Scrabble: Scrabble.cpp Player.o Board.o Bag.o Tile.h Dictionary.o
	$(CC) $(CPPFLAGS) Scrabble.cpp Player.o Board.o Bag.o Tile.h Dictionary.o -o Scrabble

.PHONY: clean

clean:
	rm Scrabble