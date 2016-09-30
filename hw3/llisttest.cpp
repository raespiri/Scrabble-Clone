#include "lliststring.h"
#include <cstdlib>
#include <stdexcept>
#include "gtest/gtest.h"


class LLtest : public testing::Test {
protected:
	LLtest() {

	}
	
	virtual ~LLtest () {

	}

	virtual void SetUp() { //SetUp for all Test cases
		list.insert(0, "a"); 
		list.insert(1, "b");
		list.insert(2, "c");
	}

	LListString list; //Object to be used for test case

};

//Test with valid input for Copy Constructor
TEST_F(LLtest, CopyNominal) {  
	LListString list2(list);
	//Expecting nodes from "list" to be copied into "copy"
	EXPECT_EQ("a", list2.get(0));
	EXPECT_EQ("b", list2.get(1));
	EXPECT_EQ("c", list2.get(2));
}

//Test Copy Constructor with empty list
TEST_F(LLtest, CopyBoundary1) {
	LListString empty;
	LListString list2(empty);
	EXPECT_EQ(0, list2.size()); //Expect new list to have a size of 0
}

//Test for Copy Constructor: copying over LL with only 1 node
TEST_F(LLtest, CopyBoundary2) {
	LListString one;
	one.insert(0, "a"); //making LL with only 1 node
	LListString list2(one);
	EXPECT_EQ("a", list2.get(0));
}

//Test with valid input for Assingment Operator
TEST_F(LLtest, AssignmentNominal) {
	LListString list2 = list;
	//Expecting nodes from "list" to be copied into "copy"
	EXPECT_EQ("a", list2.get(0));
	EXPECT_EQ("b", list2.get(1));
	EXPECT_EQ("c", list2.get(2));
}

//Test with assignment of empty LL
TEST_F(LLtest, AssignmentBoundary1) {
	LListString empty;
	list = empty;
	EXPECT_EQ(0, list.size()); //Expect list to have a size of 0
}

//Test with assignment of LL with only one node 
TEST_F(LLtest, AssignmentBoundary2) {
	LListString one;
	one.insert(0, "a"); //making LL with only 1 node
	LListString list2 = one;
	EXPECT_EQ("a", list2.get(0));
}

//Test for Concatenation operator 
TEST_F(LLtest, ConcatenationNominal) {
	LListString list2;
	list2.insert(0, "d");
	list2 = list2 + list;
	//Expect previous node from list2
	EXPECT_EQ("d", list2.get(0));
	//Expecting nodes from "list" to be added on
	EXPECT_EQ("a", list2.get(1));
	EXPECT_EQ("b", list2.get(2));
	EXPECT_EQ("c", list2.get(3));
	//Expecting size to be updated
	EXPECT_EQ(4, list2.size());
} 

//Test for Concatenation operator on a LL with size 0
TEST_F(LLtest, ConcatenationOffNominal) {
	LListString empty;
	list = list + empty;
	EXPECT_EQ(3, list.size()); //Expect LL size to be unchanged
}

//Test for invalid input with Access Operator
TEST_F(LLtest, AccessOffNominal) {
	EXPECT_ANY_THROW(list[-1]); //Out of bounds access
}

//Test for valid input with Access Operator
TEST_F(LLtest, AccessNominal) {
	EXPECT_EQ("a", list[0]);
}
