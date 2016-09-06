#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 3){
    cerr << "Please provide an input and output file." << endl;
    return 1;
  }
  ifstream input(argv[1]);
  ofstream output(argv[2]);

  int constructionspots;
  int *buildingheights;
  string **skyline;
  string curr;

  input >> constructionspots;
  getline(input, curr); //consume the newline character for the first line.
  skyline = new string*[constructionspots];

  //you will need to keep track of the height of each building
  buildingheights = new int[constructionspots];

  for (int i = 0; i < constructionspots; i++) {
	  buildingheights[i] = 0;
	  skyline[i] = NULL;
  }

  while(getline(input, curr)) {
	  stringstream ss;
	  ss << curr;
	  ss >> curr;
	  if (curr == "BUILD") {
		  int x,y;
		  string c;
		  ss >> x;
		  ss >> y;
		  ss >> c;
		  string extra; //variable to check for incorrect command
		  if (ss.fail() || skyline[x] != NULL || x < 0 || x >= constructionspots || ss >> extra) {
			  output << "Error - incorrect command" << endl;
		  }
		  else {
		  	buildingheights[x] = y; //creating height of building
		  	skyline[x] = new string[y + 1];
		  	//loop to set all the floors of building at construction spot 'x' to color 'c'
		  	for(int i = 0; i <= y; i++)
		  	{
		  		skyline[x][i] = c; 
		  	}
		  }
	  }
	  else if (curr == "EXTEND") {
	  	int x,y;
		string c;
		ss >> x;
		ss >> y;
		ss >> c;
		string extra; //variable to check for incorrect command
		if (ss.fail() || skyline[x] == NULL || x < 0 || x >= constructionspots || ss >> extra) {
			output << "Error - incorrect command" << endl;
		}
	  	else {
	  		int old = buildingheights[x]; //saving old building heigth
	  		string *temp = new string[old + 1];
	  		//saving old floor colors into temp array
	  		for(int i = 0; i <= old; i++)
	  		{
	  			temp[i] = skyline[x][i];
	  		}

	  		buildingheights[x] = buildingheights[x] + y; //setting new height
	  		delete [] skyline[x]; //deleting old dynamic array
	  		skyline[x] = new string[buildingheights[x] + 1]; //creating new dynamic array with extended size
	  		
	  		//loop to copy over old floor colors
	  		for(int i = 0; i <= old; i++)
	  		{
	  			skyline[x][i] = temp[i];
	  		}
	  		delete [] temp;
	  		//loop to add additional floors with correct color
	  		for(int i = old + 1; i <= buildingheights[x]; i++)
	  		{
	  			skyline[x][i] = c;
	  		}
	  	}
	  }
	  else if (curr == "DEMOLISH") {
	  	int x;
		ss >> x;
		string extra; //variable to check for incorrect command
		if (ss.fail() || skyline[x] == NULL || x < 0 || x >= constructionspots || ss >> extra) {
			output << "Error - incorrect command" << endl;
		}
	  	else {
	  		buildingheights[x] = 0; //setting building height at site x to 0
	  		delete [] skyline[x]; //deleting building at site x and setting point back to NULL
	  		skyline[x] = NULL;
	  	}
	  }
	  else if (curr == "SKYLINE") {
	  	int y;
	  	ss >> y;
		string extra; //variable to check for incorrect command
	  	if(ss.fail() || y < 1 || ss >> extra)
	  	{
	  		output << "Error - incorrect command" << endl;
	  	}
	  	else {
	  		for(int i = 0; i < constructionspots; i++)
	  		{
	  			if(skyline[i] == NULL || buildingheights[i] < y) //if no building at the spot exists or building is too short ouput SKY
	  			{
	  				output << "SKY ";
	  			}
	  			else
	  			{
	  				output << skyline[i][y] + " ";
	  			}
	  		}
	  		output << endl;
	  	}
  	  }
  	  else {
  	  	output << "Error - incorrect command" << endl;
  	  }
  }
  //clean-up
  for(int i = 0; i < constructionspots; ++i) 
  {
    delete [] skyline[i];
  }
   delete [] skyline;
   delete [] buildingheights;

  return 0;
}
