#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
//CTOR AND DTOR 
    //ctor (alias - key, alias - value, node ptr - parent)
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    
    //virtual dtor 
    virtual ~Node();

//GETTER FUNCTIONS 
    //const getItem - returns pair <K, V> 
    const std::pair<const Key, Value>& getItem() const;

    //getItem - returns pair <K, V>
    std::pair<const Key, Value>& getItem();

    //const getKey - returns alias to key 
    const Key& getKey() const;

    //const getValue - returns alias to value 
    const Value& getValue() const;

    //getValue - returns alias to value 
    Value& getValue();

//VIRTUAL FUNCTIONS
    //virtual getParent - returns node ptr to parent 
    virtual Node<Key, Value>* getParent() const;

    //virtual getLeft - returns node ptr to left  
    virtual Node<Key, Value>* getLeft() const;
    
    //virtual getRight - returns node ptr to right 
    virtual Node<Key, Value>* getRight() const;

//MODIFICATION FUNCTIONS
    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
//DATA MEMBERS 
    //item itself, a pair of <K, V> 
    std::pair<const Key, Value> item_;

    //ptr to parent, l, r
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value), //fills item's k, v 
    parent_(parent), //fills item's parent 
    left_(NULL), //sets l to null 
    right_(NULL) //sets r to null 
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:

    //ctor 
    BinarySearchTree(); //TODO

    //virtual dtor 
    virtual ~BinarySearchTree(); //TODO

    //virtual insert: add new node to the tree, does NOT need to balance 
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO

    //virtual remove: remove specified node, does NOTneed to balance 
    virtual void remove(const Key& key); //TODO

    //clear - delete all nodes, turns into empty tree 
    void clear(); //TODO

    //isBalanced: T if AVL tree (balanced)
    bool isBalanced() const; //TODO

    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const; // returns iterator to smallest node
    iterator end() const; //returns iterator to 1 after the biggest node 
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
// Mandatory helper functions
    //returns ptr to the node w key 
    Node<Key, Value>* internalFind(const Key& k) const; // TODO

    //returns ptr to node w smallest key 
    Node<Key, Value> *getSmallestNode() const;  // TODO

    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    static Node<Key, Value>* successor(Node<Key, Value>* current);
    void clearSubtrees (Node<Key, Value>* n);
    int getHeight(Node<Key, Value>* n) const;
    bool balanceHelper(Node<Key, Value>* n) const;
    void removeNode(Node<Key, Value>* n);

protected:
    //ptr to root node 
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr): 
    current_(ptr) //set current to root 
{
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(): 
    current_(nullptr)
{

}

/**
* Provides access to the item.
    returns alias to a pair of key and value
    operator overloading for dereference 
    returns the item that iterator is currently at 
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
    return type: ptr to pair of k,v 
    operator overloading for -> 
    returns dereference  of current item 
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    if ((this->current_)==rhs.current_) {
        return true; 
    }
    else return false; 
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    if ((this->current_)!=(rhs.current_)) {
    return true; 
    }
    else return false; 

}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
  //BC: curr is null  
  if (current_==nullptr) return *this; 

  //case 1: curr has Rchild - go to Left most grandchild 
  if (current_->getRight()!=nullptr) {
    current_=current_->getRight();
    while (current_->getLeft()!=nullptr) {
      current_=current_->getLeft();
    }
  }

  //case 2: curr has no R child 
  else {
    Node<Key, Value>* p = current_->getParent(); 
    while (p!=nullptr && current_==p->getRight()) {
      current_=p;
      p=p->getParent();
    }
    current_=p;
  }

  return *this; 
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree(): root_(nullptr)
{
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clearSubtrees(root_);
    root_=nullptr;

}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    Key k = keyValuePair.first; //key 
    Value v = keyValuePair.second; //value 
    

    //BC1: tree is empty 
    if (root_==nullptr) {
        root_ = new Node<Key, Value>(k, v, nullptr);  
        return;
    }
    
    //BC2: key is already in tree 
    Node<Key, Value>* item = internalFind(k); //use internal find: if null then key DNE, else it returns ptr to the node with that key 
    if (item!=nullptr) {  

        //overwrite current value w updated value 
        item->setValue(v); 
        return;
    }
    //else key is new to the tree -
    //make new node, parent null for now, and insert it 
    Node<Key, Value>* n = new Node<Key, Value>(k, v, nullptr); 
    
    //walk to correct leaf node

    Node<Key, Value>* curr = root_; //ptr curr to hold current position 
    Node<Key, Value>* parent = nullptr; //ptr to parent 

    //while the ideal location is taken: 
    while (curr!=nullptr) {
        if (k==curr->getKey()) {
          curr->setValue(v);
          return; 
        }
        /*inserting new node: 
        1. compare node to temp  
        2. if node's key < temp's key -> go to L subtree 
        3. else go to R subtree 
        */
        parent = curr; //child becomes parent 

        if (k<curr->getKey()) {
            curr = curr->getLeft(); 
        }
        else {
            curr = curr->getRight(); 
        }
    }

    //now we are at the end of the map, either L or R child is nullptr, curr is null 
    if (k<parent->getKey()) {
        parent->setLeft(n);
    }
    else {
        parent->setRight(n);
    }
    n->setParent(parent); 
    return; 
    
}

//my helper function to remove a single node 
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeNode(Node<Key, Value>* n) {
  //BC nullptr
  if (n==nullptr) return;

  Node<Key, Value>* parent = n->getParent();
  Node<Key, Value>* lChild = n->getLeft();
  Node<Key, Value>* rChild = n->getRight();

  // Case 1 : leaf
  if (lChild == nullptr && rChild == nullptr) {
      if (parent == nullptr) { // deleting root only node
          root_ = nullptr;
      } else {
          if (parent->getLeft() == n) parent->setLeft(nullptr);
          else parent->setRight(nullptr);
      }
      delete n;
      return;
  }

  // Case2 : 1 child
  Node<Key, Value>* child = (lChild != nullptr) ? lChild : rChild;
  if (parent == nullptr) { // n is root
      root_ = child;
      child->setParent(nullptr);
  } else {
      if (parent->getLeft() == n) parent->setLeft(child);
      else parent->setRight(child);
      child->setParent(parent);
  }


  delete n;
  return;
}

/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    //BC 1: empty tree 
    if (root_==nullptr) return;

    //find node 
    Node<Key, Value>* n = internalFind(key); 

    //BC 2: removing a node that DNE 
    if (n==nullptr) {
        return;
    }
    
    //C3: node exists
    Node<Key, Value>* parent=n->getParent();
    Node<Key, Value>* lChild = n->getLeft(); 
    Node<Key, Value>* rChild = n->getRight(); 

        //subcase 1: no children (a leaf node)
    if (lChild==nullptr && rChild==nullptr){

        //sub sub case: if deleting root 
        if (n==root_) {
            root_=nullptr;
            delete n;
            return; 
        }

       if (parent->getLeft()==n) { //if n was the L child 
            parent->setLeft(nullptr); 
       }
       else if (parent->getRight()==n) { //if n was R child
            parent->setRight(nullptr);
       }
       else {

       }


       delete n; 
       return; 
    }
        //subcase 2: 1 child 
    else if (lChild==nullptr || rChild==nullptr) {
        
        Node<Key, Value>* parent=n->getParent();
        Node<Key, Value>* temp;

        //assign temp to whichever child exists 
        if (lChild==nullptr) {
            temp=rChild;
        }
        else {
            temp=lChild;
        }


        if (parent==nullptr){ //if n is root with 1 child, only child remains and the child becomes root 
            root_=temp;
            temp->setParent(nullptr);
        }
        else if (parent->getLeft()==n) {
            parent->setLeft(temp);
            temp->setParent(parent); 
        }
        else {
            parent->setRight(temp);
            temp->setParent(parent);
        }
        delete n;
        return; 

    }
        //subcase 3: 2 children - swap w predecessor then remove 
    else {
        // Save original key
      Key delKey = n->getKey();

      // Find predecessor (this node always has <=1 child)
      Node<Key, Value>* pred = predecessor(n);

      // Swap nodes in the tree
      nodeSwap(n, pred);

      // After swap, either n or pred contains delKey — find which one
      Node<Key, Value>* toDelete = (n->getKey() == delKey ? n : pred);

      // Remove the node that now contains the original key
      removeNode(toDelete);
      return;
    }
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    if (current==nullptr) return current; 
    
    Node<Key, Value>* temp = current; 

    //If left child exists, predecessor is the right most node of the left subtree
    if (current->getLeft()!=nullptr) { 
        temp = current->getLeft(); 
        while (temp->getRight()!=nullptr) {
            temp=temp->getRight(); 
        }
        //at this point the right child == nullptr so we are at the right most bottom most node of the left subtree 
        return temp;
    }

    /*Else walk up the ancestor chain until you
    traverse the first right child pointer (find
    the first node who is a right child of his
    parent…that parent is the predecessor)
    – If you get to the root w/o finding a node
    who is a right child, there is no predecessor*/ 
    else {
        temp = temp->getParent();
        while (temp!=nullptr && current==temp->getLeft()) {
            current = temp; 
            temp = temp->getParent();
        }
    }

    return temp; 
    
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current){
   /* If right child exists, successor is the
left most node of the right subtree*/
    if (current == nullptr) return current;
    
    Node<Key, Value>* temp = current; 

    if (temp->getRight()!=nullptr) {
        temp=temp->getRight(); 
        while (temp->getLeft()!=nullptr) {
            temp = temp->getLeft(); 
        }
        //at this point we have reached the left most node of the right subtree, there are no more left children 
        return temp; 
    }

/*• Else walk up the ancestor chain until
you traverse the first left child pointer
(find the first node who is a left child
of his parent…that parent is the
successor)
– If you get to the root w/o finding a node
who is a left child, there is no successor*/
    else {
        temp = temp->getParent(); 
        while (temp!=nullptr && current==temp->getRight()) {
            //when current = parent's left child that parent is the successor 
            current = temp; 
            temp= temp->getParent(); 
        }
        //at this point we have reached the first left child pointer 
        return temp; 
    }

}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    //BC1: empty tree 
    if (root_==nullptr) return;

    clearSubtrees(root_);
    root_=nullptr; 
    return; 
}

//my helper function for clear()
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearSubtrees (Node<Key, Value>* n) {
    //BC: if n=nullptr 
    if (n==nullptr) return; 
    clearSubtrees(n->getLeft());
    clearSubtrees(n->getRight());

    delete n; 
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    //BC: empty tree 
    if (root_==nullptr) return nullptr; 

    //smallest node will be the left most subtree 
    Node<Key, Value>* temp = root_; 
    while (temp->getLeft()!=nullptr ){
        temp = temp->getLeft();
    }
    //at this point L child DNE so we are at the left most subtree / leaf 
    return temp; 
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    //given a key go down the tree depending on its value compared to the value of each node 

    //BC1: empty tree 
    if (root_==nullptr) return nullptr; 

    Node<Key, Value>* temp = root_; 

    //while we are in the tree 
    while (temp!=nullptr) {
        if (key==temp->getKey()) {
            return temp; 
        }
        else if (key < temp->getKey()) {
            temp=temp->getLeft();
        }
        else if (key > temp->getKey()) {
            temp = temp->getRight();
        }
    }
    //at this point temp = nullptr so we fell off the tree without finding an equal key 
    return nullptr; 
}


//my recursive helper function to get height of a subtree for isBalanced function 
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* n) const {
    //BC: n is null 
    if (n==nullptr) return 0; 

    return 1+std::max(getHeight(n->getLeft()), getHeight(n->getRight()));
}


//my recursive helper for isBalanced to check balance for each subtree 
template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balanceHelper(Node<Key, Value>* n) const {
    //BC no child 
    if (n==nullptr) return true; 
    
    int lHeight=getHeight(n->getLeft());
    int rHeight=getHeight(n->getRight());

    if (std::abs(lHeight - rHeight)>1) return false; 

    return balanceHelper(n->getLeft()) && balanceHelper(n->getRight());

}


/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    //BC: root is null 
    if (root_==nullptr) return true; 

    if (balanceHelper(root_)==true)
 return true; 
    return false; 
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }


}


/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif