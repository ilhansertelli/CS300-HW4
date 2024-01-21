//
// Created by ILHAN on 12.12.2023.
//

#ifndef CS300_THE3_BST_H
#define CS300_THE3_BST_H

#include <string>
#include <vector>
using namespace std;

template <class Key, class Value>
struct BSTNode {
    Key key;
    Value value;
    BSTNode * left;
    BSTNode * right;

    BSTNode(const Key & k, const Value & v,  BSTNode * l = nullptr, BSTNode * r = nullptr):
            key(k), value(v), left(l), right(r) {}
};

template <class Key, class Value>
class BST
{
public:
    BST();
    ~BST();

    const Key & findMin() const;
    const Key & findMax() const;
    const BSTNode<Key, Value> * find(const Key & x) const;
    bool isEmpty() const;

    void makeEmpty();
    void insert(const Key & x, const Value & v);
    void remove(const Key & x, const Value & v);

private:
    BSTNode<Key, Value> * root;
    //Key ITEM_NOT_FOUND;

    const BSTNode<Key, Value> * elementAt(BSTNode<Key, Value> * t) const;
    void insert(const Key & x, const Value & v, BSTNode<Key, Value> *& t) const;
    void remove(const Key & x, const Value & v, BSTNode<Key, Value> *& t);

    BSTNode<Key, Value> * findMin(BSTNode<Key, Value> * t) const;
    BSTNode<Key, Value> * findMax(BSTNode<Key, Value> * t) const;
    BSTNode<Key, Value> * find(const Key & x, BSTNode<Key, Value> * t) const;

    void makeEmpty(BSTNode<Key, Value> *& t) const;

};

#include "BST.cpp"
#endif //CS300_THE3_BST_H
