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
    head_ = newItem; 
    tail_ = newItem;
    newItem->val = val;
    newItem->prev = NULL;
    newItem->next = NULL;
    size_++; //increment size
  }
  else if(pos > 0 && pos < size_-1) { //pos is between 0 and size-1
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    newItem->next = old;
    newItem->prev = old->prev;
    old->prev->next = newItem;
    old->prev = newItem;
    size_++; //increment size
  }
  else if(pos == 0) { //insertion at starting node
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    newItem->next = old;
    newItem->prev = NULL;
    old->prev = newItem;
    head_ = newItem;
    size_++; //increment size
  }
  else if(pos == size_-1){ //insertion at last node
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos);
    newItem->next = old;
    newItem->prev = old->prev;
    old->prev->next = newItem;
    old->prev = newItem;
    old->next = NULL;
    tail_ = old;
    size_++; //increment size
  }
  else if(pos == size_) { //insertion to add new node to the back of list
    Item *newItem = new Item;
    newItem->val = val;
    Item *old = getNodeAt(pos-1);
    newItem->next = NULL;
    newItem->prev = old;
    old->next = newItem;
    tail_ = newItem;
    size_++; //increment size
  }
   
}

void LListString::remove(int pos)
{
  if(pos == 0 && size() == 1) {
    Item *toRemove = getNodeAt(pos);
    delete toRemove;
    head_ = NULL;
    tail_ = NULL;
    size_--; //decrement size
  }
  else if(pos == 0) {
    Item *toRemove = getNodeAt(pos);
    head_ = toRemove->next;
    toRemove->next->prev = NULL;
    delete toRemove;
    size_--; //decrement size
  }
  else if(pos == size_ - 1) {
    Item *toRemove = getNodeAt(pos);
    toRemove->prev->next = NULL;
    tail_ = toRemove->prev;
    delete toRemove;
    size_--; //decrement size
  }
  else if(pos > 0 && pos < size_ - 1) {
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
}
