#include <iostream>
#include <sstream>
#include <cstring> 
#include <fstream>

using namespace std;

int main (int argc, char* argv[])
{
	if(argc < 2) 
	{
    	cout << "Please provide a filename." << endl;
    	return 1;
  	}
  
  	ifstream ifile(argv[1]);
	if(ifile.fail())
	{
		cout << "Couldn't open file" << endl;
		return 1;
	}
	
	int count;
	ifile >> count;
	char* words[count]; //array of all the words in the text file
	char temp[50]; //buffer for individual words
	
	for(int i = 0; i < count; i++)
	{
		ifile >> temp; //getting input from file
		words[i] = new char[strlen(temp)+1]; //copying input from file in char*
		strcpy(words[i], temp);
	}
	//loop to output words in reverse order
	for(int i = count - 1; i >= 0; i--) 	
	{
		cout << words[i] << endl;
	}

	for(int i = 0; i < count; i++)
	{
		delete [] words[i];
	}

	return 0;
}