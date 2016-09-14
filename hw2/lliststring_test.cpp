#include "lliststring.h"
#include <iostream>
 
using namespace std;

int main() {
  LListString* list = new LListString();

  /*	TEST 1: INSERTING INTO EMPTY list    */
  
  // Check if the list is initially empty.
  if (list->empty()) {
    cout << "SUCCESS: List is empty initially." << endl;
  } else {
    cout << "FAIL: List is not empty initially when it should be." << endl;
  }

  // Insert an item at the head.
  list->insert(0, "Gandalf");

  // Check if the list is still empty.
  if (!list->empty()) {
    cout << "SUCCESS: List is not empty after one insertion." << endl;
  } else {
    cout << "FAIL: List is empty after one insertion." << endl;
  }

  if (list->size() == 1) {
    cout << "SUCCESS: List has size 1 after one insertion." << endl;
  } else {
    cout << "FAIL: List has size " << list->size() << " after one insertion.";
    cout << endl;
  }

  // Check if the value is correct.
  if (list->get(0) == "Gandalf") {
    cout << "SUCCESS: 'Gandalf' is at the 0th index of the list." << endl;
  } else {
    cout << "FAIL: 'Gandalf' is not at the 0th index of the list, " << list->get(0);
    cout << " is instead." << endl;
  }

  /*	TEST 2: INSERTING TO END OF LIST    */
  // Inserting item to end of list (pos = size)
  list->insert(1, "Mario");

  // Check size of list
  if(list->size() == 2) {
  	cout << "SUCCESS: List has size 2 after an additional insertion." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after an additional insertion." << endl;
  }

  // Check that new item is at correct node
  if(list->get(1) == "Mario") {
  	cout << "SUCCESS: 'Mario' is at the 1st index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Mario' is not at the 1st index of the list, " << list->get(1) << " is instead." << endl;
  }

  /*	TEST 3: INSERTING AT LAST NODE OF THE LIST    */
  // Inserting item to last node of the list
  list->insert(1, "Luigi");

  // Check size of list
  if(list->size() == 3) {
  	cout << "SUCCESS: List has size 3 after an additional insertion." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after an additional insertion." << endl;
  }

  // Check that new item is at correct node
  if(list->get(1) == "Luigi") {
  	cout << "SUCCESS: 'Luigi' is at the 1st index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Luigi' is not at the 1st index of the list, '" << list->get(1) << "'' is instead." << endl;
  }

  // Check that item at previous node is moved to subsequent node
  if(list->get(2) == "Mario") {
  	cout << "SUCCESS: 'Mario' is at the 2nd index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Mario' is not at the 2nd index of the list, " << list->get(2) << " is instead." << endl;
  }

  /*	TEST 4: INSERTING TO MIDDLE OF LIST    */
  // Inserting item to middle of the list
  list->insert(1, "Yoshi");

  // Check size of list
  if(list->size() == 4) {
  	cout << "SUCCESS: List has size 4 after an additional insertion." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after an additional insertion." << endl;
  }

  // Check that new item is at correct node
  if(list->get(1) == "Yoshi") {
  	cout << "SUCCESS: 'Yoshi' is at the 1st index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Yoshi' is not at the 1st index of the list, '" << list->get(1) << "'' is instead." << endl;
  }

  // Check that item previously at the middle node is moved to the next node
  if(list->get(2) == "Luigi") {
  	cout << "SUCCESS: 'Luigi' is at the 2nd index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Luigi' is not at the 2nd index of the list, " << list->get(2) << " is instead." << endl;
  }

  // Check that previous node is unchanged
  if(list->get(0) == "Gandalf") {
  	cout << "SUCCESS: 'Gandalf' is at the 0th index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Gandalf' is not at the 0th index of the list, " << list->get(0) << " is instead." << endl;
  }

  /*	TEST 5: INSERTING TO 0TH NODE OF FILLED LIST    */
  // Inserting item to 0th node
  list->insert(0, "Link");

   // Check size of list
  if(list->size() == 5) {
  	cout << "SUCCESS: List has size 5 after an additional insertion." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after an additional insertion." << endl;
  }

  // Check that new item is at correct node
  if(list->get(0) == "Link") {
  	cout << "SUCCESS: 'Link' is at the 0th index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Link' is not at the 0th index of the list, '" << list->get(0) << "'' is instead." << endl;
  }

  // Check that item previously at the 0th node is moved to the next node
  if(list->get(1) == "Gandalf") {
  	cout << "SUCCESS: 'Gandalf' is at the 1st index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Gandalf' is not at the 1st index of the list, " << list->get(1) << " is instead." << endl;
  }

  /*	TEST 6: INSERTING OUT OF BOUNDS    */
  // Inserting out of bounds item
  list->insert(-1, "Toad");

  // Check that size of list doesn't change
  if(list->size() == 5) {
  	cout << "SUCCESS: List size was not changed after invalid insertion." << endl;
  }
  else {
  	cout << "FAIL: List size was changed to " << list->size() << " after invalid insertion." << endl;
  }

  /*	TEST 7: REMOVING FROM 0TH NODE    */
  // Removing from 0th node
  list->remove(0);

  if(list->size() == 4) {
  	cout << "SUCCESS: List has size 4 after a removal." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after a removal." << endl;
  }

  // Check that removed item is not in list
  bool fail1 = false;
  for(int i = 0; i < list->size(); i++) {
  	list->get(i);
  	if(list->get(i) == "Link"){
  		cout << "FAIL: End item was not successfully removed, it exists at node " << i << "." << endl;
  		fail1 = true;
  		break;
  	}
  }
  if(fail1 == false) {
  	cout << "SUCCESS: End item was successfully removed." << endl;
  }

  // Check that item previously at 1st node is now at 0th node
  if(list->get(0) == "Gandalf") {
  	cout << "SUCCESS: 'Gandalf' is now at the 0th index of the list." << endl;
  }
  else {
  	cout << "FAIL: 'Gandalf' is not at the 0th index of the list, '" << list->get(0) << "'' is instead." << endl;
  }

  /*	TEST 8: REMOVING FROM END NODE    */
  // Removing from 0th node
  list->remove(3);

  if(list->size() == 3) {
  	cout << "SUCCESS: List has size 3 after a removal." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after a removal." << endl;
  }

  // Check that removed item is not in list
  bool fail2 = false;
  for(int i = 0; i < list->size(); i++) {
  	list->get(i);
  	if(list->get(i) == "Mario"){
  		cout << "FAIL: End item was not successfully removed, it exists at node " << i << "." << endl;
  		fail2 = true;
  		break;
    }
  }
  if(fail2 == false) {
  	cout << "SUCCESS: End item was successfully removed." << endl;
  }

  /*	TEST 9: REMOVING FROM MIDDLE    */
  list->remove(1);

  if(list->size() == 2) {
	cout << "SUCCESS: List has size 2 after a removal." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after a removal." << endl;
  }

  // Check that removed item is not in list
  bool fail3 = false;
  for(int i = 0; i < list->size(); i++) {
  	list->get(i);
  	if(list->get(i) == "Yoshi"){
  		cout << "FAIL: End item was not successfully removed, it exists at node " << i << "." << endl;
  		fail3 = true;
  		break;
    }
  }
  if(fail3 == false) {
  	cout << "SUCCESS: End item was successfully removed." << endl;
  }

  /*	TEST 10: REMOVING FROM OUT OF BOUNDS POSITION	*/
  list->remove(-1);

  if(list->size() == 2) {
	cout << "SUCCESS: List size remains the same after an invalid removal." << endl;
  }
  else {
  	cout << "FAIL: List size changed to " << list->size() << " after an invalid removal." << endl;
  }

  /*	TEST 11: REMOVING LAST ITEM FROM LIST    */
  list->remove(1);
  list->remove(0);

  if(list->size() == 0) {
  	cout << "SUCCESS: List has size 0 after a removal." << endl;
  }
  else {
  	cout << "FAIL: List has size " << list->size() << " after a removal." << endl;
  }


  delete list; // clean up memory
}