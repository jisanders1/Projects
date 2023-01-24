#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

template <typename key>
class FibHeapNode {
public:
    key nodeKey;
    bool isMarked;
    int rank;
    FibHeapNode *parent;
    FibHeapNode *left;
    FibHeapNode *right;
    FibHeapNode *child;
    FibHeapNode() {
        rank = 0;
        isMarked = false;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        child = nullptr;
    }
    FibHeapNode(key newKey) {
        nodeKey = newKey;
        rank = 0;
        isMarked = false;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        child = nullptr;
    }
};

template <typename key>
class FibHeap {
private:
    int numNodes;
    FibHeapNode<key> *head;
    FibHeapNode<key> *tail;
    FibHeapNode<key> *min;
    void consolidate(){ //combinies the root list trees after extract min
        int degree;

        //calculating the upper limit on the numebr of trees in the root list
        float temp = (log(numNodes) / log(2));
        int upperBound = temp;
        FibHeapNode<key> *A[upperBound + 1]; //array used for consolidation
        
        //initializing all the pointers to nullptr
        for (int i = 0; i <= upperBound; i++) {
            A[i] = nullptr;
        }

        FibHeapNode<key> *node1 = head; //starts at head
        FibHeapNode<key> *node2;
        FibHeapNode<key> *node3;
        //FibHeapNode<key> *node4 = node1;

        do {
            degree = node1->rank;
            //node4 = node4->right;
            //cout << "Cur node is: " << node1->nodeKey << endl;

            //using the A array to check for numerous trees of a certain degree
            while(A[degree] != nullptr) {
                node2 = A[degree];
                if (node2 == node1) break;
                if (node1->nodeKey > node2->nodeKey) { //swap node1 and node2 if node1 is pointing to the largest value
                    node3 = node1;
                    node1 = node2;
                    node2 = node3;
                }
                if (node2 == head) {
                    head = node1;
                }
                
                link(node2, node1); //makes node2 a child of node1

                //updating A array to reflect the linking operation making a larger tree
                A[degree] = nullptr;
                degree++;
            }
            //Assigning A list with appropriate sized tree
            A[degree] = node1;
            node1 = node1->right;
        } while (node1 != head);

        min = nullptr;
        head = nullptr;
        tail = nullptr;
        //reinserting the trees back into the root list in the correct order
        for (int j = 0; j <= upperBound; j++) {
            if (A[j] != nullptr) {
                nodeInsert(A[j]);
            }
        }
    }
    void link(FibHeapNode<key> *node2, FibHeapNode<key> *node1) { //linking a parent's child list to the root list, node1 becomes the parent of node2
        //removing node2 from the root list
        (node2->left)->right = node2->right;
        (node2->right)->left = node2->left;

        //if node1 becomes the only member of the root list
        if (node1->right == node1) {
            min = node1;
        }

        //making node2 its own list and make node1 its parent
        node2->left = node2;
        node2->right = node2;
        node2->parent = node1;

        //if node1 does not have a child pointer, make its child pointer point to node2
        if (node1->child == nullptr) {
            node1->child = node2;
        }

        //inserting node2 into the child list
        node2->right = node1->child;
        node2->left = (node1->child)->left;
        ((node1->child)->left)->right = node2;
        (node1->child)->left = node2;

        node1->child = node2;
        node2->isMarked = false;
        node1->rank++;
    }
    void nodeInsert(FibHeapNode<key> *x) { //takes a node x and adds it to the end of the root list
        if (min == nullptr) { //if the heap is empty, make x the list
            min = x;
            head = x;
            tail = x;
            x->right = x;
            x->left = x;
        }
        else { //list is not empty, add to the back of the linked list
            tail->right = x;
            x->left = tail;
            tail = x;
            tail->right = head;
            head->left = tail;
            if(x->nodeKey < min->nodeKey) {
                min = x;
            }
        }
    }
    void cut(FibHeapNode<key> *x, FibHeapNode<key> *y) { //y is the parent, x is the child to be cut
        //if x is y's only child, mak ey's child null
        if (x == x->right) {
            y->child = nullptr;
        }

        //removing x from it's linked list
        (x->left)->right = x->right;
        (x->right)->left = x->left;

        //if y was pointing to x, make it point to x's right sibling
        if (x == y->child) {
            y->child = x->right;
        }

        //insert x into the root list and unmark it
        nodeInsert(x);
        x->isMarked = false;

        //decrement y's rank
        y->rank--;
    }
    void cascadingCut(FibHeapNode<key> *y) {//checks if y is marked, and cuts if y is maarked, also checked ancestors in the process
        FibHeapNode<key> *z = y->parent;
        if (z != nullptr) {
            if (!(y->isMarked)) {//if y wasn't marked previously, mark it
                y->isMarked = true;
            }
            else { //if y was marked
                cut(y, z); //cut it from its tree and add it to the end of the root list
                cascadingCut(z); //check its parent for mark violations
            }
        }
    }
    void recursivePrint(FibHeapNode<key> *curNode) { //recursive print function using preorder
        if (curNode == nullptr) return;
        do {
            cout << curNode->nodeKey << " ";
            if (curNode->child != nullptr) {
                recursivePrint(curNode->child);
            }
            curNode = curNode->right;
        } while (curNode != curNode->parent->child);
    }
    void heapDelete(FibHeapNode<key> *curNode) { //recursive delete using postorder
        if (curNode == nullptr) return;
        FibHeapNode<key> *nextNode = nullptr;
        int numNodes = curNode->parent->rank; //rank is used to know when to stop
        do {
            nextNode = curNode->right;
            if (curNode->child != nullptr) {
                heapDelete(curNode->child);
            }
            delete curNode;
            curNode = nextNode;
            numNodes--;
        } while (numNodes > 0);
    }
public:
    FibHeap() {
        numNodes = 0;
        head = nullptr;
        tail = nullptr;
        min = nullptr;
    }
    FibHeap(key k[], int s, CircularDynamicArray <FibHeapNode<key> *> &handle) { //makes a FibHeap with array k, single call of consolidate after all data has been inserted, pointers to nodes go in handle
        numNodes = 0;
        head = nullptr;
        tail = nullptr;
        min = nullptr;
        for(int i = 0; i < s; i++) {
            FibHeapNode<key> *newNode = insert(k[i]);
            handle.addEnd(newNode);
        }
        consolidate();
    }
    ~FibHeap() {
        if (min != nullptr) { //only actually delete anything if the heap is not empty
            FibHeapNode<key> *curNode = head;
            int numTrees = 0;

            //compute the number of trees in the heap in order to know when to stop
            do {
                numTrees++;
                curNode = curNode->right;
            } while (curNode != head);

            //resets curNode as the head
            curNode = head;
            FibHeapNode<key> *nextNode = nullptr; //keeps track of the next node after curNode is deleted
            do {
                nextNode = curNode->right;
                heapDelete(curNode->child);
                delete curNode;
                curNode = nextNode;
                numTrees--;
            } while (numTrees > 0);
        }
    }
    FibHeap(const FibHeap &src); //copy constructor
    FibHeap &operator=(const FibHeap &src) { //copy assignment operator
        return *this;
    }

    key peekKey(){ //returns minimum key without modifying heap
        return min->nodeKey;
    }
    FibHeapNode<key>* insert(key k) {
        FibHeapNode<key> *newNode = new FibHeapNode<key>(k);
        if (min == nullptr) {
            min = newNode;
            head = newNode;
            tail = newNode;
            newNode->right = newNode;
            newNode->left = newNode;
        }
        else {
            tail->right = newNode;
            newNode->left = tail;
            tail = newNode;
            newNode->right = head;
            head->left = newNode;
            if(newNode->nodeKey < min->nodeKey) {
                min = newNode;
            }
        }
        numNodes++;
        return newNode;
    }
    key extractMin(){
        key dummyNode;
        if (min == nullptr) return dummyNode;
        
        FibHeapNode<key> *nodeToRemove = min;
        FibHeapNode<key> *x = nullptr;
        FibHeapNode<key> *tempNode = nullptr;
        
        if (nodeToRemove->child != nullptr) { //insert nodeToRemoves children into the root list if they exist
            x = nodeToRemove->child;
            do {
                tempNode = x->right;
                nodeInsert(x);
                x->parent = nullptr;
                x = tempNode;
            } while (tempNode != nodeToRemove->child);
        }

        //removes node to remove from the root list
        (nodeToRemove->left)->right = nodeToRemove->right;
        (nodeToRemove->right)->left = nodeToRemove->left;

        //updates head pointer if nodeToRemove was the head
        if (nodeToRemove == head) {
            head = nodeToRemove->right;
        }

        if ((nodeToRemove == nodeToRemove->right) && (nodeToRemove->child == nullptr)) { //if nodeToRemove was the only node in the heap, clear the heap
            min = nullptr;
            head = nullptr;
            tail = nullptr;
        }
        else { //otherwise consolidate
            min = nodeToRemove->right;
            consolidate();
        }
        numNodes--;

        key returnKey = nodeToRemove->nodeKey;
        delete nodeToRemove;
        return returnKey;
    }
    
    bool decreaseKey(FibHeapNode<key> *h, key k) { //decreases h's key to k
        if (k >= h->nodeKey) return false;
        if (min == nullptr) return false;
        h->nodeKey = k;
        FibHeapNode<key> *i = h->parent;
        if (i != nullptr && h->nodeKey < i->nodeKey) { //checks if there is a heap order violation
            cut(h, i); //cuts h from its tree and puts it into the root list, updates marks
            cascadingCut(i);
        }
        if (h->nodeKey < min->nodeKey) { //updating min pointer
            min = h;
        }
        return true;
    }
    void merge(FibHeap<key> &H2) { //merges 2 heaps together, consuming H2 in the process
        if(min == nullptr) return;
        if (H2.min == nullptr) return;
        head->left = H2.tail;
        tail->right = H2.head;
        H2.head->left = tail;
        H2.tail->right = head;
        tail = H2.tail;
        
        if(H2.min->nodeKey < min->nodeKey) {
            min = H2.min;
        }
        numNodes += H2.numNodes;

        //Makes H2 "empty"
        H2.min = nullptr;
        H2.head = nullptr;
        H2.tail = nullptr;
        H2.numNodes = 0;
    }
    void printKey() { //prints using preorder for each tree
        if (min != nullptr) {
            FibHeapNode<key> *cur = head;
            do {
                cout << "Rank " << cur->rank << endl;
                cout << cur->nodeKey << " ";
                recursivePrint(cur->child);
                cout << endl << endl;
                cur = cur->right;
            } while (cur != head);
        }
    }
};