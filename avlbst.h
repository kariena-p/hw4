#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance__ as a data member, plus
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

    // Getter/setter for the node's balance.
    int8_t getBalance () const;
    void setBalance (int8_t balance__);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance__;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance__(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance__ of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance__;
}

/**
* A setter for the balance__ of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance__)
{
    // CRITICAL FIX: Was 'balance__ = balance__;' (self-assignment).
    // This bug prevented ALL balance factors from ever being updated.
    this->balance__ = balance__;
}

/**
* Adds diff to the balance__ of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance__ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item);
    virtual void remove(const Key& key);
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent);
    void rotateLeft(AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);

    void debugPrint() const;
    void printInOrderHelper(Node<Key,Value>* node) const;
};



// Rotations
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
#ifdef DEBUG
std::cout << "start rotate l fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

    if (!n) return;
    AVLNode<Key, Value>* r = n->getRight();
    if (!r) return;
    AVLNode<Key, Value>* rl = r->getLeft();

    r->setLeft(n);
    AVLNode<Key, Value>* parent = n->getParent();
    r->setParent(parent);
    n->setParent(r);

    n->setRight(rl);
    if (rl) rl->setParent(n);

    if (!parent) this->root_ = r;
    else if (parent->getLeft() == n) parent->setLeft(r);
    else parent->setRight(r);
    
#ifdef DEBUG
std::cout << "end rotate l fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {

#ifdef DEBUG
std::cout << "start rotate right fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

    if (!n) return;
    AVLNode<Key, Value>* l = n->getLeft();
    if (!l) return;
    AVLNode<Key, Value>* lr = l->getRight();

    l->setRight(n);
    AVLNode<Key, Value>* parent = n->getParent();
    l->setParent(parent);
    n->setParent(l);

    n->setLeft(lr);
    if (lr) lr->setParent(n);

    if (!parent) this->root_ = l;
    else if (parent->getLeft() == n) parent->setLeft(l);
    else parent->setRight(l);

#ifdef DEBUG
std::cout << "end rotate right fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent) {
// grand: the node whose balance we are currently checking/fixing.
// parent: the child of grand that caused the height increase (i.e., the node whose balance we just fixed).

    if (grand == nullptr) return;

    // Determine balance change (diff) at grand due to parent's height increase
    int8_t diff;
    if (parent == grand->getLeft()) {
        diff = -1; // Left side grew
    } else {
        diff = 1; // Right side grew
    }
    
    // Logic Fix: Only update balance if we are propagating (i.e., grand is not the node that just became ±2 in `insert`).
    // If grand is already ±2, the balance was set in `insert` (Case C), and updating it here would make it ±3.
    if (grand->getBalance() != -2 && grand->getBalance() != 2) {
        grand->updateBalance(diff);
    }

    int8_t gb = grand->getBalance();

    // Case 1: grand becomes 0
    if (gb == 0) {
        return; // Height of subtree at grand did not change (it went from 1/0 to 1/1) → stop
    }

    // Case 2: grand becomes ±1
    if (gb == -1 || gb == 1) {
        // Height of subtree at grand increased → propagate up
        insertFix(grand->getParent(), grand);
        return;
    }

    // Case 3: grand is ±2 (rotation needed)
    
    if (gb == -2) { // Left subtree is too tall
        if (parent->getBalance() == -1) { 
            // Left-Left (zig-zig)
            rotateRight(grand);
            parent->setBalance(0);
            grand->setBalance(0);
        } else { // parent->getBalance() == 1
            // Left-Right (zig-zag)
            AVLNode<Key, Value>* lr = parent->getRight();
            rotateLeft(parent); // This changes parent's right child to lr, and lr's parent to parent
            rotateRight(grand);

            int8_t nb = lr->getBalance(); // Check the balance of the *new* root (lr)
            if (nb == -1) { grand->setBalance(1); parent->setBalance(0); }
            else if (nb == 0) { grand->setBalance(0); parent->setBalance(0); }
            else { grand->setBalance(0); parent->setBalance(-1); }
            lr->setBalance(0);
        }
    } else if (gb == 2) { // Right subtree is too tall
        if (parent->getBalance() == 1) {
            // Right-Right (zig-zig)
            rotateLeft(grand);
            parent->setBalance(0);
            grand->setBalance(0);
        } else { // parent->getBalance() == -1
            // Right-Left (zig-zag)
            AVLNode<Key, Value>* rl = parent->getLeft();
            rotateRight(parent);
            rotateLeft(grand);

            int8_t nb = rl->getBalance(); // Check the balance of the *new* root (rl)
            if (nb == 1) { grand->setBalance(-1); parent->setBalance(0); }
            else if (nb == 0) { grand->setBalance(0); parent->setBalance(0); }
            else { grand->setBalance(0); parent->setBalance(1); }
            rl->setBalance(0);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
#ifdef DEBUG
std::cout << "start insert fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

    Key k = new_item.first;
    Value v = new_item.second;

    // empty tree
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(k, v, nullptr);
#ifdef DEBUG
std::cout << "end insert fn - ROOT = NULLPTR - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
        return;
    }

    // find insertion point
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (curr != nullptr) {
        parent = curr;
        if (k < curr->getKey()) curr = curr->getLeft();
        else if (curr->getKey() < k) curr = curr->getRight();
        else { // key exists
            curr->setValue(v);
#ifdef DEBUG
std::cout << "end insert fn - LINE 280, KEY EXISTS - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
            return;
        }
    }

    //at this point curr == nullptr so we are at a leaf 

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(k, v, parent);

    if (k < parent->getKey()) parent->setLeft(newNode);
    else parent->setRight(newNode);

    // Initial update on parent's balance
    // Determine the balance change (diff) at the parent
    int8_t diff = (newNode == parent->getLeft() ? -1 : 1); // Note: diff is -1 for left, +1 for right
    
    // Case A: Parent's balance was 0
    if (parent->getBalance() == 0) {
        parent->updateBalance(diff);
        // Height of parent's subtree increased, continue fix up.
        insertFix(parent->getParent(), parent); // Propagate up
    } 
    // Case B: Parent's balance was -diff (now it's 0)
    else if (parent->getBalance() == -diff) { 
        // Parent's balance becomes 0. Height didn't change (it went from 1/0 to 1/1).
        parent->setBalance(0);
        // Stop propagation.
    }
    // Case C: Parent's balance was diff (now it becomes ±2)
    else { // parent->getBalance() == diff 
        // Unbalanced → parent's balance is now ±2. 
        parent->updateBalance(diff); // Now parent is ±2 (e.g., -1 to -2)
        // Call fix. insertFix must check if the grand's balance is already ±2 and skip the update.
        insertFix(parent, newNode); 
    }

#ifdef DEBUG
std::cout << "end insert fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
}



// --- REMOVE FIX ---
template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) {
#ifdef DEBUG
std::cout << "start remove fix fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

    while (n != nullptr) {
        n->updateBalance(diff);

        int8_t b = n->getBalance();
        AVLNode<Key, Value>* parent = n->getParent();

        if (b == -2) {
            AVLNode<Key, Value>* leftChild = n->getLeft();
            // Defensive check (b=-2 implies leftChild exists)
            if (leftChild == nullptr) return; 

            if (leftChild->getBalance() <= 0) {
                // Left-Left
                rotateRight(n);
                if (leftChild->getBalance() == 0) {
                    n->setBalance(-1);
                    leftChild->setBalance(1);
                    break;
                } else {
                    n->setBalance(0);
                    leftChild->setBalance(0);
                    
                    AVLNode<Key, Value>* next_n = leftChild->getParent();
                    if (next_n) {
                        diff = (next_n->getLeft() == leftChild ? +1 : -1); 
                    }
                    n = next_n;
                    continue;
                }
            } else {
                // Left-Right
                AVLNode<Key, Value>* lr = leftChild->getRight();
                // lr must exist if leftChild->getBalance() == 1
                if (lr == nullptr) return;

                rotateLeft(leftChild);
                rotateRight(n);

                int8_t bLR = lr->getBalance();
                // CORRECTED BALANCE ASSIGNMENTS:
                if (bLR == -1) { 
                    n->setBalance(1); leftChild->setBalance(0); // FIX: was (0, 1)
                }
                else if (bLR == 0) { 
                    n->setBalance(0); leftChild->setBalance(0); 
                }
                else { // bLR == 1
                    n->setBalance(0); leftChild->setBalance(-1); // FIX: was (-1, 0)
                }
                lr->setBalance(0);

                AVLNode<Key, Value>* next_n = lr->getParent();
                if (next_n) {
                    diff = (next_n->getLeft() == lr ? +1 : -1); 
                }
                n = next_n;
                continue;
            }
        } else if (b == 2) {
            AVLNode<Key, Value>* rightChild = n->getRight();
            // Defensive check (b=2 implies rightChild exists)
            if (rightChild == nullptr) return; 

            if (rightChild->getBalance() >= 0) {
                // Right-Right
                rotateLeft(n);
                if (rightChild->getBalance() == 0) {
                    n->setBalance(1);
                    rightChild->setBalance(-1);
                    break;
                } else {
                    n->setBalance(0);
                    rightChild->setBalance(0);
                    
                    AVLNode<Key, Value>* next_n = rightChild->getParent();
                    if (next_n) {
                        diff = (next_n->getLeft() == rightChild ? +1 : -1); 
                    }
                    n = next_n;
                    continue;
                }
            } else {
                // Right-Left
                AVLNode<Key, Value>* rl = rightChild->getLeft();
                // rl must exist if rightChild->getBalance() == -1
                if (rl == nullptr) return;

                rotateRight(rightChild);
                rotateLeft(n);

                int8_t bRL = rl->getBalance();
                // CORRECTED BALANCE ASSIGNMENTS:
                if (bRL == 1) { 
                    n->setBalance(-1); rightChild->setBalance(0); // FIX: was (0, -1)
                }
                else if (bRL == 0) { 
                    n->setBalance(0); rightChild->setBalance(0); 
                }
                else { // bRL == -1
                    n->setBalance(0); rightChild->setBalance(1); // FIX: was (1, 0)
                }
                rl->setBalance(0);

                AVLNode<Key, Value>* next_n = rl->getParent();
                if (next_n) {
                    diff = (next_n->getLeft() == rl ? +1 : -1);
                }
                n = next_n;
                continue;
            }
        } else if (b == -1 || b == 1) {
            // Height unchanged (or height decrease stopped here), stop propagation
            break;
        } else { // b == 0
            // Height decreased, continue up
            if (parent != nullptr)
                diff = (parent->getLeft() == n ? +1 : -1);
            n = parent;
        }
    }
#ifdef DEBUG
std::cout << "end remove fix fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
}


// Remove
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {

#ifdef DEBUG
std::cout << "start remove fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif


    AVLNode<Key, Value>* z = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!z) return;

    AVLNode<Key, Value>* parent = z->getParent();
    int8_t diff = 0; // Difference to apply to parent's balance

    if (z->getLeft() && z->getRight()) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(z));
        nodeSwap(z, pred);
        parent = z->getParent(); // Parent of z (old predecessor) is updated
    }

    AVLNode<Key, Value>* child = z->getLeft() ? z->getLeft() : z->getRight();
    if (child) child->setParent(parent);

    // Determine the diff based on which child z was of its final parent
    if (parent) diff = (parent->getLeft() == z ? +1 : -1); // +1 means left side height decreased, -1 means right side height decreased

    if (!parent) this->root_ = child;
    else if (parent->getLeft() == z) parent->setLeft(child);
    else parent->setRight(child);

    delete z;
    removeFix(parent, diff);


    #ifdef DEBUG
std::cout << "end remove fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{

#ifdef DEBUG
std::cout << "start node swap fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif

    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);

#ifdef DEBUG
std::cout << "end node swap fn - printing AVL in-order" << std::endl;
this->debugPrint();
#endif
}


#ifdef DEBUG
template<class Key, class Value>
void AVLTree<Key, Value>::debugPrint() const {
    std::cout << "AVL In-order: ";
    printInOrderHelper(this->root_);
    std::cout << std::endl;
}


template<class Key, class Value>
void AVLTree<Key, Value>::printInOrderHelper(Node<Key,Value>* node) const {
    if (!node) return;

    auto* avn = static_cast<AVLNode<Key,Value>*>(node);

    printInOrderHelper(node->getLeft());
    std::cout << "(" 
              << node->getKey()
              << ", b=" << int(avn->getBalance())
              << ") ";
    printInOrderHelper(node->getRight());
}

#endif


#endif

// #ifndef AVLBST_H
// #define AVLBST_H

// #include <iostream>
// #include <exception>
// #include <cstdlib>
// #include <cstdint>
// #include <algorithm>
// #include "bst.h"

// struct KeyError { };

// /**
// * A special kind of node for an AVL tree, which adds the balance__ as a data member, plus
// * other additional helper functions. You do NOT need to implement any functionality or
// * add additional data members or helper functions.
// */
// template <typename Key, typename Value>
// class AVLNode : public Node<Key, Value>
// {
// public:
//     // Constructor/destructor.
//     AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
//     virtual ~AVLNode();

//     // Getter/setter for the node's height.
//     int8_t getBalance () const;
//     void setBalance (int8_t balance__);
//     void updateBalance(int8_t diff);

//     // Getters for parent, left, and right. These need to be redefined since they
//     // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
//     // for more information.
//     virtual AVLNode<Key, Value>* getParent() const override;
//     virtual AVLNode<Key, Value>* getLeft() const override;
//     virtual AVLNode<Key, Value>* getRight() const override;

// protected:
//     int8_t balance__;    // effectively a signed char
// };

// /*
//   -------------------------------------------------
//   Begin implementations for the AVLNode class.
//   -------------------------------------------------
// */

// /**
// * An explicit constructor to initialize the elements by calling the base class constructor
// */
// template<class Key, class Value>
// AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
//     Node<Key, Value>(key, value, parent), balance__(0)
// {

// }

// /**
// * A destructor which does nothing.
// */
// template<class Key, class Value>
// AVLNode<Key, Value>::~AVLNode()
// {

// }

// /**
// * A getter for the balance__ of a AVLNode.
// */
// template<class Key, class Value>
// int8_t AVLNode<Key, Value>::getBalance() const
// {
//     return balance__;
// }

// /**
// * A setter for the balance__ of a AVLNode.
// */
// template<class Key, class Value>
// void AVLNode<Key, Value>::setBalance(int8_t balance__)
// {
//     balance__ = balance__;
// }

// /**
// * Adds diff to the balance__ of a AVLNode.
// */
// template<class Key, class Value>
// void AVLNode<Key, Value>::updateBalance(int8_t diff)
// {
//     balance__ += diff;
// }

// /**
// * An overridden function for getting the parent since a static_cast is necessary to make sure
// * that our node is a AVLNode.
// */
// template<class Key, class Value>
// AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
// {
//     return static_cast<AVLNode<Key, Value>*>(this->parent_);
// }

// /**
// * Overridden for the same reasons as above.
// */
// template<class Key, class Value>
// AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
// {
//     return static_cast<AVLNode<Key, Value>*>(this->left_);
// }

// /**
// * Overridden for the same reasons as above.
// */
// template<class Key, class Value>
// AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
// {
//     return static_cast<AVLNode<Key, Value>*>(this->right_);
// }


// /*
//   -----------------------------------------------
//   End implementations for the AVLNode class.
//   -----------------------------------------------
// */


// template <class Key, class Value>
// class AVLTree : public BinarySearchTree<Key, Value>
// {
// public:
//     virtual void insert (const std::pair<const Key, Value> &new_item); // TODO - done 
//     virtual void remove(const Key& key);  // TODOdone
// protected:
//     virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

//     // Add helper functions here
//     void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node); // done 
//     void rotateLeft(AVLNode<Key, Value>* n); //done
//     void rotateRight(AVLNode<Key, Value>* n); // done
//     void removeFix(AVLNode<Key, Value>* n, int8_t diff);

//     void debugPrint() const;
//     void printInOrderHelper(Node<Key,Value>* node) const;
// };



// // Rotations
// template<class Key, class Value>
// void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
// #ifdef DEBUG
// std::cout << "start rotate l fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

//     if (!n) return;
//     AVLNode<Key, Value>* r = n->getRight();
//     if (!r) return;
//     AVLNode<Key, Value>* rl = r->getLeft();

//     r->setLeft(n);
//     AVLNode<Key, Value>* parent = n->getParent();
//     r->setParent(parent);
//     n->setParent(r);

//     n->setRight(rl);
//     if (rl) rl->setParent(n);

//     if (!parent) this->root_ = r;
//     else if (parent->getLeft() == n) parent->setLeft(r);
//     else parent->setRight(r);
    
// #ifdef DEBUG
// std::cout << "end rotate l fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
// }

// template<class Key, class Value>
// void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {

// #ifdef DEBUG
// std::cout << "start rotate right fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

//     if (!n) return;
//     AVLNode<Key, Value>* l = n->getLeft();
//     if (!l) return;
//     AVLNode<Key, Value>* lr = l->getRight();

//     l->setRight(n);
//     AVLNode<Key, Value>* parent = n->getParent();
//     l->setParent(parent);
//     n->setParent(l);

//     n->setLeft(lr);
//     if (lr) lr->setParent(n);

//     if (!parent) this->root_ = l;
//     else if (parent->getLeft() == n) parent->setLeft(l);
//     else parent->setRight(l);

// #ifdef DEBUG
// std::cout << "end rotate right fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
// }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// template<class Key, class Value>
// void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent) {
// // grand: the node whose balance we are currently checking/fixing.
// // parent: the child of grand that caused the height increase (i.e., the node whose balance we just fixed).

//     if (grand == nullptr) return;

//     // Determine balance change (diff) at grand due to parent's height increase
//     int8_t diff;
//     if (parent == grand->getLeft()) {
//         diff = -1; // Left side grew
//     } else {
//         diff = 1; // Right side grew
//     }

//     grand->updateBalance(diff);
//     int8_t gb = grand->getBalance();

//     // Case 1: grand becomes 0
//     if (gb == 0) {
//         return; // Height of subtree at grand did not change (it went from 1/0 to 1/1) → stop
//     }

//     // Case 2: grand becomes ±1
//     if (gb == -1 || gb == 1) {
//         // Height of subtree at grand increased → propagate up
//         insertFix(grand->getParent(), grand);
//         return;
//     }

//     // Case 3: grand is ±2 (rotation needed)
//     AVLNode<Key, Value>* node = (diff == -1) ? parent->getLeft() : parent->getRight(); // The node that was just inserted/fixed.
    
//     if (gb == -2) { // Left subtree is too tall
//         if (parent->getBalance() == -1) { 
//             // Left-Left (zig-zig)
//             rotateRight(grand);
//             parent->setBalance(0);
//             grand->setBalance(0);
//         } else { // parent->getBalance() == 1
//             // Left-Right (zig-zag)
//             AVLNode<Key, Value>* lr = parent->getRight();
//             rotateLeft(parent); // This changes parent's right child to lr, and lr's parent to parent
//             rotateRight(grand);

//             int8_t nb = lr->getBalance(); // Check the balance of the *new* root (lr)
//             if (nb == -1) { grand->setBalance(1); parent->setBalance(0); }
//             else if (nb == 0) { grand->setBalance(0); parent->setBalance(0); }
//             else { grand->setBalance(0); parent->setBalance(-1); }
//             lr->setBalance(0);
//         }
//     } else if (gb == 2) { // Right subtree is too tall
//         if (parent->getBalance() == 1) {
//             // Right-Right (zig-zig)
//             rotateLeft(grand);
//             parent->setBalance(0);
//             grand->setBalance(0);
//         } else { // parent->getBalance() == -1
//             // Right-Left (zig-zag)
//             AVLNode<Key, Value>* rl = parent->getLeft();
//             rotateRight(parent);
//             rotateLeft(grand);

//             int8_t nb = rl->getBalance(); // Check the balance of the *new* root (rl)
//             if (nb == 1) { grand->setBalance(-1); parent->setBalance(0); }
//             else if (nb == 0) { grand->setBalance(0); parent->setBalance(0); }
//             else { grand->setBalance(0); parent->setBalance(1); }
//             rl->setBalance(0);
//         }
//     }
// }
// // template<class Key, class Value>
// // void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
// // #ifdef DEBUG
// // std::cout << "start insert FIX fn - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif

// //     if (parent == nullptr) return;

// //     AVLNode<Key, Value>* grand = parent->getParent();
// //     if (grand == nullptr) {
// // #ifdef DEBUG
// // std::cout << "end insert FIX fn - grand=null - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif
// //       return; // reached root
// //     }

// //     bool parentIsLeft = (parent == grand->getLeft());

// //     // update grandparent balance
// //     grand->updateBalance(parentIsLeft ? -1 : 1);

// //     int8_t gb = grand->getBalance();

// //     // Case 1: grand becomes 0 → stop
// //     if (gb == 0) {
// // #ifdef DEBUG
// // std::cout << "end insert FIX fn - CASE 1, grand becomes 0 - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif
// //       return;
// //     }

// //     // Case 2: grand becomes ±1 → propagate up
// //     if (gb == -1 || gb == 1) {
// //         insertFix(grand, parent);
// // #ifdef DEBUG
// // std::cout << "end insert FIX fn - CASE 2, grand becomes -+1 propogate up - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif
// //         return;
// //     }

// //     // Case 3: grand is ±2 → rotation needed
// //     if (gb == -2) {
// // #ifdef DEBUG
// // std::cout << " insert FIX fn - CASE 3, grand -+2 rotation needed - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif
// //         if (node == parent->getLeft()) {
// //             // Left-Left (zig-zig)
// //             rotateRight(grand);
// //             parent->setBalance(0);
// //             grand->setBalance(0);
// //         } else {
// //             // Left-Right (zig-zag)
// //             rotateLeft(parent);
// //             rotateRight(grand);
// //             int8_t nb = node->getBalance();
// //             if (nb == -1) { parent->setBalance(0); grand->setBalance(1); node->setBalance(0); }
// //             else if (nb == 0) { parent->setBalance(0); grand->setBalance(0); node->setBalance(0); }
// //             else { parent->setBalance(-1); grand->setBalance(0); node->setBalance(0); }
// //         }
// //     } else if (gb == 2) {
// //         if (node == parent->getRight()) {
// //             // Right-Right (zig-zig)
// //             rotateLeft(grand);
// //             parent->setBalance(0);
// //             grand->setBalance(0);
// //         } else {
// //             // Right-Left (zig-zag)
// //             rotateRight(parent);
// //             rotateLeft(grand);
// //             int8_t nb = node->getBalance();
// //             if (nb == 1) { parent->setBalance(0); grand->setBalance(-1); node->setBalance(0); }
// //             else if (nb == 0) { parent->setBalance(0); grand->setBalance(0); node->setBalance(0); }
// //             else { parent->setBalance(1); grand->setBalance(0); node->setBalance(0); }
// //         }
// //     }
// // #ifdef DEBUG
// // std::cout << "end insert FIX fn - printing AVL in-order" << std::endl;
// // this->debugPrint();
// // #endif
// // }

// template<class Key, class Value>
// void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
// #ifdef DEBUG
// std::cout << "start insert fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

//     Key k = new_item.first;
//     Value v = new_item.second;

//     // empty tree
//     if (this->root_ == nullptr) {
//         this->root_ = new AVLNode<Key, Value>(k, v, nullptr);
// #ifdef DEBUG
// std::cout << "end insert fn - ROOT = NULLPTR - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
//         return;
//     }

//     // find insertion point
//     AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
//     AVLNode<Key, Value>* parent = nullptr;

//     while (curr != nullptr) {
//         parent = curr;
//         if (k < curr->getKey()) curr = curr->getLeft();
//         else if (curr->getKey() < k) curr = curr->getRight();
//         else { // key exists
//             curr->setValue(v);
// #ifdef DEBUG
// std::cout << "end insert fn - LINE 280, KEY EXISTS - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
//             return;
//         }
//     }

//     //at this point curr == nullptr so we are at a leaf 

//     AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(k, v, parent);

//     if (k < parent->getKey()) parent->setLeft(newNode);
//     else parent->setRight(newNode);

//     // Initial update on parent's balance
//     // newNode is always inserted as a leaf, so its balance is 0.
//     // The height of the subtree rooted at parent *might* change.
//     // Determine the balance change (diff) at the parent
//     int8_t diff = (newNode == parent->getLeft() ? +1 : -1);
    
//     // Case A: Parent's balance was 0
//     if (parent->getBalance() == 0) {
//         parent->updateBalance(diff);
//         // Height of parent's subtree increased, continue fix up.
//         insertFix(parent->getParent(), parent); // Propagate up
//     } 
//     // Case B: Parent's balance was diff (now it's 2*diff, i.e., ±2)
//     else if (parent->getBalance() == -diff) { // e.g., parent balance was -1, diff is +1 (LR) -> becomes 0
//         // Parent's balance becomes 0. Height of parent's subtree didn't change (it went from 1/0 to 1/1).
//         parent->setBalance(0);
//         // Stop propagation.
//     }
//     // Case C: Parent's balance was -diff (now it's 0)
//     else { // parent->getBalance() == diff // e.g., parent balance was -1, diff is -1 (LL) -> becomes -2
//         // Unbalanced → parent's balance is now ±2. 
//         parent->updateBalance(diff); // Now parent is ±2
//         insertFix(parent, newNode); // Call fix with parent (the ±2 node) and newNode (the trigger)
//     }

//     // // set initial balance for parent
//     // int8_t pb = parent->getBalance();
//     // if (pb == 0) {
//     //     if (newNode == parent->getLeft()) parent->setBalance(-1);
//     //     else parent->setBalance(1);
//     //     insertFix(parent, newNode);
//     // } else {
//     //     // parent was ±1 → now becomes 0, tree remains balanced
//     //     parent->setBalance(0);
//     // }
// #ifdef DEBUG
// std::cout << "end insert fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
// }



// // --- REMOVE FIX ---
// template <class Key, class Value>
// void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) {
// #ifdef DEBUG
// std::cout << "start remove fix fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

//     while (n != nullptr) {
//         n->updateBalance(diff);

//         int8_t b = n->getBalance();
//         AVLNode<Key, Value>* parent = n->getParent();

//         if (b == -2) {
//             AVLNode<Key, Value>* leftChild = n->getLeft();
//             if (leftChild->getBalance() <= 0) {
//                 // Left-Left
//                 rotateRight(n);
//                 if (leftChild->getBalance() == 0) {
//                     n->setBalance(-1);
//                     leftChild->setBalance(1);
//                     break;
//                 } else {
//                     n->setBalance(0);
//                     leftChild->setBalance(0);
//                     n = leftChild->getParent();
//                     diff = (n && n->getParent() && n->getParent()->getLeft() == n ? +1 : -1);
//                     continue;
//                 }
//             } else {
//                 // Left-Right
//                 AVLNode<Key, Value>* lr = leftChild->getRight();
//                 rotateLeft(leftChild);
//                 rotateRight(n);

//                 int8_t bLR = lr->getBalance();
//                 if (bLR == -1) { n->setBalance(0); leftChild->setBalance(1); }
//                 else if (bLR == 1) { n->setBalance(-1); leftChild->setBalance(0); }
//                 else { n->setBalance(0); leftChild->setBalance(0); }
//                 lr->setBalance(0);

//                 n = lr->getParent();
//                 diff = (n && n->getParent() && n->getParent()->getLeft() == n ? +1 : -1);
//                 continue;
//             }
//         } else if (b == 2) {
//             AVLNode<Key, Value>* rightChild = n->getRight();
//             if (rightChild->getBalance() >= 0) {
//                 // Right-Right
//                 rotateLeft(n);
//                 if (rightChild->getBalance() == 0) {
//                     n->setBalance(1);
//                     rightChild->setBalance(-1);
//                     break;
//                 } else {
//                     n->setBalance(0);
//                     rightChild->setBalance(0);
//                     n = rightChild->getParent();
//                     diff = (n && n->getParent() && n->getParent()->getLeft() == n ? +1 : -1);
//                     continue;
//                 }
//             } else {
//                 // Right-Left
//                 AVLNode<Key, Value>* rl = rightChild->getLeft();
//                 rotateRight(rightChild);
//                 rotateLeft(n);

//                 int8_t bRL = rl->getBalance();
//                 if (bRL == 1) { n->setBalance(0); rightChild->setBalance(-1); }
//                 else if (bRL == -1) { n->setBalance(1); rightChild->setBalance(0); }
//                 else { n->setBalance(0); rightChild->setBalance(0); }
//                 rl->setBalance(0);

//                 n = rl->getParent();
//                 diff = (n && n->getParent() && n->getParent()->getLeft() == n ? +1 : -1);
//                 continue;
//             }
//         } else if (b == -1 || b == 1) {
//             // Height unchanged, stop propagation
//             break;
//         } else { // b == 0
//             // Height decreased, continue up
//             if (parent != nullptr)
//                 diff = (parent->getLeft() == n ? +1 : -1);
//             n = parent;
//         }
//     }
// #ifdef DEBUG
// std::cout << "end remove fix fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
// }


// // Remove
// template<class Key, class Value>
// void AVLTree<Key, Value>::remove(const Key& key) {

// #ifdef DEBUG
// std::cout << "start remove fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif


//     AVLNode<Key, Value>* z = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
//     if (!z) return;

//     AVLNode<Key, Value>* parent = z->getParent();
//     int8_t diff = 0;

//     if (z->getLeft() && z->getRight()) {
//         AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(z));
//         nodeSwap(z, pred);
//         parent = z->getParent();
//     }

//     AVLNode<Key, Value>* child = z->getLeft() ? z->getLeft() : z->getRight();
//     if (child) child->setParent(parent);

//     if (parent) diff = (parent->getLeft() == z ? +1 : -1);

//     if (!parent) this->root_ = child;
//     else if (parent->getLeft() == z) parent->setLeft(child);
//     else parent->setRight(child);

//     delete z;
//     removeFix(parent, diff);


//     #ifdef DEBUG
// std::cout << "end remove fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

// }


// template<class Key, class Value>
// void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
// {

// #ifdef DEBUG
// std::cout << "start node swap fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif

//     BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
//     int8_t tempB = n1->getBalance();
//     n1->setBalance(n2->getBalance());
//     n2->setBalance(tempB);

// #ifdef DEBUG
// std::cout << "end node swap fn - printing AVL in-order" << std::endl;
// this->debugPrint();
// #endif
// }


// #ifdef DEBUG
// template<class Key, class Value>
// void AVLTree<Key, Value>::debugPrint() const {
//     std::cout << "AVL In-order: ";
//     printInOrderHelper(this->root_);
//     std::cout << std::endl;
// }


// template<class Key, class Value>
// void AVLTree<Key, Value>::printInOrderHelper(Node<Key,Value>* node) const {
//     if (!node) return;

//     auto* avn = static_cast<AVLNode<Key,Value>*>(node);

//     printInOrderHelper(node->getLeft());
//     std::cout << "(" 
//               << node->getKey()
//               << ", b=" << int(avn->getBalance())
//               << ") ";
//     printInOrderHelper(node->getRight());
// }

// #endif


// #endif
