//
// Created by ILHAN on 13.01.2024.
//

#ifndef CS300_THE4_SORT_H
#define CS300_THE4_SORT_H


//
// Created by ILHAN on 13.01.2024.
//
#include <vector>
#include <iostream>
#include <string>
using namespace std;

template <class Comparable>
class Sort {

public:
    Sort();

    void addToVectors(Comparable & element);
    void insertionSort ();
    void insertionSort( vector<Comparable> & a, int left, int right );
    inline void swap(Comparable & i, Comparable & j);
    void percDown(vector<Comparable> & a, int i, int n);
    void heapSort();
    void merge(vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd);
    void mergeSort(vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right);
    void mergeSort();
    inline int leftChild(int i);
    const Comparable & median3(vector<Comparable> & a, int left, int right);
    void quickSortMedian(vector<Comparable> & a, int left, int right);
    void quickSortFirstPivot(vector<Comparable> a, vector<Comparable> & tmpArray, int left, int right);
    void quickSortRandomPivot(vector<Comparable> a, vector<Comparable> & tmpArray, int left, int right);
    void quickSortMedian();
    void quickSortRandomPivot();
    void quickSortFirstPivot();

    void setQVec(const vector<Comparable> & a);
    void setHVec(const vector<Comparable> & a);
    void setMVec(const vector<Comparable> & a);
    void setIVec(const vector<Comparable> & a);

    vector<Comparable> getQVec();
    vector<Comparable> getHVec();
    vector<Comparable> getMVec();
    vector<Comparable> getIVec();

private:
    vector<Comparable> quickSortVec;
    vector<Comparable> heapSortVec;
    vector<Comparable> mergeSortVec;
    vector<Comparable> insertionSortVec;
};


#include "Sort.cpp"
#endif //CS300_THE4_SORT_H
