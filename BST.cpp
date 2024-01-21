//
// Created by ILHAN on 17.11.2023.
//
#include <iostream>
#include <string>
#include "BST.h"
using namespace std;

// ------------------------------- PUBLIC --------------------------------------
template <class Key, class Value>
BST<Key, Value>::BST() {
    root = nullptr;
}
// Taken from the lecture slides
template <class Key, class Value>
BST<Key, Value>::~BST()
{
    makeEmpty();
}
// Taken from the lecture slides
template <class Key, class Value>
const BSTNode<Key, Value> * BST<Key, Value>::find(const Key & x) const
{
    return elementAt(find(x, root));
}
// Taken from the lecture slides
template <class Key, class Value>
const Key & BST<Key, Value>::findMin() const
{
    return elementAt(findMin(root));
}
// Taken from the lecture slides
template <class Key, class Value>
const Key & BST<Key, Value>::findMax() const
{
    return elementAt(findMax(root));
}
// Taken from the lecture slides
template <class Key, class Value>
void BST<Key, Value>::makeEmpty()
{
    makeEmpty(root);
}
// Taken from the lecture slides
template <class Key, class Value>
void BST<Key, Value>::insert(const Key & x, const Value & v)
{
    insert(x, v, root);
}

template <typename Key, typename Value>
void BST<Key, Value>::remove(const Key & x, const Value & v) {
    remove(x, v, root);
}
// Taken from the lecture slides
template <class Key, class Value>
bool BST<Key, Value>::isEmpty() const {
    return (root == nullptr);
}

// ------------------------------- PRIVATE --------------------------------------

// Taken from the lecture slides
template <class Key, class Value>
const BSTNode<Key, Value> * BST<Key, Value>::elementAt(BSTNode<Key, Value> * t) const
{
    if (t == nullptr)
        return nullptr;
    else
        return t;
}
// Taken from the lecture slides
template <class Key, class Value>
BSTNode<Key, Value> * BST<Key, Value>::find(const Key & x, BSTNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    else if(x < t->key)
        return find(x, t->left);
    else if(t->key < x)
        return find(x, t->right);
    else
        return t; // Match
}
// Taken from the lecture slides
template <class Key, class Value>
BSTNode<Key, Value> * BST<Key, Value>::findMin(BSTNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    if(t->left == nullptr)
        return t;
    return findMin(t->left);
}
// Taken from the lecture slides
template <class Key, class Value>
BSTNode<Key, Value> * BST<Key, Value>::findMax(BSTNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    if(t->right == nullptr)
        return t;
    return findMin(t->right);
}
// Taken from the lecture slides
template <class Key, class Value>
void BST<Key, Value>::insert(const Key & x, const Value & v, BSTNode<Key, Value> *& t) const
{
    if( t == nullptr ) //  create a new node at the right place
        t = new BSTNode<Key, Value>(x, v, nullptr, nullptr);
    else if( x < t->key )
        insert( x, v, t->left );  // insert at the left or
    else if( t->key < x )
        insert( x, v, t->right );  // right subtree
    else
        ;  // Duplicate; do nothing

}

template <class Key, class Value>
void BST<Key, Value>::remove(const Key & x, const Value & v, BSTNode<Key, Value> *& t) {
    if( t == nullptr )
        return;   // Item not found; do nothing
    if( x < t->key )
        remove( x, v, t->left );
    else if( t->key < x )
        remove( x, v, t->right );
    else if( t->left != nullptr && t->right != nullptr ) // Two children
    {
        t->key = findMin( t->right )->key;
        remove( t->key, t->value, t->right );
    }
    else // one or no children
    {
        BSTNode<Key, Value> *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
    }

}

// Taken from the lecture slides
template <class Key, class Value>
void BST<Key, Value>::makeEmpty(BSTNode<Key, Value> *& t) const
{
    if(t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}