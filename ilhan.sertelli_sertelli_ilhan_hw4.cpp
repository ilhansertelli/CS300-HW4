#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include "BST.h"
#include "HashTable.h"
#include "strutils.h"
#include "Sort.h"
using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    DocumentItem(const string & name, const int & c): documentName(name), count(c) {}
};

struct WordItem {
    string word;
    BST<string, DocumentItem> docInfo; // Document info is stored in tree

    WordItem(const string & w) : word(w) {}

    bool operator<(const WordItem* rhs) const {
        return this->word < rhs->word;
    }

    bool operator>(const WordItem* rhs) const {
        return this->word > rhs->word;
    }
};


// Parsing the query into words and creating the list of the words
vector<string> parser(string query) {
    vector<string> words;
    int idx = 0;
    while (idx < query.length()) {
        string tempWord = "";
        if (('a' <= query.at(idx) && query.at(idx) <= 'z') || ('A' <= query.at(idx) && query.at(idx) <= 'Z')) {
            while (idx < query.length() && ('a' <= query.at(idx) && query.at(idx) <= 'z') || ('A' <= query.at(idx) && query.at(idx) <= 'Z')) {
                tempWord += query.at(idx);
                idx++;
                if (idx == query.length())
                    break;
            }
        }
        else
            idx++;
        if (tempWord != "") {
            ToLower(tempWord);
            words.push_back(tempWord);
        }
    }
    return words;
}

bool isIn(const vector<WordItem*> & checkVec, const string & word) {
    for (int i = 0; i < checkVec.size(); i++) {
        if (checkVec[i]->word == word)
            return true;
    }
    return false;
}

int findIdx(const vector<WordItem*> & checkVec, const string & word) {
    for (int i = 0; i < checkVec.size(); i++) {
        if (checkVec[i]->word == word)
            return i;
    }
    return -1;
}

int binarySearch(const vector<WordItem*> & sortedArray, const string & target) {
    int left = 0;
    int right = sortedArray.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (sortedArray[mid]->word == target ) {
            return mid; // Target found at index mid
        } else if (sortedArray[mid]->word < target) {
            left = mid + 1; // Discard the left half
        } else {
            right = mid - 1; // Discard the right half
        }
    }
    return -1; // Target not found
}

int main() {

    string notFound;
    int numFiles, size = 101;
    vector<string> fileList; // The list of input files
    cout << "Enter number of input files: ";
    cin >> numFiles;

    // Adding the files into the list
    for (int i = 1; i <= numFiles; i++) {
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;
        fileList.push_back(fileName);
    }

    HashTable<string, WordItem*> table(notFound, size);
    BST<string, WordItem*> searchTree;
    Sort<WordItem*> sort;
    Sort<string> sortStr;

    for (int j = 0; j < fileList.size(); j++) {
        ifstream file;
        string word;
        file.open(fileList[j].c_str());
        if (!file.fail()) {
            // Getting all the words one by one
            while (getline(file, word)) {
                ToLower(word);
                int idx = 0;
                for (int k = 0; k < word.length(); k++) {
                    if (!(('a' <= word.at(k) && word.at(k) <= 'z') || ('A' <= word.at(k) && word.at(k) <= 'Z'))) {
                        word.at(k) = ' ';
                        StripWhite(word);
                    }
                }
                while (idx < word.length()) {
                    string newWord = "";
                    if (word[idx] != ' ') {
                        while (word[idx] != ' ' && idx < word.length()) {
                            newWord += word[idx];
                            idx++;
                        }
                        // Word Extracted
                        WordItem * item = new WordItem(newWord);
                        vector<WordItem*> itemVec = sort.getQVec();
                        //----------------------------------------- VECTOR OPERATION ----------------------------------------------
                        if (!isIn(itemVec, newWord)) {
                            DocumentItem doc(fileList[j], 1);
                            item->word = newWord;
                            item->docInfo.insert(fileList[j], doc);
                            sort.addToVectors(item);
                            sortStr.addToVectors(newWord);
                        }
                        else {
                            if (isIn(itemVec, newWord)) {
                                if (sort.getQVec()[findIdx(itemVec, newWord)]->docInfo.find(fileList[j]) == nullptr) {
                                    DocumentItem doc(fileList[j], 1);
                                    itemVec[findIdx(itemVec, newWord)]->docInfo.insert(fileList[j], doc);
                                    sort.setQVec(itemVec);
                                    sort.setHVec(itemVec);
                                    sort.setMVec(itemVec);
                                    sort.setIVec(itemVec);
                                } else {
                                    // Storing the count of the appearance of the word in the file
                                    int newCount = itemVec[findIdx(itemVec, newWord)]->docInfo.find(
                                            fileList[j])->value.count;
                                    newCount++;
                                    // Removing the old count-version and adding the +1 version to the table (updating)
                                    itemVec[findIdx(itemVec, newWord)]->docInfo.remove(fileList[j], table.find(
                                            newWord).value->docInfo.find(fileList[j])->value);
                                    DocumentItem newDoc(fileList[j], newCount);
                                    itemVec[findIdx(itemVec, newWord)]->docInfo.insert(fileList[j], newDoc);
                                    sort.setQVec(itemVec);
                                    sort.setHVec(itemVec);
                                    sort.setMVec(itemVec);
                                    sort.setIVec(itemVec);
                                }
                            }
                        }
                        // Add the word to the tree if it does not exist
                        //----------------------------------------- BST OPERATION ----------------------------------------------
                        item = new WordItem(newWord);
                        if (searchTree.find(newWord) == nullptr) {
                            DocumentItem doc(fileList[j], 1);
                            item->word = newWord;
                            item->docInfo.insert(fileList[j], doc);
                            searchTree.insert(newWord, item);
                        }
                            // If the word exists
                        else {
                            if (searchTree.find(newWord)->key == newWord) {
                                // Check if the file is already stored in the word node
                                if (searchTree.find(newWord)->value->docInfo.find(fileList[j]) == nullptr) {
                                    DocumentItem doc(fileList[j], 1);
                                    searchTree.find(newWord)->value->docInfo.insert(fileList[j], doc);
                                }
                                    // If the file is already in the word node
                                else {
                                    // Storing the count of the appearance of the word in the file
                                    int newCount = searchTree.find(newWord)->value->docInfo.find(fileList[j])->value.count;
                                    newCount++;
                                    // Removing the old count-version and adding the +1 version to the tree (updating)
                                    searchTree.find(newWord)->value->docInfo.remove(fileList[j], searchTree.find(newWord)->value->docInfo.find(fileList[j])->value);
                                    DocumentItem newDoc(fileList[j], newCount);
                                    searchTree.find(newWord)->value->docInfo.insert(fileList[j], newDoc);
                                }
                            }
                        }
                        //----------------------------------------- HASH OPERATION ----------------------------------------------

                        WordItem * item1 = new WordItem(newWord);
                        if (table.find(newWord).key == notFound) {
                            DocumentItem doc(fileList[j], 1);
                            item1->word = newWord;
                            item1->docInfo.insert(fileList[j], doc);
                            table.insert(newWord, item1);
                        }
                        else {
                            if (table.find(newWord).key == newWord) {
                                // Check if the file is already stored in the word node
                                if (table.find(newWord).value->docInfo.find(fileList[j]) == nullptr) {
                                    DocumentItem doc(fileList[j], 1);
                                    table.find(newWord).value->docInfo.insert(fileList[j], doc);
                                }
                                else {
                                    // Storing the count of the appearance of the word in the file
                                    int newCount = table.find(newWord).value->docInfo.find(fileList[j])->value.count;
                                    newCount++;
                                    // Removing the old count-version and adding the +1 version to the table (updating)
                                    table.find(newWord).value->docInfo.remove(fileList[j], table.find(newWord).value->docInfo.find(fileList[j])->value);
                                    DocumentItem newDoc(fileList[j], newCount);
                                    table.find(newWord).value->docInfo.insert(fileList[j], newDoc);
                                }
                            }
                        }
                    }
                    else
                        idx++;
                }
            }
        }
    }

    cout << endl << endl << "After preprocessing, the unique word count is " << table.getTableSize() << ". Current load ratio is " << table.getLoadFactor() << endl << endl;

    Sort<WordItem*> temp;

    auto startQuickSortFirst = std::chrono::high_resolution_clock::now();
    sortStr.quickSortFirstPivot();
    auto quickSortTimeFirst = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startQuickSortFirst);

    //cout << "Quicksort Time: " << quickSortTimeFirst.count() << " ns\n";

    auto startQuickSortRandom = std::chrono::high_resolution_clock::now();
    sortStr.quickSortRandomPivot();
    auto quickSortTimeRandom = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startQuickSortRandom);

    //cout << "Quicksort Time: " << quickSortTimeRandom.count() << " ns\n";

    auto startQuickSortMedian = std::chrono::high_resolution_clock::now();
    sortStr.quickSortMedian();
    auto quickSortTimeMedian = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startQuickSortMedian);

    //cout << "Quicksort Time: " << quickSortTimeMedian.count() << " ns\n";

    // Applying heapsort to heapSortVec
    auto startHeapSort = std::chrono::high_resolution_clock::now();
    sortStr.heapSort();
    auto heapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startHeapSort);

    //cout << "Heapsort Time: " << heapSortTime.count() << " ns\n";

    // Applying mergesort to mergeSortVec
    auto startMergeSort = std::chrono::high_resolution_clock::now();
    sortStr.mergeSort();
    auto mergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startMergeSort);

    //cout << "Mergesort Time: " << mergeSortTime.count() << " ns\n";

    // Applying insertion sort to insertSortVec
    auto startInsertionSort = std::chrono::high_resolution_clock::now();
    sortStr.insertionSort();
    auto insertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - startInsertionSort);

    //cout << "Insertion Sort Time: " << insertionSortTime.count() << " ns\n";

    for (string elem : sortStr.getQVec()) {
        temp.addToVectors(sort.getQVec()[findIdx(sort.getQVec(), elem)]);
    }

    string query;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, query);
    vector<string> parsedQuery = parser(query);

    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        if (i == 0) {
            // QueryDocuments(with BST);
            bool none = true; // True -> The query does not exist in none of the files
            for (int i = 0; i < fileList.size(); i++) {
                bool inEach = true; // True -> The query words exist in the file
                for (int j = 0; j < parsedQuery.size(); j++) {
                    // If the word already exists in the tree
                    if (searchTree.find(parsedQuery[j]) != nullptr) {
                        // Checking if the file is stored in the node
                        if (searchTree.find(parsedQuery[j])->value->docInfo.find(fileList[i]) == nullptr)
                            inEach = false;
                    } else
                        inEach = false;
                }
                if (inEach) { // It means that every word in the query exists in at least one file
                    cout << "in Document " << fileList[i] << ", ";
                    for (int k = 0; k < parsedQuery.size(); k++) {
                        if (k != parsedQuery.size() - 1)
                            cout << parsedQuery[k] << " found "
                                 << searchTree.find(parsedQuery[k])->value->docInfo.find(fileList[i])->value.count
                                 << " times, ";
                        else
                            cout << parsedQuery[k] << " found "
                                 << searchTree.find(parsedQuery[k])->value->docInfo.find(fileList[i])->value.count
                                 << " times.";
                    }
                    cout << endl;
                    none = false;
                }
            }
            if (none) { // It means that none of the file contains every word in the query
                cout << "No document contains the given query" << endl;
            }
        }
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with hashtable);
        if (i == 0) {
            bool none = true; // True -> The query does not exist in none of the files
            for (int i = 0; i < fileList.size(); i++) {
                bool inEach = true; // True -> The query words exist in the file
                for (int j = 0; j < parsedQuery.size(); j++) {
                    // If the word already exists in the table
                    if (table.find(parsedQuery[j]).key != notFound) {
                        // Checking if the file is stored in the node
                        if (table.find(parsedQuery[j]).value->docInfo.find(fileList[i]) == nullptr)
                            inEach = false;
                    } else
                        inEach = false;
                }
                if (inEach) { // It means that every word in the query exists in at least one file
                    cout << "in Document " << fileList[i] << ", ";
                    for (int k = 0; k < parsedQuery.size(); k++) {
                        if (k != parsedQuery.size() - 1)
                            cout << parsedQuery[k] << " found "
                                 << table.find(parsedQuery[k]).value->docInfo.find(fileList[i])->value.count
                                 << " times, ";
                        else
                            cout << parsedQuery[k] << " found "
                                 << table.find(parsedQuery[k]).value->docInfo.find(fileList[i])->value.count
                                 << " times.";
                    }
                    cout << endl;
                    none = false;
                }
            }
            if (none) { // It means that none of the file contains every word in the query
                cout << "No document contains the given query" << endl;
            }
        }
    }

    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with vector);
        if (i == 0) {
            bool none = true; // True -> The query does not exist in none of the files
            for (int i = 0; i < fileList.size(); i++) {
                bool inEach = true; // True -> The query words exist in the file
                for (int j = 0; j < parsedQuery.size(); j++) {
                    // Using binary search to check if the word exists in the sorted list
                    if (binarySearch(temp.getQVec(), parsedQuery[j]) != -1) {
                        if (temp.getQVec()[findIdx(temp.getQVec(), parsedQuery[j])]->docInfo.find(fileList[i]) == nullptr)
                            inEach = false;
                    }
                    else
                        inEach = false;
                }
                if (inEach) { // It means that every word in the query exists in at least one file
                    cout << "in Document " << fileList[i] << ", ";
                    for (int k = 0; k < parsedQuery.size(); k++) {
                        if (k != parsedQuery.size() - 1)
                            cout << parsedQuery[k] << " found "
                                 << temp.getQVec()[findIdx(temp.getQVec(), parsedQuery[k])]->docInfo.find(fileList[i])->value.count
                                 << " times, ";
                        else
                            cout << parsedQuery[k] << " found "
                                 << temp.getQVec()[findIdx(temp.getQVec(), parsedQuery[k])]->docInfo.find(fileList[i])->value.count
                                 << " times.";
                    }
                    cout << endl;
                    none = false;
                }
            }
            if (none) { // It means that none of the file contains every word in the query
                cout << "No document contains the given query" << endl;
            }
        }
    }
    auto BinarySearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    cout << "\nBinary Search Tree Time: " << BSTTime.count() / k << "\n";
    cout << "Hash Table Time: " << HTTime.count() / k << "\n";
    cout << "Binary Search Time: " << BinarySearchTime.count() / k << "\n";

    cout << "\nQuick Sort(median) Time: " << quickSortTimeMedian.count() / k << "\n";
    cout << "Quick Sort(random) Time: " << quickSortTimeRandom.count() / k << "\n";
    cout << "Quick Sort(first) Time: " << quickSortTimeFirst.count() / k << "\n";
    cout << "Merge Sort Time: " << mergeSortTime.count() / k << "\n";
    cout << "Heap Sort Time: " << heapSortTime.count() / k << "\n";
    cout << "Insertion Sort Time: " << insertionSortTime.count() / k << "\n";

    cout << "\nSpeed Up BST/HST: " << double(BSTTime.count() / k) / double(HTTime.count() / k);
    cout << "\nSpeed Up Merge Sort/Quick Sort(Median): " << double(mergeSortTime.count() / k) / double(quickSortTimeMedian.count() / k);
    cout << "\nSpeed Up Heap Sort/Quick Sort(Median): " << double(heapSortTime.count() / k) / double(quickSortTimeMedian.count() / k);
    cout << "\nSpeed Up Insertion Sort/Quick Sort(Median): " << double(insertionSortTime.count() / k) / double(quickSortTimeMedian.count() / k) << endl;

    cout << "\nSpeed Up Binary Search / Binary Search Tree Time: " << double(BinarySearchTime.count() / k) / double(BSTTime.count() / k);
    cout << "\nSpeed Up Binary Search / Hash Table Time: " << double(BinarySearchTime.count() / k) / double(HTTime.count() / k);


    return 0;
}
