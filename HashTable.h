//
// Created by ILHAN on 12.12.2023.
//
#ifndef CS300_THE3_HASHTABLE_H
#define CS300_THE3_HASHTABLE_H
#include <vector>
#include <string>
using namespace std;

enum EntryType {ACTIVE, EMPTY, DELETED};

template <class Key, class Value>
struct HashEntry
{
    Key key;
    Value value;
    EntryType info = EMPTY;

};

template <class Key, class Value>
class HashTable
{
public:
    HashTable(const Key & notFound, int size);
    HashTable(const HashTable & rhs)
            : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
              array(rhs.array), currentSize(rhs.currentSize) {}

    const HashEntry<Key, Value> find(const Key & x);
    int hashing(const Key & key, int tableSize);

    void makeEmpty();
    void insert(const Key & x, const Value & y);
    void remove(const Key & x);

    int getTableSize();
    double getLoadFactor();

private:
    vector<HashEntry<Key, Value>> array;
    int currentSize;
    Key ITEM_NOT_FOUND;

    bool isActive(int currentPos) const;
    int findPos(const Key & x);
    void rehash();
};

int hashing(const string & key, int tableSize);


#include "HashTable.cpp"
#endif //CS300_THE3_HASHTABLE_H

