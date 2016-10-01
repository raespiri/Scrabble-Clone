CC       = g++
CPPFLAGS = -Wall -g
GTEST_LL = -I /usr/include/gtest/ -l gtest -l gtest_main -pthread

llisttest: lliststring.cpp llisttest.cpp
	$(CC) $(CPPFLAGS) lliststring.cpp llisttest.cpp $(GTEST_LL) -o llisttest

stringparser: stringparser.cpp stackstring.cpp lliststring.cpp
	$(CC) $(CPPFLAGS) stackstring.cpp lliststring.cpp stringparser.cpp -o $@

.PHONY: clean

clean:
	rm llisttest
	rm stringparser