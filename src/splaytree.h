#ifndef SPLAYTREE_H
#define SPLAYTREE_H

template <class T,typename Less_xy>
class SplayTree;


template <class T>
struct BinaryNode
{
    T  element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode( ) : left( NULL ), right( NULL ) { }
    BinaryNode( const T & theElement, BinaryNode *lt, BinaryNode *rt )
      : element( theElement ), left( lt ), right( rt ) { }

    // friend class SplayTree<T,Less_xy<T> >;
};


template <class T,typename Less_xy>
class SplayTree
{
  public:
    explicit SplayTree();
    void printTree( ) ;
    std::vector<T> toArray;
    void insert( const T & x,Less_xy less_xy );
  private:
    BinaryNode<T> *root;
    BinaryNode<T> *nullNode;

    void printTree( BinaryNode<T> *t );

    void rotateWithLeftChild( BinaryNode<T> * & k2 ) const;
    void rotateWithRightChild( BinaryNode<T> * & k1 ) const;
    void splay( const T & x, BinaryNode<T> * & t,Less_xy less_xy ) const;
};


template <class T,typename Less_xy>
SplayTree<T,Less_xy>::SplayTree(){
    nullNode = new BinaryNode<T>;
    nullNode->left = nullNode->right = nullNode;
    //nullNode->element = notFound;
    root = nullNode;
}


template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::insert( const T & x,Less_xy less_xy )
{
    static BinaryNode<T> *newNode = NULL;

    if( newNode == NULL )
        newNode = new BinaryNode<T>;
    newNode->element = x;

    if( root == nullNode )
    {
        newNode->left = newNode->right = nullNode;
        root = newNode;
    }
    else
    {
        splay( x, root,less_xy );
        if(less_xy(x,root->element))
        {
            newNode->left = root->left;
            newNode->right = root;
            root->left = nullNode;
            root = newNode;
        }
        else
        if(less_xy(root->element, x))
        {
            newNode->right = root->right;
            newNode->left = root;
            root->right = nullNode;
            root = newNode;
        }
        else
            return;
    }
    newNode = NULL;   // So next insert will call new
}


template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::printTree( )
{
    printTree( root );
}

template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::splay( const T & x,
                                   BinaryNode<T> * & t,Less_xy less_xy ) const
{
    BinaryNode<T> *leftTreeMax, *rightTreeMin;
    static BinaryNode<T> header;

    header.left = header.right = nullNode;
    leftTreeMax = rightTreeMin = &header;

    nullNode->element = x;   // Guarantee a match

    for( ; ; )
        if(less_xy(x,t->element))
        {
            if( less_xy(x,t->left->element))
                rotateWithLeftChild( t );
            if( t->left == nullNode )
                break;
            // Link Right
            rightTreeMin->left = t;
            rightTreeMin = t;
            t = t->left;
        }
        else if(less_xy(t->element,x) )
        {
            if( less_xy(t->right->element,x) )
                rotateWithRightChild( t );
            if( t->right == nullNode )
                break;
            // Link Left
            leftTreeMax->right = t;
            leftTreeMax = t;
            t = t->right;
        }
        else
            break;

    leftTreeMax->right = t->left;
    rightTreeMin->left = t->right;
    t->left = header.right;
    t->right = header.left;
}

/**
 * Rotate binary tree node with left child.
 */
template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::rotateWithLeftChild( BinaryNode<T> * & k2 ) const
{
    BinaryNode<T> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 */
template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::rotateWithRightChild( BinaryNode<T> * & k1 ) const
{
    BinaryNode<T> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
}


template <class T,typename Less_xy>
void SplayTree<T,Less_xy>::printTree( BinaryNode<T> *t )
{
    if( t != t->left )
    {
        printTree( t->left );
        toArray.push_back(t->element);
        printTree( t->right );
    }
}

#endif // SPLAYTREE_H
