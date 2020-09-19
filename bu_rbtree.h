#ifndef RBDATA_H
#define RBDATA_H

//bottom-up RB tree
//算法来自算法导论
//自底向上.完全按照算法导论的伪码形式来实现的

class BU_RBtree
{
public:
    BU_RBtree();
    ~BU_RBtree();
    void insert(int );
    void remove(int );
    void print() const;
    bool check()const
    {
        int number{};
        if(root->color==Color::Black)
            return check(root,number);
        return false;
    }
private:
    enum Color :int { Red, Black };

    template <typename T>
    class Node {
    public:
        T item;
        Node<T> *parent;
        Node<T> *left;
        Node<T> *right;
        Color color;

        Node(const T &_item, Node<T> *_parent = nullptr, Node<T> *_left = nullptr,
             Node<T> *_right = nullptr, Color _Color = Red)
            : item(_item),
              parent(_parent),
              left(_left),
              right(_right),
              color(_Color) {}
    };

    Node<int> * root;        //红黑树根节点
    Node<int> * NIL;     //红黑树的哨兵节点

private:
    void print(Node<int> *const  & root_) const;

    void emptyTree(Node<int> * &root);
    void rotationWithLeftChild(Node<int> *&root);
    void rotationWithRightChild(Node<int> *&root);

    void insertionFixUpOfDoubleRed(Node<int> *root);

    void removeFixUpOfLostOfBlack(Node<int> *root);
    void replace(Node<int> *y, Node<int> *x);
    Node<int> * findMinValueNode(const Node<int> * root);
    Node<int> *&getParentReference(Node<int> *child) {
        if (child->parent == NIL) return this->root;
        return child == child->parent->left ? child->parent->left
                                            : child->parent->right;
    }
    bool check(Node<int> * const & root_,int & number_)const;
};

#endif // RBDATA_H
