#include <iostream>

using namespace std;

struct Node {
    int value;
    Node *next;
};

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


int main() {
	Node a,b,c,d;
	a.value = 1;
	b.value = 2;
	c.value = 3;
	d.value = 4;
	a.next = &b;
	b.next = &c;
	c.next = &d;
	d.next = NULL;
	Node* odds = NULL;
	Node* evens = NULL;
	Node* in = &a;
	split(in, odds, evens);

	cout << a.next << " " << b.next << " " << c.next << " " << d.next << endl; 
	cout << odds->value << " " << odds->next->value << " " << odds->next->next << endl;
	cout << evens->value << " " << evens->next->value << " " << evens->next->next << endl;


	return 0;
}