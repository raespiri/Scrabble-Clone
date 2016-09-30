#ifndef STACKSTRING_H
#define STACKSTRING_H
#include "lliststring.h"
#include <cstdlib>
#include <stdexcept> 

class StackString {
 public:
  StackString();
  ~StackString();

  /**
   * Returns true if the stack is empty, false otherwise
   */
  bool empty() const;

  /**
   * Pushes a new value, val, onto the top of the stack
   */
  void push (const std::string& val);

  /**
   * Returns the top value on the stack.
   * If the stack is empty, the correct behavior is to throw
   * an exception: use LListString::get to achieve this.
   */
  std::string const & top () const;

  /**
   * Removes the top element on the stack.
   * Should do nothing if the stack is empty.
   */
  void pop ();

 private:
  /** 
   * Single data member of the Stack.
   * We use composition to implement this Stack 
   */
  LListString list;
};
#endif
