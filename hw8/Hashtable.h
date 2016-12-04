#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <string>
#include <iostream>

/**
* A Hashtable to store <std::string, int> as the entries. The string represents
* the element in the Hashtable, and the int represents the number of instances of
* that string in the Hashtable. 
*/
class Hashtable
{
public:
	/**
	* Creates a new Hashtable instance. Two optional parameters are provided, and by
	* default are turned off. If the debug parameter is set to true, then the Hashtable
	* should use the pre-prescribed random numbers in the assignment description. The 
	* probing parameter will always be 0, 1, or 2, indicating the kind of collision
	* avoidance strategy. 
	* 0: Linear probing
	* 1: Quadratic probing
	* 2: Double hashing
	*/
	Hashtable(bool debug = false, unsigned int probing = 0);

	/**
	* Destructor.
	*/
	~Hashtable();

	/**
	* If the parameter k already exists as an element in the Hashtable, increment the 
	* total number of entries. Otherwise create a new entry with a count of 1.
	*/
	void add(const std::string& k);

	/**
	* Returns the total number of entries for the given string, or 0 if it does not
	* exist in the Hashtable.
	*/
	int count(const std::string& k) const;

	/**
	* Outputs to the provided stream every key-value pair of elements in the Hashtable
	* in the order they appear. The output should follow the format that each
	* key-value pair is printed on a separate line, and the key is separated from the 
	* value by a space .
	*/
	void reportAll(std::ostream& out) const;

private:
	/**
	* A helper function to resize and rehash the entries in the table when the load 
	* factor is >= 0.5. See the assignment description for the specific increments
	* when increasing the size of the Hashtable as well as other implementation details. 
	*/
	void resize();

	/**
	* The actual hash function that transforms the string input into the index where
	* the element should be stored. See the assignment for more details on how to 
	* implement this function according to our specific guidelines. 
	*/
	int hash(const std::string& k) const;

private:
	/**
	* Include any additional private data members and/or helper functions to finish
	* this problem. 
    */
    std::pair<std::string,int>* hashArr; //array of elements and number of instances for each
    long long r1; //random int for hash function
    long long r2; //random int for hash function
    long long r3; //random int for hash function
    long long r4; //random int for hash function
    long long r5; //random int for hash function
   	bool linear; //flag for linear probing
   	bool quadratic; //flag for quadratic probing
   	bool doublehash; //flag for double hashing
   	int size; //number of indices in hashArr
	
	//initializing the array for the different sizes of the hash table
	int hashArr_sizes[28] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 
					 	     12853, 25717, 51437, 102877, 205759, 411527, 823117, 
					         1646237, 3292489, 6584983, 13169977, 26339969, 52679969, 
					         105359969, 210719881, 421439783, 842879579, 1685759167};   	

	//initializing the primes array for the second hash function
	int primes[28] = {7, 19, 43, 89, 193, 389, 787, 1583, 3191, 6397, 12841, 25703, 51431, 
					  102871, 205721, 411503, 823051, 1646221, 3292463, 6584957, 13169963, 
					  26339921, 52679927, 105359939, 210719881, 421439749, 842879563, 1685759113};

   	double loadFactor; //load factor of hash table
   	int num_elements; //number of elements in the table
   	bool Debug;
   	unsigned int Probing; 

   	int powerHelper(int a1, int a2, int a3, int a4, int a5, int a6) const; //function to compute the power formula given in hw
   	int hash2(const std::string& k) const; //second hash function for double hashing
   	int findIndex(const std::string& k) const; //returns index of given key value

};

#endif