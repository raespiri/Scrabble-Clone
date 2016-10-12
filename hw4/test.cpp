#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	string x = "dictionary.txt";
	Dictionary dict(x);
	dict.findWord("aah");

	return 0;
}