#include "lliststring.h"
#include <cstdlib>
#include <stdexcept> // See the comment directly below.

/**
 * This implementation uses exceptions in both versions of 'get'. 
 * You do not need to change the if-statements that use exceptions, 
 * nor do you need to test for exceptions. 
 *
 * In case you are interested, you can find more information here:
 * http://www.cplusplus.com/doc/tutorial/exceptions/
 * 
 * We will cover exceptions later this semester. 
 */

LListString::LListString()
{
  head_ = NULL;
  tail_ = NULL;
  size_ = 0;
}

LListString::~LListString()
{
  clear();
}

bool LListString::empty() const
{
  return (size_ == 0);
}

int LListString::size() const
{
  return size_;
}

void LListString::insert(int pos, const std::string& val)
{
  if(empty()) { //insertion into an empty list
    Item *newItem = new Item; 
    head_ = newItem; //update head
    tail_ = newItem; //update tail
    newItem->val = val; 
    //update new item pointers 
    newItem->prev = NULL; 
    newItem->next = NULL;
    size_++; //increment size
  }
  else if(pos > 0 && pos < size_-1) { //pos is between 0 and size-1
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    //update new item pointers 
    newItem->next = old;
    newItem->prev = old->prev;
    //update old item pointers 
    old->prev->next = newItem;
    old->prev = newItem;
    size_++; //increment size
  }
  else if(pos == 0) { //insertion at starting node
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    //update new item pointers 
    newItem->next = old;
    newItem->prev = NULL;
    //update old item pointers 
    old->prev = newItem;
    head_ = newItem; //update head to new item
    size_++; //increment size
  }
  else if(pos == size_-1){ //insertion at last node
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    //update new item pointers 
    newItem->next = old;
    newItem->prev = old->prev;
    //update old item pointers 
    old->prev->next = newItem;
    old->prev = newItem;
    old->next = NULL;
    tail_ = old; //make sure tail still points to old item
    size_++; //increment size
  }
  else if(pos == size_) { //insertion to add new node to the back of list
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos-1);
    //update new item pointers 
    newItem->next = NULL;
    newItem->prev = old;
    //update old item pointers 
    old->next = newItem;
    tail_ = newItem; //update tail to point to new item
    size_++; //increment size
  }
   
}

void LListString::remove(int pos)
{
  if(pos == 0 && size() == 1) { //removing from first node of list with size 1
    Item *toRemove = getNodeAt(pos);
    delete toRemove;
    head_ = NULL;
    tail_ = NULL;
    size_--; //decrement size
  }
  else if(pos == 0) { //removing from first node of list with more than one node
    Item *toRemove = getNodeAt(pos);
    head_ = toRemove->next;
    toRemove->next->prev = NULL;
    delete toRemove;
    size_--; //decrement size
  }
  else if(pos == size_ - 1) { //removing from last node of list
    Item *toRemove = getNodeAt(pos);
    toRemove->prev->next = NULL;
    tail_ = toRemove->prev;
    delete toRemove;
    size_--; //decrement size
  }
  else if(pos > 0 && pos < size_ - 1) { //removing from middle of the list
    Item *toRemove = getNodeAt(pos);
    toRemove->prev->next = toRemove->next;
    toRemove->next->prev = toRemove->prev;
    delete toRemove;
    size_--; //decrement size
  }
}

void LListString::set(int pos, const std::string& val)
{
  if(pos >= 0 && pos < size_) {
  	Item *temp = getNodeAt(pos);
    temp->val = val;	
  }
}

std::string& LListString::get(int pos)
{
  if(pos < 0 || pos >= size_) {
    throw std::invalid_argument("bad location");
  }
  Item *temp = getNodeAt(pos);
  return temp->val;
}

std::string const & LListString::get(int pos) const
{
  if(pos < 0 || pos >= size_) {
    throw std::invalid_argument("bad location");
  }
  Item *temp = getNodeAt(pos);
  return temp->val;
}

void LListString::clear()
{
  while(head_ != NULL) {
    Item *temp = head_->next;
    delete head_;
    head_ = temp;
  }
  tail_ = NULL;
  size_ = 0;
}

LListString::Item* LListString::getNodeAt(int pos) const
{
  if(pos < 0 || pos >= size_) {
    throw std::invalid_argument("bad location");
  }
  if(empty()) { //error if list is empty
    throw std::invalid_argument("list is empty");
  }
  Item *temp = head_; //temp pointer to move down list
  for(int i = 0; i < size_; i++) { //looping through size of list
    if(i == pos) { //checking if current index = inputed pos
      return temp; //return Item at this location
    }
    else {
      temp = temp->next; //else move to next node
    }
  }
  throw std::invalid_argument("could not locate node at given pos") //if "i" never equalled pos raise this error
}
