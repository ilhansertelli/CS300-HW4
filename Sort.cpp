//
// Created by ILHAN on 13.01.2024.
//
#include <vector>
#include <iostream>
#include <string>
#include "Sort.h"
using namespace std;

template <class Comparable>
Sort<Comparable>::Sort() {
    vector<Comparable> a;
    quickSortVec = a;
    heapSortVec = a;
    insertionSortVec = a;
    mergeSortVec = a;
}

template <class Comparable>
vector<Comparable> Sort<Comparable>::getQVec() {
    return quickSortVec;
}

template <class Comparable>
vector<Comparable> Sort<Comparable>::getHVec() {
    return heapSortVec;
}

template <class Comparable>
vector<Comparable> Sort<Comparable>::getMVec() {
    return mergeSortVec;
}

template <class Comparable>
vector<Comparable> Sort<Comparable>::getIVec() {
    return insertionSortVec;
}

template <class Comparable>
void Sort<Comparable>::setQVec(const vector<Comparable> & a) {
    quickSortVec = a;
}

template <class Comparable>
void Sort<Comparable>::setHVec(const vector<Comparable> & a) {
    heapSortVec = a;
}

template <class Comparable>
void Sort<Comparable>::setMVec(const vector<Comparable> & a) {
    mergeSortVec = a;
}

template <class Comparable>
void Sort<Comparable>::setIVec(const vector<Comparable> & a) {
    insertionSortVec = a;
}

template <class Comparable>
void Sort<Comparable>::addToVectors(Comparable & element) {
    quickSortVec.push_back(element);
    mergeSortVec.push_back(element);
    insertionSortVec.push_back(element);
    heapSortVec.push_back(element);
}
// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::insertionSort () {
    int j;
    for (int p = 1;  p < insertionSortVec.size(); p++) {
        Comparable tmp = insertionSortVec[p];
        for (j = p; j > 0 &&  tmp < insertionSortVec[j - 1]; j--)
        insertionSortVec[j] = insertionSortVec[j - 1];
        insertionSortVec[j] = tmp;
    }
}

// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::insertionSort(vector<Comparable> & a, int left, int right) {
    for (int p = left + 1; p <= right; p++) {
        Comparable tmp = a[p];
        int j;

        for (j = p; j > left && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

// Taken from the lecture slides
template <class Comparable>
inline int Sort<Comparable>::leftChild(int i)
{
    return 2*i+1;
}

// Taken from the lecture slides
template <class Comparable>
inline void Sort<Comparable>::swap(Comparable & obj1, Comparable & obj2) {
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::percDown(vector<Comparable> & a, int i, int n) {
    int child;
    Comparable tmp;

    for (tmp = a[i]; leftChild(i) < n; i = child) {
        child = leftChild(i);
        if (child + 1 < n && a[child] < a[child + 1]) {
            child++;
        }
        if (a[child] > tmp) {
            a[i] = a[child];
        } else {
            break;
        }
    }
    a[i] = tmp;
}

// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::heapSort() {
    // buildHeap
    for (int i = heapSortVec.size() / 2 - 1; i >= 0; i--) {
        percDown(heapSortVec, i, heapSortVec.size());
    }

    // sort
    for (int j = heapSortVec.size() - 1; j > 0; j--) {
        swap(heapSortVec[0], heapSortVec[j]);    // swap max to the last pos.
        percDown(heapSortVec, 0, j); // re-form the heap
    }
}

// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::merge(vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd) {
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;
    // Main loop
    while (leftPos <= leftEnd && rightPos <= rightEnd) {
        if (a[leftPos] <= a[rightPos])
            tmpArray[tmpPos++] = a[leftPos++];
        else
            tmpArray[tmpPos++] = a[rightPos++];
    }
    while (leftPos <= leftEnd)    // Copy rest of first half
        tmpArray[tmpPos++] = a[leftPos++];

    while ( rightPos <= rightEnd )  // Copy rest of right half
        tmpArray[tmpPos++] = a[rightPos++];

    // Copy tmpArray back
    for (int i = 0; i < numElements; i++, rightEnd--)
        a[rightEnd] = tmpArray[rightEnd];
}

// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::mergeSort(vector<Comparable> & a,
               vector<Comparable> & tmpArray, int left, int right) {
    if (left < right)
    {
        int center = (left + right) / 2;
        mergeSort(a, tmpArray, left, center);
        mergeSort(a, tmpArray, center + 1, right);
        merge(a, tmpArray, left, center + 1, right);
    }
}
// Taken from the lecture slides
template <class Comparable>
void Sort<Comparable>::mergeSort() {
    vector<Comparable> tmpArray(mergeSortVec.size());
    mergeSort(mergeSortVec, tmpArray, 0, mergeSortVec.size() - 1);
}
// Taken from the lecture slides
template <class Comparable>
const Comparable & Sort<Comparable>::median3(vector<Comparable> & a, int left, int right)
{
    int center = (left + right) / 2;
    if (a[center] < a[left])
        swap(a[left], a[center]);
    if (a[right] < a[left ])
        swap(a[left], a[right]);
    if (a[right] < a[center])
        swap(a[center], a[right]);

    // Place pivot at position right - 1
    swap(a[center], a[right - 1]);
    return a[right - 1];
}

template <class Comparable>
void Sort<Comparable>::quickSortMedian(vector<Comparable> & a, int left, int right) {
    if (left + 10 <= right) {
        Comparable pivot = median3(a, left, right);
        int i = left, j = right - 1;
        for ( ; ; ) {
            while (a[++i] < pivot) {}

            while (pivot < a[--j]) {}

            if (i < j)
                swap(a[i], a[j]);
            else
                break;
        }
        swap(a[i], a[right - 1]);   // Restore pivot
        quickSortMedian(a, left, i - 1);       // Sort small elements
        quickSortMedian(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertionSort(a, left, right);
}

template <class Comparable>
void Sort<Comparable>::quickSortFirstPivot(vector<Comparable> a, vector<Comparable> & tmpArray, int left, int right) {
    if (left + 10 <= right) {
        Comparable pivot = a[left];
        int i = left + 1, j = right;

        while (i <= j) {
            while (i <= j && a[i] <= pivot) {
                i++;
            }

            while (i <= j && a[j] > pivot) {
                j--;
            }

            if (i < j) {
                swap(a[i], a[j]);
            }
        }

        swap(a[left], a[j]);
        quickSortFirstPivot(a, tmpArray, left, j - 1);
        quickSortFirstPivot(a, tmpArray, j + 1, right);
    } else {
        insertionSort(a, left, right);
    }
}

template <class Comparable>
void Sort<Comparable>::quickSortRandomPivot(vector<Comparable> a, vector<Comparable> & tmpArray, int left, int right) {
    if (left + 10 <= right) {
        // Choose a random pivot within the range [left, right]
        int pivotIndex = rand() % (right - left + 1) + left;
        swap(a[pivotIndex], a[right]);

        Comparable pivot = a[right];
        int i = left, j = right - 1;

        while (i <= j) {
            while (i <= j && a[i] <= pivot) {
                i++;
            }

            while (i <= j && a[j] > pivot) {
                j--;
            }

            if (i < j) {
                swap(a[i], a[j]);
            }
        }

        swap(a[i], a[right]);
        quickSortRandomPivot(a, tmpArray, left, i - 1);
        quickSortRandomPivot(a, tmpArray, i + 1, right);
    } else {
        insertionSort(a, left, right);
    }
}


template <class Comparable>
void Sort<Comparable>::quickSortMedian() {
    quickSortMedian(quickSortVec, 0, quickSortVec.size() - 1);
}

template <class Comparable>
void Sort<Comparable>::quickSortFirstPivot() {
    vector<Comparable> tmpArray(quickSortVec.size());
    quickSortFirstPivot(quickSortVec, tmpArray, 0, quickSortVec.size() - 1);
}

template <class Comparable>
void Sort<Comparable>::quickSortRandomPivot() {
    vector<Comparable> tmpArray(quickSortVec.size());
    quickSortRandomPivot(quickSortVec, tmpArray, 0, quickSortVec.size() - 1);
}