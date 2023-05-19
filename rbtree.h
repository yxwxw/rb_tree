#ifndef MY_RBTREE
#define MY_RBTREE
#include <iostream>
#include <string>
#include <cassert>

enum nodeColor { red, black };

template <typename Key, typename T>
struct Node {
    Node() { left = right = nullptr; }
    Node(Key key, T val) { this->val = val; this->key = key; left = right = nullptr; }
    Node(Key key, T val, nodeColor c) { this->key = key; this->val = val; left = right = nullptr; color = c; }
    T val;
    Key key;
    Node *left, *right;
    nodeColor color;
};

template <typename Key, typename T>
class RBtree {
public:
    RBtree();
    ~RBtree() { clear(); }

    int size();
    bool isEmpty();
    int height();
    T get(Key key);
    bool contains(Key key);
    void put(Key key, T val);
    Key deleteMin();
    Key deleteMax();
    void deleteNode(Key key);
    void clear();
    void output();
    bool is23Tree();
    bool isBalanced();
    Key min();
    Key max();
    Key precursor(Key key);
    Key successor(Key key);
    Node<Key, T>* insert(Node<Key, T> *f, Node<Key, T> *s, int flag);
    void my_guess();

private:
    bool isRed(Node<Key, T>* h);
    T* get(Node<Key, T>* x, Key key);
    int size(Node<Key, T>* h);
    int height(Node<Key, T>* h);
    Node<Key, T>* put(Node<Key, T>* h, Key key, T val);
    Node<Key, T>* deleteMin(Node<Key, T>* h);
    Node<Key, T>* deleteMax(Node<Key, T>* h);
    Node<Key, T>* deleteNode(Node<Key, T>* h, Key key);
    Node<Key, T>* rotateRight(Node<Key, T>* h);
    Node<Key, T>* rotateLeft(Node<Key, T>* h);
    void flipColors(Node<Key, T>* h);
    Node<Key, T>* moveRedLeft(Node<Key, T>* h);
    Node<Key, T>* moveRedRight(Node<Key, T>* h);
    Node<Key, T>* balance(Node<Key, T> *h);
    bool is23Tree(Node<Key, T>* h);
    bool isBalanced(Node<Key, T> *h, int black);
    Node<Key, T>* precursor(Node<Key, T> *cur, Key key);
    Node<Key, T>* successor(Node<Key, T> *cur, Key key);
    
    void my_guess(Node<Key, T> *h);
    Node<Key, T>* min(Node<Key, T> *h);
    Node<Key, T>* max(Node<Key, T> *h);
    void output(Node<Key, T>* h);
    void outputOne(Node<Key, T>* h);
    void clear(Node<Key, T>* h);
    int rbtreeSize;
    Key lastMin, lastMax;
    Node<Key, T> *root;
};

template <typename Key, typename T>
RBtree<Key, T>::RBtree() {
    rbtreeSize = 0;
    root = nullptr;
}

template <typename Key, typename T>
bool RBtree<Key, T>::isRed(Node<Key, T>* h) {
    if (nullptr == h)
        return false;
    return h->color == nodeColor::red;
}

template <typename Key, typename T>
int RBtree<Key, T>::size() {
    return size(root);
}

template <typename Key, typename T>
int RBtree<Key, T>::size(Node<Key, T>* h) {
    if (nullptr == h)
        return 0;
    return size(h->left) + size(h->right) + 1;
}

template <typename Key, typename T>
int RBtree<Key, T>::height() {
    return height(root);
}

template <typename Key, typename T>
int RBtree<Key, T>::height(Node<Key, T>* h) {
    if (nullptr == h)
        return 0;
    return std::max(height(h->left), height(h->right)) + 1;
}

template <typename Key, typename T>
bool RBtree<Key, T>::isEmpty() {
    int size = this->size();
    assert(size == rbtreeSize);
    return rbtreeSize == 0;
}

template <typename Key, typename T>
T RBtree<Key, T>::get(Key key) {
    return *get(root, key);
}

template <typename Key, typename T>
T* RBtree<Key, T>::get(Node<Key, T>* h, Key key) {
    if (nullptr == h)
        return nullptr;
    if (h->key < key)
        return get(h->right, key);
    if (h->key > key)
        return get(h->left, key);
    return &h->val;
}

template <typename Key, typename T>
bool RBtree<Key, T>::contains(Key key) {
    return get(root, key) != nullptr;
}

/**
 *  Rotate left
 *      <A>                               <B>          
 *     /  \                              /  \
 *   <A0> <B>      ==rotate left==>    <A> <B2>    
 *       /  \                          / \
 *     <B1> <B2>                    <A0><B1>  
*/
template <typename Key, typename T>
Node<Key, T> *RBtree<Key, T>::rotateLeft(Node<Key, T>* A) {
    assert(A != nullptr);
    assert(isRed(A->right));

    Node<Key, T>* B = A->right;
    A->right = B->left;
    B->left = A;
    // change color and make A red
    B->color = A->color;
    A->color = nodeColor::red;
    return B;
}

/**
 *  Rotate right
 *      <A>                              <B>
 *     /  \                             /  \
 *    <B> <A0>    ==rotete right==>   <B1> <A>
 *   /  \                                 /  \     
 * <B1><B2>                             <B2><A0>
*/

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::rotateRight(Node<Key, T>* A) {
    assert(A != nullptr);
    assert(isRed(A->left));

    Node<Key, T>* B = A->left;
    A->left = B->right;
    B->right = A;
    B->color = A->color;
    A->color = nodeColor::red;
    return B;
}    

/**
 *   <Black>                         <RED>
 *   /     \       ===flip===>      /     \
 * <RED>  <RED>                  <Black><Black> 
*/
template <typename Key, typename T>
void RBtree<Key, T>::flipColors(Node<Key, T>* h) {
    h->color        = (h->color        == nodeColor::black) ? nodeColor::red : nodeColor::black;
    h->left->color  = (h->left->color  == nodeColor::black) ? nodeColor::red : nodeColor::black; 
    h->right->color = (h->right->color == nodeColor::black) ? nodeColor::red : nodeColor::black;
}

/**
 * insert a node into rbtree
 * the root must be black
*/
template <typename Key, typename T>
void RBtree<Key, T>::put(Key key, T val) {
    if (root == nullptr) {
        root = new Node<Key, T>(key, val, nodeColor::black);
        rbtreeSize += 1;
    } else {
        assert(root != nullptr);
        root = put(root, key, val);
    }
    root->color = nodeColor::black;
    // assert(size() == rbtreeSize);
}

/**
 * insert a node into rbtree which is red by default
 * if h is a left-leaning 3 node: Do nothing
 * if h is a right-leaning 3 node: rotate left
 * 
 * if the node insert into a previous 3 node is
 * LARGER:  flip color
 * SMALLER: rotate right, flip color 
 * BETWEEN: rotate left, rotate right, flip color
 */
template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::put(Node<Key, T>* h, Key key, T val) {
    // way down 
    // find a location to place the new node
    if (nullptr == h) {
        rbtreeSize += 1;
        return new Node<Key, T>(key, val, nodeColor::red);
    } 
    
    if (h->key < key) {
        h->right = put(h->right, key, val);
    } else if (h->key > key) {
        h->left = put(h->left, key, val);
    } else {
        h->val = val;
        return h;
    }

    // way up
    // balcance the tree
    return balance(h);
}

template <typename Key, typename T>
Key RBtree<Key, T>::deleteMin() {
    // can't delete a node which is empty
    if (isEmpty())
        return Key();
    // is both child of root is black, set root to red
    if (!isRed(root->left) && !isRed(root->right)) 
        root->color = nodeColor::red;

    root = deleteMin(root);
    rbtreeSize -= 1;

    if (!isEmpty()) 
        root->color = nodeColor::black;

    return lastMin;
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::deleteMin(Node<Key, T>* h) {
    // left node of h lives
    if (nullptr == h->left) {
        lastMin = h->val;
        delete h;
        return nullptr;
    }
    
    // left is a 2-node
    // move a node from its slibings
    if (!isRed(h->left) && !isRed(h->left->left))
        h = moveRedLeft(h);
    
    h->left = deleteMin(h->left);
    
    return balance(h);
}

template <typename Key, typename T>
Key RBtree<Key, T>::deleteMax() {
    if (isEmpty())
        return Key();

    if (!isRed(root->left) && !isRed(root->right))
        root->color = nodeColor::red;

    root = deleteMax(root);
    rbtreeSize -= 1;
    if (!isEmpty())
        root->color = nodeColor::black;
    
    return lastMax;
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::deleteMax(Node<Key, T>* h) {
    if (isRed(h->left))
        h = rotateRight(h);

    if (nullptr == h->right) {
        lastMax = h->val;
        delete h;
        return nullptr;
    }

    if (!isRed(h->right) && !isRed(h->right->left))
        h = moveRedRight(h);

    h->right = deleteMax(h->right);

    return balance(h);
}

template <typename Key, typename T>
void RBtree<Key, T>::deleteNode(Key key) {
    if (!contains(key))
        return;
    
    if (!isRed(root->left) && !isRed(root->right))
        root->color = nodeColor::red;
    
    root = deleteNode(root, key);
    rbtreeSize -= 1;

    if (!isEmpty())
        root->color = nodeColor::black;
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::deleteNode(Node<Key, T>* h, Key key) {
    // enter left subtree
    if (key < h->key) {
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = deleteNode(h->left, key);
    }
    // enter right subtree
    else {
        // change left leaning to right leaning
        if (isRed(h->left))
            h = rotateRight(h);
        if (key == h->key && nullptr == h->right) {
            delete h;
            return nullptr;
        }
        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);
        if (key == h->key) {
        // get the min of right subtree, also known as successor of h
            Node<Key, T> *x = min(h->right);
            h->key = x->key;
            h->val = x->val;
        // delete the min of right subtree
            h->right = deleteMin(h->right);
        } else {
            h->right = deleteNode(h->right, key);
        }
    }
    return balance(h);
}


template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::moveRedLeft(Node<Key, T>* h) {

//  assert(h->color == nodeColor::red);
//  assert(h->left->color == nodeColor::black);
//  assert(h->right->color == nodeColor::black);

    /**
     *  now node h is a 4-node
    */
    flipColors(h);
    
//  assert(h->color == nodeColor::black);
//  assert(h->left->color == nodeColor::red);
//  assert(h->right->color == nodeColor::red);
    /**
        direct sibling os h->left is a 3-node
        so, borrow one item from direct sibling 
        :<
    */
    if (isRed(h->right->left)) {
        h->right = rotateRight(h->right);
        h = rotateLeft(h);
        flipColors(h); 
    }
    return h;
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::moveRedRight(Node<Key, T>* h) {
    assert(h->color == nodeColor::red);
    assert(h->left->color == nodeColor::black);
    assert(h->right->color == nodeColor::black);
    flipColors(h);
    if (isRed(h->left->left)) {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

/**
 * balance a node 
 * no right-leaning 3-node
 * no 4-node
 * no left-leaning but grandson is red
*/
template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::balance(Node<Key, T>* h) {
    if (isRed(h->right) && !isRed(h->left))
        h = rotateLeft(h);
    if (isRed(h->left) && isRed(h->left->left))
        h = rotateRight(h);
    if (isRed(h->left) && isRed(h->right))
        flipColors(h);
    return h;
}

/**
 * clear the whole rb tree
 * recall memory
*/
template <typename Key, typename T>
void RBtree<Key, T>::clear() {
    clear(root);
    root = nullptr;
    rbtreeSize = 0;
}

template <typename Key, typename T>
void RBtree<Key, T>::clear(Node<Key, T>* h) {
    if (nullptr != h) {
        clear(h->left);
        clear(h->right);
        // std::cout << "clear ";
        // if (isRed(h))
        //     std::cout << "\033[1;31mRED\033[0m   ";
        // else 
        //     std::cout << "\033[1;40mBLACK\033[0m "; 
        // std::cout << "KEY: " << h->key << ", VAL: " << h->val << std::endl;
        delete h;
        h = nullptr;
    }
}

template <typename Key, typename T>
Key RBtree<Key, T>::successor(Key key) {
    auto ret = successor(root, key);
    if (nullptr == ret) {
        return Key();
    } 
    return ret->val;
}

/**
 * return the successor of the node h
*/
template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::successor(Node<Key, T> *cur, Key key) {
    Node<Key, T>* ret = nullptr;
    if (nullptr == cur) 
        return ret;
    // if current node is less or equal than h
    // the successor must in the right child
    if (cur->val <= key) {
        ret = successor(cur->right, key);
    } else {
    // current node is greater than h
    // succssor is cur or in the left child
        Node<Key, T> *tmp = successor(cur->left, key);
        ret = tmp == nullptr ? cur : tmp;
    }
    return ret;
}

template <typename Key, typename T>
Key RBtree<Key, T>::precursor(Key key) {
    auto ret = precursor(root, key);
    if (nullptr == ret) {
        return Key();
    } 
    return ret->val;
}


template <typename Key, typename T>
Key RBtree<Key, T>::max() {
    if (nullptr == root)
        return Key();

    auto ret = max(root);
    if (nullptr == ret)
        return Key();

    return max(root)->val;
}

template <typename Key, typename T>
Key RBtree<Key, T>::min() {
    if (nullptr == root)
        return Key();

    auto ret = min(root);
    if (nullptr == ret)
        return Key();
        
    return min(root)->val;
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::min(Node<Key, T> *h) {
    if (nullptr == h)
        return nullptr;

    if (h->left == nullptr)
        return h;
    
    return min(h->left);
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::max(Node<Key, T> *h) {
    if (nullptr == h)
        return nullptr;

    if (h->right == nullptr)
        return h;

    return max(h->right);
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::precursor(Node<Key, T> *cur, Key key) {
    Node<Key, T>* ret = nullptr;
    if (nullptr == cur)
        return ret;
    
    // if current node is greater or equal than h
    // the precursor must in the left child
    if (cur->val >= key) {
        ret = precursor(cur->left, key);
    } else {
    // current node is less than h
    // precursor is cur or in the right child
        Node<Key, T> *tmp = precursor(cur->right, key);
        ret = tmp == nullptr ? cur : tmp;
    }
    return ret;
}

/*
 * caculate numbers of black links on path from root to min
*/
template <typename Key, typename T>
bool RBtree<Key, T>::isBalanced() {
    int black = 0;
    auto x = root;
    while (nullptr != x) {
        if (!isRed(x))
            black += 1;
        x = x->left;
    }
    return isBalanced(root, black);
}

/*
 * do all paths from the root to a leaf have the given number of black link?
*/
template <typename Key, typename T>
bool RBtree<Key, T>::isBalanced(Node<Key, T> *h, int black) {
    if (nullptr == h)
        return black == 0;
    if (!isRed(h))
        black -= 1;
    return isBalanced(h->left, black) && isBalanced(h->right, black);
}

template <typename Key, typename T>
Node<Key, T>* RBtree<Key, T>::insert(Node<Key, T> *f, Node<Key, T>* s, int flag) {
    if (f == nullptr)
        return root;
    if (flag == 1) {
        f->right = s;
    }
    if (flag == 0) {
        f->left = s;
    }
    rbtreeSize += 1;
    return nullptr;
}

/*------------------- helper function ------------------*/

template <typename Key, typename T>
void RBtree<Key, T>::output() {
    output(this->root);
}

template <typename Key, typename T>
void RBtree<Key, T>::outputOne(Node<Key, T>* h) {
    if (nullptr != h) {
        if (isRed(h))
            std::cout << "\033[1;31mRED\033[0m   ";
        else 
            std::cout << "\033[1;40mBLACK\033[0m "; 
        std::cout << "KEY: " << h->key;
        std::cout << " Left: ";
        if (h->left == nullptr) std::cout << "NULL ";
        else std::cout << h->left->key;
        
        std::cout << " Right: ";
        if (h->right == nullptr) std::cout << "NULL ";
        else std::cout << h->right->key;

        std::cout << std::endl;
    }
}

template <typename Key, typename T>
void RBtree<Key, T>::output(Node<Key, T>* h) {
    if (nullptr == h)
        return;
    outputOne(h);
    output(h->left);
    output(h->right);
}

template <typename Key, typename T>
bool RBtree<Key, T>::is23Tree() {
    return is23Tree(root);
}

template <typename Key, typename T>
bool RBtree<Key, T>::is23Tree(Node<Key, T>* h) {
    if (nullptr == h)
        return true;
    if (isRed(h->right))
        return false;
    if (h != root && isRed(h) && isRed(h->left))
        return false;
    
    return is23Tree(h->left) && is23Tree(h->right);
}

template <typename Key, typename T>
void RBtree<Key, T>::my_guess() {
    if (nullptr == root)
        return;
    my_guess(root);    
}

template <typename Key, typename T>
void RBtree<Key, T>::my_guess(Node<Key, T> *h) {
    if (nullptr == h)
        return;
    if (h->left != nullptr && h->right != nullptr) {
        // std::cout << "h->key = " << h->key << std::endl;
        assert(max(h->left) == precursor(h, h->key)->val);
        assert(min(h->right) == successor(h, h->key)->val);
    }
    my_guess(h->left);
    my_guess(h->right);
}

#endif