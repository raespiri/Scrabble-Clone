#include <iostream>

using namespace std;

void split(Node*& in, Node*& odds, Node*& evens) {
	if(in == NULL) { //base case 
		in = NULL; //in set to NULL
		odds = NULL; //tail of odds set to NULL
		evens = NULL; //tail of evens set to NULL
	}
	else {
		if(in->value % 2 == 0) { //if even integer
			evens = in;
			Node* temp = in->next; //temp pointer to hold address of the next node in in
			in->next = NULL; //setting in->next to NULL so orginal list is not preserved 
			split(temp, odds, evens->next); //recurse with evens and in moved to next nodes
		}
		else {
			odds = in;
			Node* temp = in->next; //temp pointer to hold address of the next node in in
			in->next = NULL; //setting in->next to NULL so orginal list is not preserved
			split(temp, odds->next, evens); // recurse with odds and in moved to next nodes
		}
	}
}
