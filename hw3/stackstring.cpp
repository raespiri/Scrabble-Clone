#include "stackstring.h"
#include <cstdlib>
#include <stdexcept> 

StackString::StackString() {

}

StackString::~StackString()	{

}

bool StackString::empty() const {
	return list.empty(); //call list empty function
}

void StackString::push (const std::string& val) {
	list.insert(0, val); //insert to front of list
}

std::string const & StackString::top () const {
	return list.get(0); //gets string at node 0 of the list
}


void StackString::pop () {
	if(!list.empty()) { //if list isn't empty
		list.remove(0); //removing item at top of stack
	}
}