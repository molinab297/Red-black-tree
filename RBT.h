
#ifndef BSTPRACTICE_RBT_H
#define BSTPRACTICE_RBT_H

template <class T>
struct rbtNode{rbtNode<T> *left, *right, *parent; T data; bool color;}; // 0 = black, 1 = red

template <class T>
class RBT{

public:
    RBT();
    virtual ~RBT();

    void insert(T key);
    void bfs();
    void DrawTree();
    bool remove(T key);
    bool search(T key);
    bool isEmpty();


private:
    rbtNode<T> *root;
    rbtNode<T> *createLeaf(T data);
    rbtNode<T> *findGrandparent(rbtNode<T>* node);
    rbtNode<T> *findUncle(rbtNode<T>* node);
    rbtNode<T> *searchForNodePrivate(T key);
    int DrawTreePrivate(rbtNode<T> *tree, int is_left, int offset, int depth, char s[20][255]);

    /* restructuring & recoloring methods */
    void recolor(rbtNode<T>* x);
    void swapColors(rbtNode<T>* a, rbtNode<T> *b);
    void restructure(rbtNode<T> *x);
    void fixRoot();
    void fixTree(rbtNode<T> *start);
    void singleRotateRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
    void singleRotateLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
    void rotateRightRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
    void rotateLeftLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
    void rotateLeftRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
    void rotateRightLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent);
};

template <class T>
RBT<T>::RBT() { root = nullptr; }

template <class T>
RBT<T>::~RBT() { }

template <class T>
void RBT<T>::fixRoot() {
    if(root->parent){
        while(root->parent)
            root = root->parent;
    }
}

template <class T>
void RBT<T>::insert(T key) {
    if(isEmpty()) {
        root = createLeaf(key);
        root->color = 0;
    }
    else{
        rbtNode<T> *curr{root}, *trail{nullptr};
        while(curr){
            trail = curr;
            (key < curr->data) ? curr = curr->left : curr = curr->right;
        }

        // insert the node
        rbtNode<T> *newNode{nullptr};
        if (key < trail->data){
            trail->left = createLeaf(key); trail->left->parent = trail;
            newNode = trail->left;
        }
        else{
            trail->right = createLeaf(key); trail->right->parent = trail;
            newNode = trail->right;
        }

        fixTree(newNode);
    }
}

template <class T>
void RBT<T>::fixTree(rbtNode<T> *start) {

    if(start->parent){
        if(start->parent->color == 1 && start->color == 1 )
        {
            // verify node has an uncle
            rbtNode<T>*uncle = findUncle(start);
            if(uncle){
                if(uncle->color == 1) // if the node's uncle is red, recolor the tree
                    recolor(start);
                else { // if the node's uncle is black, restructure the tree
                    restructure(start);
                    fixRoot();
                }
            }
            else { // NULL uncle nodes still require restructuring
                restructure(start);
                fixRoot();
            }
        }
    }
}

template <class T>
void RBT<T>::restructure(rbtNode<T> *x) {
    rbtNode<T> *grandparent = findGrandparent(x);
    if(grandparent){
        rbtNode<T> *parent = x->parent;
        if(grandparent->left == parent){
            if(parent->left == x) {
                rotateLeftLeft(x, parent, grandparent);
                fixTree(parent);
            }
            else {
                rotateLeftRight(x, parent, grandparent);
                fixTree(x); // come back here
            }
        }
        else{
            if(parent->right == x) {
                rotateRightRight(x, parent, grandparent);
                fixTree(parent);
            }
            else {
                rotateRightLeft(x, parent, grandparent);
                fixTree(x);
            }
        }
    }
}

template <class T>
void RBT<T>::singleRotateLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent) {
    parent->right = x->left;
    if(x->left)
        x->left->parent = parent;
    x->left = parent;
    parent->parent = x;
    x->parent = grandparent;
    grandparent->left = x;
}

template <class T>
void RBT<T>::singleRotateRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent) {
    parent->left = x->right;
    if(x->right)
        x->right->parent = parent;
    x->right = parent;
    parent->parent = x;
    x->parent = grandparent;
    grandparent->right = x;
}

template <class T>
void RBT<T>::rotateLeftLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent) {
    rbtNode<T> *greatGrandparent = grandparent->parent;
    grandparent->left = parent->right;
    if(parent->right)
        parent->right->parent = grandparent;
    grandparent->parent = parent;
    parent->right = grandparent;
    parent->parent = nullptr;
    swapColors(parent, grandparent);

    if(greatGrandparent) {
        parent->parent = greatGrandparent;
        greatGrandparent->right == grandparent ?
                greatGrandparent->right = parent : greatGrandparent->left = parent;
    }
}

template <class T>
void RBT<T>::rotateRightRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent) {
    rbtNode<T> *greatGrandparent = grandparent->parent;
    grandparent->right = parent->left;
    if(parent->left)
        parent->left->parent = grandparent;
    grandparent->parent = parent;
    parent->left = grandparent;
    parent->parent = nullptr;
    swapColors(parent, grandparent);

    if(greatGrandparent) {
        parent->parent = greatGrandparent;
        greatGrandparent->right == grandparent ?
                greatGrandparent->right = parent : greatGrandparent->left = parent;
    }
}

template <class T>
void RBT<T>::rotateLeftRight(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent){
    singleRotateLeft(x,parent,grandparent);
    rotateLeftLeft(parent,x,grandparent);
}

template <class T>
void RBT<T>::rotateRightLeft(rbtNode<T> *x, rbtNode<T> *parent, rbtNode<T> *grandparent){
    singleRotateRight(x,parent,grandparent);
    rotateRightRight(parent,x,grandparent);
}


template <class T>
void RBT<T>::recolor(rbtNode<T> *x) {
        x->parent->color = 0;
        rbtNode<T> *uncle = findUncle(x);
        if(uncle)
            uncle->color = 0;
        rbtNode<T> *grandparent = findGrandparent(x);
        if(grandparent) {
            grandparent->color = 1;
            if(root == grandparent)
                grandparent->color = 0;
        }
        fixTree(grandparent); // move up tree
}

template <class T>
void RBT<T>::swapColors(rbtNode<T> *a, rbtNode<T> *b) {
    bool temp = a->color; a->color = b->color; b->color = temp;
}

template <class T>
bool RBT<T>::remove(T key) {

}

template <class T>
bool RBT<T>::search(T key) {
    rbtNode<T> *current = searchForNodePrivate(key);
    if(current)
        return true;
    return false;
}

template <class T>
bool RBT<T>::isEmpty() {
    if(!root)
        return true;
    return false;
}

template <class T>
rbtNode<T>*RBT<T>::createLeaf(T data) {
    rbtNode<T>* newNode = new rbtNode<T>;
    newNode->data   = data;
    newNode->left   = nullptr;
    newNode->right  = nullptr;
    newNode->parent = nullptr;
    newNode->color  = 1;
    return newNode;
}

template <class T>
rbtNode<T>*RBT<T>::searchForNodePrivate(T key) {
    if(this->isEmpty())
        return nullptr;
    rbtNode<T> *curr = this->root;
    bool found = false;
    while(curr && !found){
        if(key > curr->data)
            curr = curr->right;
        else if(key < curr->data)
            curr = curr->left;
        else
            found = true;
    }
    if(found)
        return curr;
    return nullptr;
}

// finds grandparent of a node
template <class T>
rbtNode<T>* RBT<T>::findGrandparent(rbtNode<T> *node) {
    if(node->parent)
            return node->parent->parent;
    return nullptr;
}

// finds uncle of a node
template <class T>
rbtNode<T>* RBT<T>::findUncle(rbtNode<T> *node) {
    rbtNode<T> *g = findGrandparent(node);
    if(!g)
        return nullptr;
    if(node->parent == g->left)
        return g->right;
    else
        return g->left;
}

template <class T>
void RBT<T>::bfs() {
    if(!isEmpty()) {
        std::queue<rbtNode<T> *> queue;
        queue.push(root);
        while (!queue.empty()) {
            rbtNode<T> *temp = queue.front();
            queue.pop();

            if (temp->left)
                queue.push(temp->left);
            if (temp->right)
                queue.push(temp->right);

            std::cout << temp->data;
            temp->color == 0 ? std::cout << "(black) " : std::cout << "(red) ";
        }
    }
}


// Drawing functions
template <class T>
void RBT<T>::DrawTree() {
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    DrawTreePrivate(root, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);

}

template <class T>
int RBT<T>::DrawTreePrivate(rbtNode<T> *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->color);

    int left  = DrawTreePrivate(tree->left,  1, offset,                depth + 1, s);
    int right = DrawTreePrivate(tree->right, 0, offset + left + width, depth + 1, s);


    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }

    return left + width + right;
}




#endif //BSTPRACTICE_RBT_H
