CC       = g++
CPPFLAGS = -Wall -g
C11 = -std=c++11

all: counting

counting: counting.cpp Hashtable.cpp
	$(CC) $(CPPFLAGS) $(C11) counting.cpp Hashtable.cpp -o counting

.PHONY: clean

clean:
	rm counting