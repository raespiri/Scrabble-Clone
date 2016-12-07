#include "Hashtable.h"
#include <ctime>
#include <algorithm>
#include <math.h>
#include <sstream>

Hashtable::Hashtable(bool debug, unsigned int probing) {
	Debug = debug;
	Probing = probing;
	//set inital values of hash flags to false
	linear = false;
	quadratic = false;
	doublehash = false;

	if(Debug == true) { //if debug-mode is turned on
		//use prescribed random numbers from assignment page
		r1 = 983132572;
		r2 = 1468777056;
		r3 = 552714139;
		r4 = 984953261;
		r5 = 261934300;
	}
	else { //else debug-mode is turned off
		srand(time(NULL)); //set random seed to null
		r1 = rand() % 1685759167; //generate number between 0 and 1685759166
		r2 = rand() % 1685759167; //generate number between 0 and 1685759166
		r3 = rand() % 1685759167; //generate number between 0 and 1685759166
		r4 = rand() % 1685759167; //generate number between 0 and 1685759166
		r5 = rand() % 1685759167; //generate number between 0 and 1685759166
	}

	if(Probing == 0) { //Linear Probing
		linear = true; 
	}
	else if(Probing == 1) { //Quadratic Probing
		quadratic = true;
	}
	else { //Double hashing
		doublehash = true;
	}

	hashArr = new std::pair<std::string,int> [11]; //creating new array of size 11 for hash table
	for(int i = 0; i < 11; i++) { //loop to set initial values of hash array
		hashArr[i] = std::make_pair("#", 0); //set initial key value for all pairs to a "garbage" string; values are set to 0
	}

	size = 11; //update size of hashArr
	num_elements = 0; //number of elements initially equals 0
	loadFactor = num_elements/size; //find load factor by dividing number of elements by size
}

Hashtable::~Hashtable() {
	delete [] hashArr; //deleting hashtable array
}

void Hashtable::add(const std::string& k) {
	if(loadFactor >= 0.5) { //if load factor is greater than or equal to 0.5
		resize(); //call resize 
	}

	if(linear) { //if linear flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert 
		bool success = false; //successful insert flag
		
		while(success == false) { //loop while unsuccessful
			index = (h+i)%size; //set index equal to linear probing formula
			if(hashArr[index].first == k) { //if key is already in the table
				hashArr[index].second++; //increment value
				break;
			}
			else if(hashArr[index].first == "#") { //if hashtable index is not filled
				hashArr[index] = std::make_pair(k,1); //add pairing of key and value to hashtable
				success = true; //set success flag to true
				num_elements++; //increment number of elements in the hash table
				loadFactor = (double) num_elements/size; //update load factor
			}
			else {
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
			}
		}
	}
	else if(quadratic) { //if quadratic flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert 
		bool success = false; //successful insert flag
		
		while(success == false) { //loop while unsuccessful
			index = (h+(i*i))%size; //set index equal to quadratic probing formula
			if(hashArr[index].first == k) { //if key is already in the table
				hashArr[index].second++; //increment value
				break;
			}
			else if(hashArr[index].first == "#") { //if hashtable index is not filled
				hashArr[index] = std::make_pair(k,1); //add pairing of key and value to hashtable
				success = true; //set success flag to true
				num_elements++; //increment number of elements in the hash table
				loadFactor = (double) num_elements/size; //update load factor
			}
			else {
				i++; //increment the number of failed inserts
				if(i >= size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
			}
		}
	}
	else { //else double hash flag must be true
		int h = hash(k); //hashing key
		int h2 = hash2(k); //second hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert 
		bool success = false; //successful insert flag
		
		while(success == false) { //loop while unsuccessful
			index = (h+(i*h2))%size; //set index equal to double hashing formula
			if(hashArr[index].first == k) { //if key is already in the table
				hashArr[index].second++; //increment value
				break;
			}
			else if(hashArr[index].first == "#") { //if hashtable index is not filled
				hashArr[index] = std::make_pair(k,1); //add pairing of key and value to hashtable
				success = true; //set success flag to true
				num_elements++; //increment number of elements in the hash table
				loadFactor = (double) num_elements/size; //update load factor
			}
			else {
				i++; //increment the number of failed inserts
				if(i >= size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
			}
		}
	}
}

int Hashtable::count(const std::string& k) const {
	if(linear) { //if linear flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+i)%size; //set index equal to linear probing formula
			if(hashArr[index].first == k) { //if key is found
				return hashArr[index].second; //return its value
			}
			else if(hashArr[index].first == "#") { //if index is empty, the key must not be in the table
				return 0; //return 0
			}
			else { //otherwise check the next array
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
	else if(quadratic) { //if quadratic flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+(i*i))%size; //set index equal to quadratic probing formula
			if(hashArr[index].first == k) { //if key is found
				return hashArr[index].second; //return its value
			}
			else if(hashArr[index].first == "#") { //if index is empty, the key must not be in the table
				return 0; //return 0
			}
			else { //otherwise check the next index
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
	else { //else double hash flag must be true
		int h = hash(k); //hashing key
		int h2 = hash2(k); //second hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+(i*h2))%size; //set index equal to double hashing formula
			if(hashArr[index].first == k) { //if key is found
				return hashArr[index].second; //return its value
			}
			else if(hashArr[index].first == "#") { //if index is empty, the key must not be in the table
				return 0; //return 0
			}
			else { //otherwise check the next index
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
	return 0;
}

void Hashtable::reportAll(std::ostream& out) const {
	for(int i = 0; i < size; i++) { 
		if(hashArr[i].first != "#") { //print non-empty indices
			out << hashArr[i].first << " " << hashArr[i].second << "\n";
		}
	} 
}

void Hashtable::resize() {
	int old_size = size; //int to store old size
	for(int i = 0; i < 28; i++) { //loop until you find the next largest size for the hashtable
		if(hashArr_sizes[i] > old_size) { //if that value is found
			size = hashArr_sizes[i]; //update our size variable
			break;
		}
	}

	std::pair<std::string,int>* old_hashArr = hashArr; //creating a shallow copy of the old hash array
	hashArr = new std::pair<std::string,int> [size]; //creating resized hash table
	for(int x = 0; x < size; x++) { //loop to set initial values of new hash array
		hashArr[x] = std::make_pair("#", 0); //set initial key value for all pairs to a "garbage" string; values are set to 0
	}

	num_elements = 0; //reset number of elements to 0 before rehashing
	loadFactor = num_elements/size; //update the load factor
	for(int j = 0; j < old_size; j++) { //loop through old hash table
		if(old_hashArr[j].first != "#") {  //if spot is occupied by a key
			add(old_hashArr[j].first); //rehash the key in the new array
			int temp = findIndex(old_hashArr[j].first); //find the key's index in the new hashmap 
			hashArr[temp].second = old_hashArr[j].second; //copy value from old hash array to the new one
		}
	}
}

int Hashtable::hash(const std::string& k) const {
	std::stringstream ss; 
	//converting letters to int values
	for(int i = 0; i < (int) k.length(); i++) {
		int temp = k[i] - 97; //temp int to hold each letter to int conversion 
		ss << temp; //add temp to stringstream
		ss << " "; //add spacing between ints
	}
	int letters[30] = {}; //array of int representation of word which initial is filled with leading zeros
	for(int j = 30-((int)k.length()); j < 30; j++) {
		ss >> letters[j]; //outputing ss contents to each index of letters while keeping leading zeros in place
	}

	int w[5] = { }; //array of w1, w2, w3, w4, w5
	w[0] = powerHelper(letters[0], letters[1], letters[2], letters[3], letters[4], letters[5]); 
	w[1] = powerHelper(letters[6], letters[7], letters[8], letters[9], letters[10], letters[11]);
	w[2] = powerHelper(letters[12], letters[13], letters[14], letters[15], letters[16], letters[17]);
	w[3] = powerHelper(letters[18], letters[19], letters[20], letters[21], letters[22], letters[23]);
	w[4] = powerHelper(letters[24], letters[25], letters[26], letters[27], letters[28], letters[29]);

	//hashing the word
	long long temp = r1*w[0] + r2*w[1] + r3*w[2] + r4*w[3] + r5*w[4];
	temp = temp % size;

	return (int) temp;
}

int Hashtable::powerHelper(int a1, int a2, int a3, int a4, int a5, int a6) const {
	double temp = pow(26.0, 5.0)*a1 + pow(26.0, 4.0)*a2 + pow(26.0, 3.0)*a3 + pow(26.0, 2.0)*a4 + 26.0*a5 + a6; //using provided formula 
	return (int) temp; //cast temp to an int
}

int Hashtable::hash2(const std::string& k) const {
	std::stringstream ss; 
	//converting letters to int values
	for(int i = 0; i < (int) k.length(); i++) {
		int temp = k[i] - 97; //temp int to hold each letter to int conversion 
		ss << temp; //add temp to stringstream
		ss << " "; //add spacing between ints
	}
	int letters[30] = {}; //array of int representation of word which initial is filled with leading zeros
	for(int j = 30-((int)k.length()); j < 30; j++) {
		ss >> letters[j]; //outputing ss contents to each index of letters while keeping leading zeros in place
	}

	int w[5] = { }; //array of w1, w2, w3, w4, w5
	w[0] = powerHelper(letters[0], letters[1], letters[2], letters[3], letters[4], letters[5]); 
	w[1] = powerHelper(letters[6], letters[7], letters[8], letters[9], letters[10], letters[11]);
	w[2] = powerHelper(letters[12], letters[13], letters[14], letters[15], letters[16], letters[17]);
	w[3] = powerHelper(letters[18], letters[19], letters[20], letters[21], letters[22], letters[23]);
	w[4] = powerHelper(letters[24], letters[25], letters[26], letters[27], letters[28], letters[29]);

	//hashing the word
	long long temp = w[0] + w[1] + w[2] + w[3] + w[4];
	
	int p; //prime number to be used in second hash function
	for(int x = 0; x < 28; x++) {
		if(primes[x] < size) { //if the int at the current index in the primes array is smaller than the hashtable size 
			p = primes[x]; //set prime to said int
		}
		else { //otherwise break from for loop
			break;
		}
	}

	//finishing the hashing of the word
	temp = temp % p;
	temp = p - temp;

	return (int) temp;
}

int Hashtable::findIndex(const std::string& k) const {
	if(linear) { //if linear flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+i)%size; //set index equal to linear probing formula
			if(hashArr[index].first == k) { //if key is found
				return index; //return it's index
			}
			else { //otherwise check the next array
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
	else if(quadratic) { //if quadratic flag is true
		int h = hash(k); //hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+(i*i))%size; //set index equal to quadratic probing formula
			if(hashArr[index].first == k) { //if key is found
				return index; //return it's index
			}
			else { //otherwise check the next index
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
	else { //else double hash flag must be true
		int h = hash(k); //hashing key
		int h2 = hash2(k); //second hashing key
		int i = 0; //number of failed inserts
		int index; //index to insert to
		int indices_searched = 0; //number of indices searched

		while(indices_searched < size) { //loop until you've searched all the indices in the function
			index = (h+(i*h2))%size; //set index equal to double hashing formula
			if(hashArr[index].first == k) { //if key is found
				return index; //return it's index
			}
			else { //otherwise check the next index
				i++; //increment the number of failed inserts
				if(i == size) { //if you reached the end of the array
					i = 0; //go back to beginning 
				}
				indices_searched++; //increment the amount of indices searched
			}
		}
	}
}
