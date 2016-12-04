CC       = g++
CPPFLAGS = -Wall -g
C11 = -std=c++11

all: counting

counting: Counting.cpp Hashtable.cpp
	$(CC) $(CPPFLAGS) $(C11) Counting.cpp Hashtable.cpp -o counting

.PHONY: clean

clean:
	rm counting