#include <iostream>
#include <string>
using namespace std;

template <typename key, typename value>
class node {
public:
    key nodeKey;
    value nodeVal;
    int numNodes;
    char nodeColor; //b for black, r for red, 0 for not declared yet
    node* parent;
    node* leftChild;
    node* rightChild;
    node() {
        parent = nullptr;
        leftChild = nullptr;
        rightChild = nullptr;
        numNodes = 1;
        nodeColor = 0;
    }
    node(string nilMarker) { //default constructor for nil node
        if (nilMarker == "nilMarker") {
            parent = nullptr;
            leftChild = nullptr;
            rightChild = nullptr;
            numNodes = 0;
            nodeColor = 'b';
        }
        else { //acts as a normal constructor incase of string use as the keytype
            parent = nullptr;
            leftChild = nullptr;
            rightChild = nullptr;
            numNodes = 1;
            nodeColor = 0;
        }
    }
    node(key insertKey, value insertValue) { //default constructor for a given key and value
        nodeKey = insertKey;
        nodeVal = insertValue;
        parent = nullptr;
        leftChild = nullptr;
        rightChild = nullptr;
        numNodes = 1;
        nodeColor = 0;
    }
    node(key insertKey, value insertValue, int insertNumNodes, char insertNodeColor) { //default constructor for deepCopy
        nodeKey = insertKey;
        nodeVal = insertValue;
        parent = nullptr;
        leftChild = nullptr;
        rightChild = nullptr;
        numNodes = insertNumNodes;
        nodeColor = insertNodeColor;
    }
};

template <typename key, typename value>
class RBTree {
private:
    node<key,value> *root;
    node<key,value> *nil;

    void insertFixUp(node<key,value> *n) { //adjusts tree to fix color rules
            node<key,value> *y;
            while (n->parent->nodeColor == 'r') {
                if (n->parent == n->parent->parent->leftChild) { //if n is on left side of tree
                    y = n->parent->parent->rightChild;
                    if (y->nodeColor == 'r') { //case 1: n's uncle is red
                        n->parent->nodeColor = 'b';
                        y->nodeColor = 'b';
                        n->parent->parent->nodeColor = 'r';
                        n = n->parent->parent;
                    }
                    else {
                        if (n == n->parent->rightChild) { //case 2: n's uncle is black and n is a right child
                            n = n->parent;                      
                            rotateLeft(n);
                        }
                        n->parent->nodeColor = 'b';         //case 3: n's uncle is black and n is a left child
                        n->parent->parent->nodeColor = 'r';
                        rotateRight(n->parent->parent);
                    }
                }
                else { //if n is on the right side of the tree
                    y = n->parent->parent->leftChild;
                    if (y->nodeColor == 'r') { //case 1: n's uncle is red
                        n->parent->nodeColor = 'b';
                        y->nodeColor = 'b';
                        n->parent->parent->nodeColor = 'r';
                        n = n->parent->parent;
                    }
                    else {
                        if (n == n->parent->leftChild) { //case 2: n's uncle is black and n is a left child
                            n = n->parent;                      
                            rotateRight(n);
                        }
                        n->parent->nodeColor = 'b';         //case 3: n's uncle is black and n is a right child
                        n->parent->parent->nodeColor = 'r';
                        rotateLeft(n->parent->parent);
                    }
                }
            }

        root->nodeColor = 'b'; //root is always black
    }
    void deleteFixup(node<key,value> *n) {
        node<key,value> *w = nullptr;
        while (n != root && n->nodeColor == 'b') {
            if (n == n->parent->leftChild) { //if n is on the left side of the tree
                w = n->parent->rightChild;
                if (w->nodeColor == 'r') { //case 1: if the uncle is red
                    w->nodeColor = 'b';
                    n->parent->nodeColor = 'r';
                    rotateLeft(n->parent);
                    w = n->parent->rightChild;
                }
                if (w->leftChild->nodeColor == 'b' && w->rightChild->nodeColor == 'b') { // case 2: if uncle's children are both black
                    w->nodeColor = 'r';
                    n = n->parent;
                }
                else { 
                    if (w->rightChild->nodeColor == 'b') { //case 3: w's left child is red and w's right child is black
                        w->leftChild->nodeColor = 'b';
                        w->nodeColor = 'r';
                        rotateRight(w);
                        w = n->parent->rightChild;
                    }
                    w->nodeColor = n->parent->nodeColor; //case 4: both w's children are red
                    n->parent->nodeColor = 'b';
                    w->rightChild->nodeColor = 'b';
                    rotateLeft(n->parent);
                    n = root;
                }
            }
            else { //if n is on the right side of the tree
                w = n->parent->leftChild;
                if (w->nodeColor == 'r') { //if the uncle is red
                    w->nodeColor = 'b';
                    n->parent->nodeColor = 'r';
                    rotateRight(n->parent);
                    w = n->parent->leftChild;
                }
                if (w->rightChild->nodeColor == 'b' && w->leftChild->nodeColor == 'b') { //if uncle's children are both black
                    w->nodeColor = 'r';
                    n = n->parent;
                }
                else {
                    if (w->leftChild->nodeColor == 'b') { //case 3: w's right child is red and w's left child is black
                        w->rightChild->nodeColor = 'b';
                        w->nodeColor = 'r';
                        rotateLeft(w);
                        w = n->parent->leftChild;
                    }
                    w->nodeColor = n->parent->nodeColor; //case 4: both w's children are red
                    n->parent->nodeColor = 'b';
                    w->leftChild->nodeColor = 'b';
                    rotateRight(n->parent);
                    n = root;
                }
            }
        }
        n->nodeColor = 'b';
    }

    void transplant(node<key,value> *x, node<key,value> *y) { //transplants y in place of x
        if (x->parent == nil) { //x is the root, make y the root
            root = y;
        }
        else if(x == x->parent->leftChild) { //if x was a left child, y is now a left child
            x->parent->leftChild = y;
        }
        else { //if x was a right child, make y a right child
            x->parent->rightChild = y;
        }
        y->parent = x->parent; //making y's parent x's parent
    }
    node<key,value> *treeMaximum(node<key,value> *curNode) {
        if (curNode->rightChild == nil) return curNode;
        return treeMaximum(curNode->rightChild);
    }

    void rotateLeft(node<key,value> *n) {
        node<key,value> *y = n->rightChild; //y will become the new parent        
        n->rightChild = y->leftChild;  //n's right child is now y's left child
        if (y->leftChild != nil) { //makes n the parent of y's left child if it has one 
            y->leftChild->parent = n;
        }
        y->parent = n->parent;
        //adjusting the pointers to perform the rotation
        if (n->parent == nil) { //if n was the root, make y the root
            root = y;
        }
        else if (n == n->parent->leftChild) { //if n was its parent's left child, make parents left y
            n->parent->leftChild = y;
        }
        else { //if n was it's parent's right child, make parents right y
            n->parent->rightChild = y; 
        }

        //final adjustments
        y->leftChild = n; //make n y's left child
        n->parent = y; //make y n's parent

        y->numNodes = n->numNodes;
        n->numNodes = n->leftChild->numNodes + n->rightChild->numNodes + 1;
        
    }
    void rotateRight(node<key,value> *n) {
        node<key,value> *y = n->leftChild; //will become new parent
        n->leftChild = y->rightChild;

        if (y->rightChild != nil) { //if y doesn't have  right child, n becomes it's parent
            y->rightChild->parent = n;
        }
        y->parent = n->parent;
        if (n->parent == nil) { //if n was the root, y is now the root
            root = y;
        }
        else if (n == n->parent->rightChild) {
            n->parent->rightChild = y;
        }
        else {
            n->parent->leftChild = y; 
        }

        //updating y and n's attributes
        y->rightChild = n;
        n->parent = y;
        y->numNodes = n->numNodes;
        n->numNodes = n->leftChild->numNodes + n->rightChild->numNodes + 1;
    }

    node<key,value> *nodeSearch(key searchKey) {//returns a pointer to the node that contains the appropriate searchKey, returns nullptr if not found
        node<key,value> *y = root;
        while (y != nil) {
            if (searchKey < y->nodeKey) {
                y = y->leftChild;
            }
            else if (searchKey > y->nodeKey) {
                y = y->rightChild;
            }
            else { // searchKey == nodeKey
                return y;
            }
        }
        return nullptr;
    }
    key recursiveSelect(node<key,value> *curNode, int pos) {//returns a key from a node at a given position
        int r = curNode->leftChild->numNodes + 1;
        if (pos == r) return curNode->nodeKey;
        else if(pos < r) return recursiveSelect(curNode->leftChild, pos);
        else return recursiveSelect(curNode->rightChild, pos - r);
    }
    node<key,value> *recursivePtrSelect(node<key,value> *curNode, int pos) {//returns a pointer to the node that has the key at a given rank
        int r = curNode->leftChild->numNodes + 1;
        if (pos == r) return curNode;
        else if(pos < r) return recursivePtrSelect(curNode->leftChild, pos);
        else return recursivePtrSelect(curNode->rightChild, pos - r);
    }

//FIXME: Remove extra output in print statements to match formatting
    void preTraversal(node<key,value> *curNode) {
        if(curNode == nil) return;
        cout << curNode->nodeKey << " ";
        preTraversal(curNode->leftChild);
        preTraversal(curNode->rightChild);
    }
    void postTraversal(node<key,value> *curNode) {
        if(curNode == nil) return;
        postTraversal(curNode->leftChild);
        postTraversal(curNode->rightChild);
        cout << curNode->nodeKey << " ";
    }
    void kTraversal(node<key,value> *curNode, int &k) { //inorder traversal k times, k keeps track of how many statements have been printed
        if(curNode == nil) return;
        kTraversal(curNode->leftChild, k);
        if(k == 0) return;
        cout << curNode->nodeKey << " ";
        k--;
        kTraversal(curNode->rightChild, k);
    }
    void treeDelete(node<key,value> *curNode) { //recursive tree deletion using postorder traversal
        if(curNode == nil) return;
        treeDelete(curNode->leftChild);
        treeDelete(curNode->rightChild);
        delete curNode;
    }
    void deepCopy(node<key,value> *&localRoot, node<key,value> *newParent, node<key,value> *srcRoot) {
        if (srcRoot->numNodes == 0) {
            localRoot = nil;
        }
        else {
            localRoot = new node<key,value>(srcRoot->nodeKey, srcRoot->nodeVal, srcRoot->numNodes, srcRoot->nodeColor);
            localRoot->parent = newParent;
            deepCopy(localRoot->leftChild, localRoot, srcRoot->leftChild); //left and right children updated due to pass by reference
            deepCopy(localRoot->rightChild, localRoot, srcRoot->rightChild);
        }

    }
public:
    RBTree() { //makes an empty tree
        nil = new node<key,value>("nilMarker");
        root = nil;
    }
    RBTree(key k[], value v[], int s) {//build a tree with the arrays, with the size s
        nil = new node<key,value>("nilMarker");
        root = nil;
        for(int i = 0; i < s; i++) {
            insert(k[i], v[i]);
        }
    }

    ~RBTree() {//destructor, should probably use post order
        treeDelete(root);
        delete nil;
    }
    RBTree(const RBTree &src) { //copy constructor
        nil = new node<key,value>("nilMarker");
        deepCopy(root, nil, src.root);
    } 
    RBTree &operator=(const RBTree &src) { //copy assignment operator
        if (this != &src) {
            // delete the tree
            if (root != nil) {
                treeDelete(root);
            }
            delete nil;
            nil = new node<key,value>("nilMarker");
            deepCopy(root, nil, src.root);
        }
        return *this;
    }
    
    value *search(key k) { //traditional binary search, returns pointer to node if found, NULL if not found
        node<key,value> *y;
        y = root;
        while (y != nil) { //equivalent to recursive call of binary search
            if (k < y->nodeKey) {
                y = y->leftChild;
            }
            else if (k > y->nodeKey) {
                y = y->rightChild;
            }
            else { // k == nodeKey
                value *ptrToVal = &(y->nodeVal);
                return ptrToVal;
            }
        }
        return nullptr; //returns nullptr if not found
    }
    
    void insert(key k, value v) {//inserts a node with the key and value into the tree, will inovlve several cases
        node<key,value> *newNode;
        newNode = new node<key,value>(k, v);
    
        node<key,value> *y = nil;
        node<key,value> *x = root;
        while (x != nil) { //finds the appropriate node to insert z at, incrememnting numNodes along the way for rank
            y = x;
            if (newNode->nodeKey < x->nodeKey) {
                x->numNodes += 1;
                x = x->leftChild;
            }
            else {
                x->numNodes += 1;
                x = x->rightChild;
            }
        }
        newNode->parent = y;
        if (y == nil) { //if the tree is empty
            root = newNode;
        }
        else if (newNode->nodeKey < y->nodeKey) { //if n is less than parent
            y->leftChild = newNode;
        }
        else { //if n is >= parent
            y->rightChild = newNode;
        }

        //updating attributes of the new child and calling fixup
        newNode->leftChild = nil;
        newNode->rightChild = nil;
        newNode->nodeColor = 'r';
        insertFixUp(newNode);

    } 
    int remove(key k) { //removes the specified from the tree (1 if found, 0 if not found), if key is not a leaf use predecessor 
        node<key,value> *n = nodeSearch(k);
        node<key,value> *x = nullptr;
        if (n == nullptr) return 0; //can't delete from an empty tree
        else {
            node<key,value> *y = n;
            node<key,value> *tempNode = y;
            char yOriginalColor = y->nodeColor;
            if (n->leftChild == nil) { //if n doesn't have a left child, slide right child up
                x = n->rightChild;
                transplant(n,n->rightChild);
                while(tempNode != nil) { //update number of nodes value for each node on the path to the root
                    if (tempNode->numNodes > 1) {
                        tempNode->numNodes--; 
                    }
                tempNode = tempNode->parent;
                }
            }
            else if (n->rightChild == nil) { //if n doesn't have a right child, slide left child up
                x = n->leftChild;
                transplant(n, n->leftChild);
                while(tempNode != nil) { //update number of nodes value for each node on the path to the root
                    if (tempNode->numNodes > 1) {
                        tempNode->numNodes--; 
                    }
                tempNode = tempNode->parent;
                }
            }
            else { //n has 2 children, do a transplant with predecessor
                y = treeMaximum(n->leftChild);
                tempNode = y;
                while(tempNode != nil) { //update number of nodes value for each node on the path to the root, starting from the new y now
                    if (tempNode->numNodes > 1) {
                        tempNode->numNodes--; 
                    }
                    tempNode = tempNode->parent;
                }
                yOriginalColor = y->nodeColor;
                x = y->leftChild;
                if (y->parent == n) {
                    x->parent = y;
                }
                else {
                    transplant(y, y->leftChild);
                    y->leftChild = n->leftChild;
                    y->leftChild->parent = y;
                }
                transplant(n, y);

                //assigning y with n's previous attributes
                y->rightChild = n->rightChild;
                y->rightChild->parent = y;
                y->nodeColor = n->nodeColor;
                y->numNodes = n->numNodes;
            }
            
            if(yOriginalColor == 'b') { //only need to call fixup when the original color was black due to black node properties
                deleteFixup(x);
            }
            delete n;
        }
        root->numNodes = root->leftChild->numNodes + root->rightChild->numNodes + 1; //updating root's number of nodes
        return 1;
    }
    int rank(key k) { //returns the rank of a given key, returns 0 if not found, numNodes are used during 
        node<key,value> *rankNode;
        rankNode = nodeSearch(k);
        if (rankNode != nullptr) { //if the tree isn't empty
            int r = rankNode->leftChild->numNodes + 1; //number of nodes on left side of tree
            node<key,value> *y = rankNode;
            while (y != root) {
                if (y == y->parent->rightChild) {
                    r = r + y->parent->leftChild->numNodes + 1; //updates rank if on right side of tree
                }
                y = y->parent;
            }
            return r;
        }
        return 0;
    }
    key select(int pos) { //returns key of node at given pos (rank) in the tree,
        return recursiveSelect(root, pos);
    }
    key *successor(key k){ //returns pointer the key after the specified key in the tree
        int curKeyRank = rank(k);
        if (curKeyRank == root->numNodes) return NULL;
        else {
            node<key,value> *successorNode = recursivePtrSelect(root, curKeyRank + 1);
            key *returnKey = &(successorNode->nodeKey);
            return returnKey;
        }
    }
    key *predecessor(key k) { //returns pointer the key before the specified key in the tree
        int curKeyRank = rank(k);
        if (curKeyRank == 1) return NULL;
        else {
            node<key,value> *predecessorNode = recursivePtrSelect(root, curKeyRank - 1);
            key *returnKey = &(predecessorNode->nodeKey);
            return returnKey;
        }
    }
    
    int size() {
        return root->numNodes;
    }
    void preorder() { //prints the tree keys w/ preorder, list separated by spaces and terminated w/ a newline
        preTraversal(root);
        cout << endl;
    }
    void inorder() { //prints the tree keys w/ inorder, list separated by spaces and terminated w/ a newline
        int numPrintNodes = root->numNodes;
        kTraversal(root, numPrintNodes);
        cout << endl;
    }
    void postorder() { //prints the tree keys w/ postorder, list separated by spaces and terminated w/ a newline
        postTraversal(root);
        cout << endl;
    }
    void printk(int k) { //prints the smallest k keys in the trees, list is separated by spaces and ended w/ a new line
        if (k < 1 || k > root->numNodes) return;
        kTraversal(root, k);
        cout << endl;
    } 
};