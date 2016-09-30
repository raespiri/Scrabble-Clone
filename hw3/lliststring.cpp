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
  //if "i" never equalled pos raise this error
  throw std::invalid_argument("could not locate node at given pos"); 
}

LListString::LListString(const LListString& other) {
  if(other.size() < 0) { //error with the passed LL's size
    throw std::invalid_argument("parameter LL is of invalid size");
  }
  else if(other.size() == 0) { //Parameter LL is empty
    head_ = NULL; //base values for variables
    tail_ = NULL;
    size_ = 0;
  }
  else if(other.size() == 1) { //if the passed LL has one node
    Item *newItem = new Item;
    newItem->val = other.getNodeAt(0)->val; //grab node's val in inputed LL
    newItem->prev = NULL; //setting prev and next to point to nothing
    newItem->next = NULL;
    head_ = newItem; //set head to new item
    tail_ = newItem; //set tail to new item
    size_ = other.size(); //setting size to other's size
  }
  else { //any other size of the LL to be copied
    size_ = other.size(); //updating size of current LL
    for(int i = 0; i < other.size(); i++) {
      if(i == 0) { //if copying the head of the list
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying given node's val from other LL to current LL
        newItem->prev = NULL; //prev should equal NULL
        head_ = newItem; //head points to head of LL
      }
      else if(i == other.size()-1) { //if copying the tail of the list
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying node's val into new LL
        newItem->prev = getNodeAt(size_-2); //updating prev of last item to point to 2nd to last item
        newItem->prev->next = newItem; //setting previous item to point to last item
        newItem->next = NULL;
        tail_ = newItem; //tail points to new item
      }
      else if(i > 0 && i < other.size()-1) {
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying node into new LL
        newItem->prev = getNodeAt(i-1); //setting prev to previous node of current LL
        newItem->prev->next = newItem; //setting previous node's next point to current newItem
      } 
    }
  }
}

LListString& LListString::operator= (const LListString& other) {
  if(other.size() < 0) { //error, LL's size is invalid
    throw std::invalid_argument("parameter LL is of invalid size");
  }
  this->clear(); //clearing current LL if already filled
  if(other.size() == 0) { //Parameter LL is empty
    this->head_ = NULL; //base values for variables
    this->tail_ = NULL;
    this->size_ = 0;
  }  
  else if(other.size() == 1) { //if the passed LL has one node
    Item *newItem = new Item;
    newItem->val = other.getNodeAt(0)->val; //grab node's val in inputed LL
    newItem->prev = NULL; //setting prev and next to point to nothing
    newItem->next = NULL;
    this->head_ = newItem; //set head to new item
    this->tail_ = newItem; //set tail to new item
    this->size_ = other.size(); //setting size to other's size
  }
  else { //any other size of LL to be copied
    this->size_ = other.size(); //updating size of current LL
    for(int i = 0; i < other.size(); i++) {
      if(i == 0) { //if copying the head of the list
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying given node's val from other LL to current LL
        newItem->prev = NULL; //prev should equal NULL
        this->head_ = newItem; //head points to head of LL
      }
      else if(i == other.size()-1) { //if copying the tail of the list
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying node's val into new LL
        newItem->prev = this->getNodeAt(this->size_-2); //updating prev of last item to point to 2nd to last item
        newItem->prev->next = newItem; //setting previous item to point to last item
        newItem->next = NULL;
        this->tail_ = newItem; //tail points to new item
      }
      else if(i > 0 && i < other.size()-1) {
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying node's val into new LL
        newItem->prev = this->getNodeAt(i-1); //setting prev to previous node of current LL
        newItem->prev->next = newItem; //setting previous node's next point to current newItem
      } 
    }
  }
  return *this;
}

LListString LListString::operator+ (const LListString& other) const {
  if(other.size() < 0) { //error, LL's size is invalid
    throw std::invalid_argument("parameter LL is of invalid size");
  }
  if(other.size() == 0) { //adding a list of size 0
    LListString listcopy(*this);
    return listcopy; //return unmodified copy of original list
  }
  if(other.size() == 1) { //adding a list of size 1
    LListString listcopy(*this);
    Item *newItem = new Item;
    newItem->val = other.getNodeAt(0)->val; //copying val from other into new Item
    listcopy.getNodeAt(listcopy.size_-1)->next = newItem; //setting end of list copy to point to new item
    newItem->prev = listcopy.getNodeAt(listcopy.size_-1); //new item previous points to last item of list copy
    newItem->next = NULL; //new item next points to NULL
    listcopy.tail_ = newItem;
    if(listcopy.size_ == 0) { //if list prior to concatenation is of size 0 update head
      listcopy.head_ = newItem;
      newItem->prev = NULL; //set previous item to NULL
    }
    listcopy.size_++; //updating size by adding 1
    return listcopy;
  }
  else { //adding a list of any other size
    LListString listcopy(*this);
    for(int i = 0; i < other.size(); i++) {
      if(i == 0) { //first node in other LL
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying val from other into new Item
        if(listcopy.size_ == 0) { //if list prior to concatenation is of size 0 update head
          listcopy.head_ = newItem;
          newItem->prev = NULL; //set previous item to NULL
        }
        else { //else update end of listcopy and new item to point to each other
          listcopy.getNodeAt(listcopy.size_-1)->next = newItem; //setting end of list copy to point to new item 
          newItem->prev = listcopy.getNodeAt(listcopy.size_-1); //new item previous points to last item of list copy
        }
        listcopy.size_++; //increment size
      }
      else if(i == other.size()-1) { //if adding the tail node of other
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying val from other into new Item
        newItem->next = NULL; //setting last node's next pointer to NULL
        newItem->prev = listcopy.getNodeAt(listcopy.size_-1); //prev points to 2nd to last item
        newItem->prev->next = newItem; //2nd to last item points to new Item
        listcopy.tail_ = newItem;
        listcopy.size_++; //increment size
      }
      else {
        Item *newItem = new Item;
        newItem->val = other.getNodeAt(i)->val; //copying val from other into new Item
        newItem->prev = listcopy.getNodeAt(listcopy.size_-1); //setting previous item to be previous node
        newItem->prev->next = newItem; //previous item's next pointer points to new Item
        listcopy.size_++; //increment size
      }
    }
    return listcopy;
  }  
}

std::string const & LListString::operator[](int pos) const {
  if(pos < 0 || pos >= size_) { //pos is out of bounds
    throw std::invalid_argument("pos is out of bounds");
  }
  for(int i = 0; i < this->size_; i++) {
    if(i == pos) {
      return this->get(i);
    }
  }
  throw std::invalid_argument("No value found for given position");
}