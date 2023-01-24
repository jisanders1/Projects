#include <iostream>
//#include "CircularDynamicArray.cpp"
using namespace std;

template<typename key>
class Heap {
private:
    key dummyKey;
    CircularDynamicArray<key> heapArray;
    int parentIndex(int currentPosition) {
        return currentPosition / 2;
    }
    int leftChildIndex(int currentPosition) {
        return currentPosition * 2;
    }
    int rightChildIndex(int currentPosition) {
        return (currentPosition * 2) + 1;
    }
    void siftDown(CircularDynamicArray<key> &array, int currentIndex) { //performs a sift down in the event of a heap order violation
        int back = array.length() - 1;
        while ((leftChildIndex(currentIndex) <= back && heapArray[leftChildIndex(currentIndex)] < heapArray[currentIndex]) || (rightChildIndex(currentIndex) <= back && heapArray[rightChildIndex(currentIndex)] < heapArray[currentIndex])) {
            bool swapRight = false;
            if (rightChildIndex(currentIndex) <= back && heapArray[rightChildIndex(currentIndex)] < heapArray[leftChildIndex(currentIndex)]) {
                swapRight = true;
            }

            if (swapRight) {
                swap(heapArray[currentIndex], heapArray[rightChildIndex(currentIndex)]);
                currentIndex = rightChildIndex(currentIndex);
            }
            else {
                swap(heapArray[currentIndex], heapArray[leftChildIndex(currentIndex)]);
                currentIndex = leftChildIndex(currentIndex);
            }
        }
    }
public:
    Heap(){
        heapArray.addEnd(dummyKey);
    }
    Heap(key k[], int s){ //should make the array a heap by heapify
        heapArray.addEnd(dummyKey);
        for (int i = 0; i < s; i++) {
            heapArray.addEnd(k[i]);
        }
        int back = heapArray.length() - 1;
        for (int i = back; i >= 1; i--) {
            if (leftChildIndex(i) > back && rightChildIndex(i) > back) continue; //skips over leaf nodes
            else if (rightChildIndex(i) > back) {
                if(heapArray[i] > heapArray[leftChildIndex(i)]) {
                    swap(heapArray[i], heapArray[leftChildIndex(i)]);
                }
            }
            else {
                siftDown(heapArray, i);
            }
        }
    }
    key peekKey() {//returns the min of the heap without removing it
        return heapArray[1];
    } 
    key extractMin() {//returns the min of the heap and removes 
        if (heapArray.length() == 0) return dummyKey;

        key returnVal = heapArray[1];
        swap(heapArray[1], heapArray[heapArray.length() - 1]);
        heapArray.delEnd();

        int back = heapArray.length() - 1;
        int pos = 1;
        siftDown(heapArray, 1);
        return returnVal;
    }

    void insert(key k) {//inserts a key into the heap, sifting up if need be
        heapArray.addEnd(k);
        int pos = heapArray.length() - 1;
        while (pos > 1 && heapArray[pos] < heapArray[parentIndex(pos)]) { //sift up
            swap(heapArray[pos], heapArray[parentIndex(pos)]);
            pos /= 2;
        }
    }
    void printKey() {
        for (int i = 1; i < heapArray.length(); i++) {
            cout << heapArray[i] << " ";
        }
        cout << endl;
    }
};