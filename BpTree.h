//
//CMPT454 Project
//Name: Keisuke Okamoto
//
//
//Compile with GCC under linux

#ifndef B_tree_BpTree_h
#define B_tree_BpTree_h

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;

class BpTree
{
    
public:
    BpTree(void);
    BpTree(const BpTree &);
    BpTree(int);
    ~BpTree(void);
    const BpTree& operator=(const BpTree&);
    
    string find(int) const;
    void printKeys() const;
    void printValues() const;
    void insert(int, string);
    void remove(int);
    
    class Node{
    public:
        bool isLeaf; //true if the node is leaf node
        int *key; //key
        string* data; //pointer to data (leaf only)
        Node** child; //pointer to child node (internal only)
        int nkey; //number of keys contained
        Node* next; // for pointer chain (leaf)  to store the new node temporaly (internal)
        Node* parent; //pointer to its parent
        Node(int);
        Node(bool,int*, string*, Node**, int, Node*, Node*);
        ~Node(void);
    };
    
    BpTree(Node*, int, int, int);
    
private:
    Node* root;
    int num; // num of keys each node
    int flo; //floor of num
    int cei; // ceil of num
    
    /*Helper functions*/
    void freeNode(); // free node
    
    /*For Both Insert and Delete*/
    bool search(int) const; //return true if the key is found in the tree
    Node* findNode(int) const; //find the node to insert the key
    int findSuc(Node*) const; //find its successor for the key
    int findLevel(Node*) const; //find the level where the node is at
    
    /*For Insert*/
    bool fixPointer(Node*); //fix up the pointers after node is splitted
    bool insertWithinNode(int, string, Node*); //With in a node, this finds the location to insert and insert key. return true if node is splitted
    void splitLeafNode(Node*, int); //split node when node is full *this does not fix the pointer
    void splitInternalNode(Node*, int); // split indternal node
    
    
    /*For Delete*/
    int whichParent(Node*) const; //find which child of its parent the node is.
    Node* findLeftSibling(Node*) const; //find its left sibling return null if its null
    Node* findRightSibling(Node*) const; //find its right sibling
    void takeKeyFromLeftLeaf(Node*, Node*); // Redistribute values from the left sibling(leaf)
    void takeKeyFromRightLeaf(Node*, Node*); // Redistribute values from the right sibling(leaf)
    void mergeWithRightLeaf(Node*, Node*); //Coalesce with the left sibling(leaf)
    void mergeWithLeftLeaf(Node*, Node*); // Coalesce with the right sibling(leaf)
    bool fixParent(Node*); //decide what to do when tree needs to be reorganized
    void takeKeyFromLeftInternal(Node*, Node*); //Redistribute values from the left sibling(internal)
    void takeKeyFromRightInternal(Node*, Node*); // Redistribute values from the right sibling(internal)
    void mergeWithRightInternal(Node*, Node*); //Coalesce with the left sibling(internal)
    void mergeWithLeftInternal(Node*, Node*); //// Coalesce with the right sibling(internal)
    void fixKey(Node*); //fix up the keys
};


#endif
