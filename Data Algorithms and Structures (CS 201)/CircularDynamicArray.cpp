#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

template <class N>
class CircularDynamicArray {
private:
    int MAX;
    int size;
    int front;
    N dummyElement;
    N *array;

    //merges between the regular arrays and the circular array
    void mergeIntoParent(N a[], N a1[], N a2[], int n) { 
        //n1 = midway point, n2 = back
        int n1 = n/2, n2 = n-n/2, i = 0; 
        int p1 = 0, p2 = 0;

        //compares sub-arrays and inserts correct value into parent array
        while (p1 < n1 && p2 < n2) {
            int index = (front + i) % MAX;
            a[index] = a1[p1]<a2[p2] ? a1[p1++] : a2[p2++];
            i++;
        }

        //slides up any leftover numbers ie if this array ends up still full after the first while loop
        while (p1 < n1) {
            int index = (front + i) % MAX;
            a[index] = a1[p1++];
            i++;
        } 

        //slides up any leftover numbers ie if this array ends up still full after the first while loop
        while (p2 < n2) {
            int index = (front + i) % MAX;
            a[index] = a2[p2++];
            i++;
        }
    }

    //recursive helper function for regular arrays
    void merge(N a[], N a1[], N a2[], int n) { 
        int n1 = n/2, n2 = n-n/2, i = 0;
        int p1 = 0, p2 = 0;
        while (p1 < n1 && p2 < n2) 
            a[i++] = a1[p1]<a2[p2] ? a1[p1++] : a2[p2++];
        while (p1 < n1) a[i++] = a1[p1++];
        while (p2 < n2) a[i++] = a2[p2++];
    }

    //recursive mergeSort call for regular arrays
    void mergeSort(N a[], int n) { 
        if (n <= 1) return;
        N a1[n / 2];
        N a2[n - (n / 2)];
        for (int i = 0; i < n / 2; i++)
            a1[i] = a[i];
        for (int i = n / 2; i<n; i++)
            a2[i-n/2] = a[i];
        mergeSort(a1, n/2);
        mergeSort(a2, n-n/2);
        merge(a, a1, a2, n);
    }

    N quickSelect(vector<N> A, int k) {
        N pivot = A.at(rand() % A.size()); //generates a random number between 0 and the size of the array
        vector<N> L, E, G;
        for (int i = 0; i < A.size(); i++) {
            N element = A.at(i);
            if (element < pivot) L.push_back(element);
            else if (element == pivot) E.push_back(element);
            else G.push_back(element);
        }
        if (k <= L.size()) return quickSelect(L, k);
        else if (k <= L.size() + E.size()) return pivot;
        else return quickSelect(G, k - L.size() - E.size());
    }

    N select(vector<N> A, int k) {
        N pivot = medianOfMedians(A);
        vector<N> L, E, G;
        for (int i = 0; i < A.size(); i++) {
            N element = A.at(i);
            if (element < pivot) L.push_back(element);
            else if (element == pivot) E.push_back(element);
            else G.push_back(element);
        }
        if (k <= L.size()) return select(L, k);
        else if (k <= L.size() + E.size()) return pivot;
        else return select(G, k - L.size() - E.size());
    }

    N medianOfMedians (vector<N> A) {
        vector<int> partitions;
        for (int i = 0; i < A.size(); i += 5) {
            partitions.push_back(i);
        }

        vector<N> medians;
        int i;
        for (i = 0; i < partitions.size(); i++) {
            int low = partitions.at(i);
            int high = (i + 1) * 5;
            if (A.size() % 5 != 0 && i == partitions.size() - 1) break;
            InsertionSort2(A, low, high);
            
            //select median of each group
            medians.push_back(A.at( ((high - 1) + low) / 2) );
        }

        if (A.size() % 5 != 0) {
            int low = partitions.at(i);
            int high = A.size();
            InsertionSort2(A, low, high);
            
            if (low == high) {
                medians.push_back(A.at(low));
            }
            else {
                medians.push_back(A.at( ((high - 1) + low) / 2 ));
            }
        }

        if (medians.size() == 1) return medians.at(0);
        else return medianOfMedians(medians);
    }

    //version of InsertionSort for the base array
    void InsertionSort(N arr[], int l, int h) {  
        //returns if both l and h are the same, which only happens when calling insertion sort on an array of size 1
        if (l == h) return;

        //outer loop starts at 1, assumes index 0 is already sorted
        for (int i = l + 1; i < h; i++){ 
            int j = i;

            //starts at i and goes backwards, performing a swap if need be until the element gets to the right spot
            while (j > l && arr[(front + j - 1) % MAX] > arr[(front + j) % MAX]) { 
                swap(arr[(front + j - 1) % MAX], arr[(front + j) % MAX]);
                j--;
            }
        } 
    }

    //version of Insertion sort for the vectors
    void InsertionSort2(vector<N> &vect, int l, int h) { 
        if (l == h) return;
        for (int i = l + 1; i < h; i++){ 
            int j = i;
            while (j > l && vect.at(j - 1) > vect.at(j)) {
                swap(vect.at(j - 1), vect.at(j));
                j--;
            }
        } 
    }

    //recursive call of binary search
    int binRecur(N a[], N x, int l, int r) {
        //returns -1 if not found
        if (r < l) return -1; 

        int mid = (l + r) / 2;
        int index = (front + mid) % MAX;
        if (x == a[index]) { //if x is found
            return mid;
        }
        else if (x < a[index]) { //if x is less that the value at the current index, recurse left
            return binRecur(a, x, l, mid - 1);
        }
        else { //if x is > the value at the current index, recurse right
            return binRecur(a, x, mid + 1, r);
        }
    }

    //copies the array from front to back
    void deepCopy(const CircularDynamicArray &src) { 
        array = new N[src.MAX];
        for(int i = src.front; i < src.front + src.size; i++) {
            array[i % src.MAX] = src.array[i % src.MAX];
        }

        //updating CDA attributes
        size = src.size;
        front = src.front;
        MAX = src.MAX;
    }
public:
    //default constructor, makes an array of MAX 2 and size 0
    CircularDynamicArray() { 
        MAX = 2;
        size = 0;
        array = new N[MAX];
        front = 0;
    }

    //constructor, array of MAX and size s
    CircularDynamicArray(int s) { 
        MAX = s;
        size = s;
        array = new N[MAX];
        front = 0;
    } 
    ~CircularDynamicArray() { //destructor
        delete[] array;
    }
    CircularDynamicArray(const CircularDynamicArray &src) { //copy constructor
        deepCopy(src);
    } 
    CircularDynamicArray &operator=(const CircularDynamicArray &src) { //copy assignment operator
        if (this != &src) {
            delete[] array;
            deepCopy(src);
        }
        return *this;
    }

    //returns the value from the array relative to the front
    N &operator[](int index) { 
        if (index < 0 || index >= MAX) return dummyElement;
        return array[(front + index) % MAX];
    }

    void addEnd(N v) {
        if (size == 0) { //empty array
            array[0] = v;
            size++;
        }
        else if (size == MAX) { //full array, resizes to a new array double the capacity
            N *newArray = new N[MAX * 2];
            for (int i = 0; i < size; i++) { //starts copying at the front of the old array
                newArray[i] = array[(front + i) % MAX];
            }
            delete[] array;

            //updating the attributes and pointing the array to the newArray
            MAX *= 2;
            front = 0;
            array = newArray;

            //inserting v into the back of the array
            array[front + size] = v;
            size++;
        }
        else { //array is neither full nor empty
            array[(front + size) % MAX] = v;
            size++;
        }
    }

    void addFront(N v) {
        if (size == 0) { //empty array
            array[0] = v;
            size++;
        }
        else if (size == MAX) { //array is full, resizing to a new array double the size
            N *newArray = new N[MAX * 2];
            for (int i = 0; i < size; i++) { //copying starting at the front
                newArray[i] = array[(front + i) % MAX];
            }
            delete[] array;

            //updating attributes, front will be at the back of the array
            MAX *= 2;
            front = MAX - 1;
            array = newArray;

            //inserting v into the front of the array
            array[front] = v;
            size++;
        }
        else { //array is neither empty nor full
            front = (front - 1 + MAX) % MAX; //updating where the front pointer is
            array[front] = v;
            size++;
        }
    }

    void delEnd() {
        if (size == 0) return; //if the array is empty, there is nothing to delete
        else if (size == (MAX / 4)) { //if the array is less than 1/4th the capacity, resize to a smaller array
            size--;
            N *newArray = new N[MAX / 2];
            for (int i = 0; i < size; i++) { //copying elements
                newArray[i] = array[(front + i) % MAX];
            }
            delete[] array;

            //updating attributes
            MAX /= 2;
            front = 0;
            array = newArray;
        }
        else { //arrray is neither empty nor at 1/4th capacity
            size = (size - 1 + MAX) % MAX; //decrement the size pointer to "delete" from back
        }
    }

    void delFront() {
        if (size == 0) return; //if the array is empty, there is nothing to delete
        else if (size == (MAX / 4)) { //if the array is less than 1/4th the capacity, resize to a smaller array
            front = (front + 1) % MAX;
            N *newArray = new N[MAX / 2];
            for (int i = 0; i < size; i++) { //copying elements
                newArray[i] = array[(front + i) % MAX];
            }
            delete[] array;

            //updating attributes
            MAX /= 2;
            front = 0;
            array = newArray;
        }
        else { //arrray is neither empty nor at 1/4th capacity
            front = (front + 1) % MAX; //decrementing the front
            size--;
        }
    }

    int length() {
        return size;
    }
    int capacity() {
        return MAX;
    }

    //resets the array to be "empty"
    void clear() { 
        delete[] array;
        MAX = 2;
        size = 0;
        front = 0;
        array = new N[MAX];
    }

    N QuickSelect(int k) {
        //error checking for k values
        if (k < 1 || k > size) return -1; 

        //a random number between 0 and the size of the array
        int randIndex = rand() % size; 

        //accesses this pivot with an offset from the front
        N pivot = array[(front + randIndex) % MAX]; 
        vector<N> L, E, G;

        //start adding elements to the less than/equal to/greater than lists
        for (int i = front; i < front + size; i++) { 
            N element = array[i % MAX];
            if (element < pivot) L.push_back(element);
            else if (element == pivot) E.push_back(element);
            else G.push_back(element);
        }
        if (k <= L.size()) return quickSelect(L, k);
        else if (k <= L.size() + E.size()) return pivot;
        else return quickSelect(G, k - L.size() - E.size());
    } 

    //performs the select algorithm in worst case O(n) time using medians of medians
    N WCSelect(int k) { 
        if (k < 1 || k > size) return -1;

        //stores a collection of partitions, the size of this collection is used to determine when to stop 
        vector<int> partitions; 
        for (int i = 0; i < size; i += 5) {
            partitions.push_back(i);
        }

        vector<N> medians;

        //for each group of 5, run insertion sort on it 
        int i;
        for (i = 0; i < partitions.size(); i++) {
            int low = partitions.at(i);
            int high = (i + 1) * 5;
            if (size % 5 != 0 && i == partitions.size() - 1) break;
            InsertionSort(array, low, high);
            
            //select median of each group
            medians.push_back(array[(( (front + high - 1) + (front + low) ) / 2) % MAX]);
        }
        
        //if the size is not divisible by 5, one more insertionSort call needs to be performed on the last bit of the array
        if (size % 5 != 0) { 
            int low = partitions.at(i);
            int high = size;
            InsertionSort(array, low, high);
            
            if (low == high) {
                medians.push_back(array[(front + low) % MAX]);
            }
            else { //selects median
                medians.push_back(array[(( (front + high - 1) + (front + low) ) / 2) % MAX]);
            }
        }

        N pivot = medianOfMedians(medians);
        vector<N> L, E, G;
        for (int i = front; i < front + size; i++) {
            N element = array[i % MAX];
            if (element < pivot) L.push_back(element);
            else if (element == pivot) E.push_back(element);
            else G.push_back(element);
        }
        if (k <= L.size()) return select(L, k);
        else if (k <= L.size() + E.size()) return pivot;
        else return select(G, k - L.size() - E.size());
    } 
    
    //contains first call of mergeSort, serves as a bridge between circular array and regular arrays
    void stableSort() { 
        N a1[size / 2];
        N a2[size - (size / 2)];
        for (int i = 0; i < size / 2; i++)
            a1[i] = array[(front + i) % MAX];
        for (int i = size / 2; i < size; i++)
            a2[i-size/2] = array[(front + i) % MAX];
        mergeSort(a1, size/2);
        mergeSort(a2, size-size/2);
        mergeIntoParent(array, a1, a2, size); 
    }

    //performs a linear search starting at the front; returns the index
    int linearSearch(N e) { 
        //used to track index in reference to the distance from front
        int userIndex = 0; 
        for(int i = front; i < front + size; i++) {
            if (array[i % MAX] == e) return userIndex;
            userIndex++;
        }
        return -1;
    } 
    int binSearch(N e) {
        return binRecur(array, e, 0, size - 1);
    }      
};