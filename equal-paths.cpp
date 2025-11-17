#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
#include <cstdlib>
#include <algorithm>

using namespace std;


// You may add any prototypes of helper functions here
//-1 means unbalanced 
//0 means height = 0
//pos integers is the height 
int height (Node* root) {
  //if we are at bottom return 0
  if (root == nullptr) return 0; 

  int left = height (root->left);
  int right = height (root->right);

  //if either branch is unbalanced ripple up
  if (left==-1 || right==-1) return -1;

  //if both children exist & unequal height ripple up  
  if ((root->left!=nullptr) && (root->right !=nullptr) && left!=right) return -1;

  //at this point the tree is balanced on both branches 
  else {
     return 1 + max((height (root->left)) , (height(root->right)));
  }
}


bool equalPaths(Node * root)
{
    // Add your code below
    //check if tree height is same for all leaves 
    //so is the tree full 

  //use height function to see if tree is same height on all paths 
  int path = height(root); 
  if (path==-1) return false;
  else return true; 

}