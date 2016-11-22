#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they 
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent)
    , mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
    return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    void remove(const Key& key);

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
    void updateHeights(AVLNode<Key, Value>* node);
    void checkBalance(AVLNode<Key, Value>* node);
    void singleRotation(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x);
    void doubleRotation(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x);
    int findHeight(AVLNode<Key, Value>* node);
    void nodeFinder(AVLNode<Key, Value>* node, const Key& key, AVLNode<Key, Value>*& found_node);
    AVLNode<Key, Value>* findSuccessor(AVLNode<Key, Value>* node, typename BinarySearchTree<Key, Value>::iterator& it);
    void promoteSuccessor(AVLNode<Key, Value>* deleted_node, AVLNode<Key, Value>* successor_node);

};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // TODO
    BinarySearchTree<Key, Value>::insert(keyValuePair); //call inherited BST insert
    updateHeights(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //updating heights of AVL tree after insertion
    checkBalance(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //check the balance of the tree after updated heights
}

/**
* Finds any unblanced nodes in the AVL tree and calls proper rotation function.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::checkBalance(AVLNode<Key, Value>* node) {
    if(node->getLeft() != NULL) {
        checkBalance(node->getLeft());
    }
    if(node->getRight() != NULL) {
        checkBalance(node->getRight());
    }
    if(findHeight(node->getLeft()) - findHeight(node->getRight()) > 1) { //if heights of children differ by more than 1 and left child is largest
        if(findHeight(node->getLeft()->getLeft()) > findHeight(node->getLeft()->getRight())) { //if left grandchild has larger height
            singleRotation(node, node->getLeft(), node->getLeft()->getLeft());
        }
        else { //else right grandchild has larger height
            doubleRotation(node, node->getLeft(), node->getLeft()->getRight());
        }
    }
    else if(findHeight(node->getRight()) - findHeight(node->getLeft()) > 1) { //if heights of children differ by more than 1 and right child is largest
        if(findHeight(node->getRight()->getLeft()) > findHeight(node->getRight()->getRight())) { //if left grandchild has larger height
            doubleRotation(node, node->getRight(), node->getRight()->getLeft());
        }
        else { //else right grandchild has larger height
            singleRotation(node, node->getRight(), node->getRight()->getRight());
        }
    }
}

/**
* Function to conduct a single rotation on the AVL tree.
*/
template<typename Key, typename Value>
int AVLTree<Key, Value>::findHeight(AVLNode<Key, Value>* node) {
    if(node == NULL) { //if node is NULL return -1
        return -1;
    }
    else { //otherwise just return original height
        return node->getHeight(); 
    }
}


/**
* Function to conduct a single rotation on the AVL tree.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::singleRotation(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x) {
    if(z->getKey() < y->getKey()) { //if z is smaller than y
        z->setRight(y->getLeft()); //set z's right child to y's left child
        if(y->getLeft() != NULL) {
            y->getLeft()->setParent(z); //set y's left child's parent to z
        }
        y->setLeft(z); //set y's left child to z
        y->setParent(z->getParent()); //set y's parent to z's parent
        if(z->getParent() == NULL) {
            BinarySearchTree<Key, Value>::mRoot = y; //update mRoot if z was previously root
        }
        else {
            if(z->getParent()->getKey() > y->getKey()) {
                z->getParent()->setLeft(y); //if z's parent is larger than y set its left child to y
            }
            else {
                z->getParent()->setRight(y); //else z's parent is smaller than y set its right child to y
            }
        }
        z->setParent(y); //set z's parent to y
    }
    else { //else z is larger than y
        z->setLeft(y->getRight()); //set z's left child to y's right child
        if(y->getRight() != NULL) {
            y->getRight()->setParent(z); //set y's right child's parent to z
        }
        y->setRight(z); //set y's right child to z
        y->setParent(z->getParent()); //set y's parent to z's parent
        if(z->getParent() == NULL) {
            BinarySearchTree<Key, Value>::mRoot = y; //update mRoot if z was previously root
        }
        else {
            if(z->getParent()->getKey() > y->getKey()) {
                z->getParent()->setLeft(y); //if z's parent is larger than y set its left child to y
            }
            else {
                z->getParent()->setRight(y); //else z's parent is smaller than y set its right child to y
            }
        }
        z->setParent(y); //set z's parent to y
    }
    updateHeights(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //updating heights of AVL tree after insertion
}

/**
* Function to conduct a double rotation on the AVL tree.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::doubleRotation(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x) {
    if(z->getKey() < y->getKey()) { //if z is smaller than y
        z->setRight(x->getLeft()); //set z's right child to x's left child
        if(x->getLeft() != NULL) {
            x->getLeft()->setParent(z); //set z as parent of x's left child
        }
        x->setLeft(z); //set x's left child to z
        x->setParent(z->getParent()); //set x's parent to z's parent
        if(z->getParent() == NULL) {
            BinarySearchTree<Key, Value>::mRoot = x; //update mRoot if z was previously root
        }
        else {
            if(z->getParent()->getKey() > x->getKey()) {
                z->getParent()->setLeft(x); //if z's parent is larger than x set its left child to x
            }
            else {
                z->getParent()->setRight(x); //else z's parent is smaller than x set its right child to x
            }
        }
        z->setParent(x); //set z's parent to x
        y->setLeft(x->getRight()); //set y's left child to x's right child
        if(x->getRight() != NULL) {
            x->getRight()->setParent(y); //set y as parent of x's right child
        }
        x->setRight(y); //set x's right child to y
        y->setParent(x); //set y's parent to x
    }
    else { //else z is larger than y
        z->setLeft(x->getRight()); //set z's left child to x's right child
        if(x->getRight() != NULL) {
            x->getRight()->setParent(z); //set z as parent of x's right child
        }
        x->setRight(z); //set x's right child to z
        x->setParent(z->getParent()); //set x's parent to z's parent
        if(z->getParent() == NULL) {
            BinarySearchTree<Key, Value>::mRoot = x; //update mRoot if z was previously root
        }
        else {
            if(z->getParent()->getKey() > x->getKey()) {
                z->getParent()->setLeft(x); //if z's parent is larger than x set its left child to x
            }
            else {
                z->getParent()->setRight(x); //else z's parent is smaller than x set its right child to x
            }
        }
        z->setParent(x); //set z's parent to x
        y->setRight(x->getLeft()); //set y's right child to x's left child
        if(x->getLeft() != NULL) {
            x->getLeft()->setParent(y); //set y as parent of x's left child
        }
        x->setLeft(y); //set x's left child to y
        y->setParent(x); //set y's parent to x
    }
    updateHeights(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //updating heights of AVL tree after insertion
}

/**
* Recursive function to update the heights of the nodes in the AVL tree after each insertion.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::updateHeights(AVLNode<Key, Value>* node) {
    int left = -1; //starting height of left child
    int right = -1; //starting height of right child

    if(node->getLeft() != NULL) { //if node has a left child
        updateHeights(node->getLeft()); //recurse to left child
        left = node->getLeft()->getHeight(); //update "left" variable
    }
    if(node->getRight() != NULL) { //if node has a right child
        updateHeights(node->getRight()); //recurse to right child
        right = node->getRight()->getHeight(); //update "right" variable
    }

    if(left > right) { //if height of left is greater than height of right
        node->setHeight(left+1); //height of parent is 1 more than left
        return;
    }
    else { //else right and left are of equal heights or right is larger
        node->setHeight(right+1); //height of parent is 1 more than right 
        return;
    }
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    typename BinarySearchTree<Key, Value>::iterator it = BinarySearchTree<Key, Value>::find(key); //point iterator to node to be removed
    if(it == BinarySearchTree<Key, Value>::end()) { //if unable to find the key, return
        return;
    }

    AVLNode<Key, Value>* deleted_node;
    nodeFinder(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot), key, deleted_node); //finding deleted node
    
    AVLNode<Key, Value>* successor_node = findSuccessor(deleted_node, it); //finding successor node

    if(successor_node != NULL) { //if a successor exists
        promoteSuccessor(deleted_node, successor_node); //promote the successor
    }
   
    if(BinarySearchTree<Key, Value>::mRoot != NULL) { //if the root isn't NULL
        updateHeights(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //updating heights of AVL tree after remove
        checkBalance(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)); //check the balance of the tree after updated heights
    }
}

/**
* Function to find a node, given a key value 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::nodeFinder(AVLNode<Key, Value>* node, const Key& key, AVLNode<Key, Value>*& found_node) { 
    if(node->getKey() == key) { //if node to be removed was found
        found_node = node;
        return; //return node
    }
    else {
        if(node->getLeft() != NULL) { //if node has a left child
            nodeFinder(node->getLeft(), key, found_node); //recurse to left child   
        }
        if(node->getRight() != NULL) { //if node has a right child
            nodeFinder(node->getRight(), key, found_node); //recurse to right child
        }
    }
}

/**
* Function to find a successor
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findSuccessor(AVLNode<Key, Value>* node, typename BinarySearchTree<Key, Value>::iterator& it) {
    if(node->getLeft() == NULL && node->getRight() == NULL && node->getParent() == NULL) { //if deleted node is only node in the tree
        node = NULL; //set it equal to NULL
        delete node;
        BinarySearchTree<Key, Value>::mRoot = NULL; //set mRoot to NULL
        return NULL;
    }

    if(node->getLeft() == NULL && node->getRight() == NULL) { //if deleted node is a leaf node, return
        if(node->getKey() > node->getParent()->getKey()) {
            node->getParent()->setRight(NULL); //if deleted node is larger than its parent set parents right child to NULL
        }
        else {
            node->getParent()->setLeft(NULL); //else deleted node is smaller than its parent 
        }
        node = NULL; //set node to NULL
        delete node;
        return NULL;
    }

    if(node->getLeft() == NULL && node->getRight() != NULL) { //if deleted node has one child that is internal
        return node->getRight(); //the internal child is the successor
    }

    if(node->getRight() == NULL && node->getLeft() != NULL) { //if deleted node has one child that is internal
        return node->getLeft(); //the internal child is the successor
    }
   ++it; //increment iterator to get successor
   Key successor_key = (*it).first; //get the key of successor from iterator
   AVLNode<Key, Value>* temp;
   nodeFinder(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot), successor_key, temp);
   return temp; //return node of successor
}

/**
* Function to promote a successor
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::promoteSuccessor(AVLNode<Key, Value>* deleted_node, AVLNode<Key, Value>* successor_node) {
    AVLNode<Key, Value>* dParent = deleted_node->getParent(); //getting parent of node to be deleted
    AVLNode<Key, Value>* dLeft_child = deleted_node->getLeft(); //getting left child of node to be deleted
    AVLNode<Key, Value>* dRight_child = deleted_node->getRight(); //getting right child of node to be deleted

    AVLNode<Key, Value>* sParent = successor_node->getParent(); //getting parent of node to be promoted
    AVLNode<Key, Value>* sLeft_child = successor_node->getLeft(); //getting left child of node to be promoted
    AVLNode<Key, Value>* sRight_child = successor_node->getRight(); //getting right child of node to be promoted

    if(dParent == NULL) {
         BinarySearchTree<Key, Value>::mRoot = successor_node; //update mRoot if deleted node was previously root
    }
    else {
        if(dParent->getKey() > successor_node->getKey()) {
                dParent->setLeft(successor_node); //if deleted node's parent is larger than successor set its left child to successor
        }
        else {
            dParent->setRight(successor_node); //else deleted node's parent is smaller than successor set its right child to successor
        }
    }
    successor_node->setParent(dParent); //updating successor's parent

    if(dLeft_child == NULL || dRight_child == NULL) { //if deleted node had only one child
        successor_node->setLeft(NULL);
        successor_node->setRight(NULL);
    }
    else { //else both of the deleted node's children were not NULL
        //attaching successor's previous left child to it's former parent
        if(sLeft_child != NULL) { 
            if(sParent->getKey() > sLeft_child->getKey()) {
                sParent->setLeft(sLeft_child);
                sLeft_child->setParent(sParent); 
            }
            else {
                sParent->setRight(sLeft_child); 
                sLeft_child->setParent(sParent);
            }
        }
        //attaching successor's previous right child to it's former parent
        if(sRight_child != NULL) { 
            if(sParent->getKey() > sRight_child->getKey()) {
                sParent->setLeft(sRight_child);
                sRight_child->setParent(sParent); 
            }
            else {
                sParent->setRight(sRight_child);
                sRight_child->setParent(sParent); 
            }
        }
        else { //successor's right child is NULL
            if(sParent->getKey() > successor_node->getKey()) { //if successor is smaller than it's former parent
                sParent->setLeft(NULL);
            }
            else {
                sParent->setRight(NULL);
            }
        }
        //attaching successor's previous left child to it's former parent
        if(dLeft_child != successor_node) {
            successor_node->setLeft(dLeft_child); //deleted node's left child becomes successor node's left child
            dLeft_child->setParent(successor_node); //successor is the parent of the node's left child
        }
        if(dRight_child != successor_node) {
            successor_node->setRight(dRight_child); //deleted node's right child becomes successor node's right child
            dRight_child->setParent(successor_node); //successor is the parent of deleted node's right child
        }
    }
    deleted_node->setLeft(NULL);
    deleted_node->setRight(NULL);
    deleted_node->setParent(NULL);
    deleted_node = NULL; //set deleted node to point to NULL
    delete deleted_node;
}    


/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif