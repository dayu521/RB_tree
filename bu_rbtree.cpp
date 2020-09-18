#include "bu_rbtree.h"
#include<iostream>

BU_RBtree::BU_RBtree()
{
    root=NIL=new Node<int>(-9998);
    NIL->color=Black;
    NIL->parent=NIL->left=NIL->right=NIL;
}

BU_RBtree::~BU_RBtree()
{
    emptyTree(root);
    delete NIL;
}

void BU_RBtree::emptyTree(Node<int> *&root)
{
    if(root!=NIL)
    {
        emptyTree(root->left);
        emptyTree(root->right);
        delete root;
        root=nullptr;
    }
}

void BU_RBtree::rotationWithLeftChild(Node<int> *&root)
{
    Node<int> *left_child = root->left;

    root->left = left_child->right;
    left_child->right->parent = root;

    left_child->right = root;
    left_child->parent = root->parent;

    root->parent = left_child;
    root = left_child;
}

void BU_RBtree::rotationWithRightChild(Node<int> *&root)
{
    Node<int> *right_child = root->right;

    root->right = right_child->left;
    right_child->left->parent = root;

    right_child->left = root;
    right_child->parent = root->parent;

    root->parent = right_child;
    root = right_child;
}

void BU_RBtree::insert(int x)
{
    auto temp=root;
    //红黑树在调整时,其哨兵节点NIL的父节点在旋转过程中会改变,因此,在树进行一轮清空后,
    //此时root=NIL,即root.parent不再是NIL.所以这里不可以写成root->parent
    auto tempParent=NIL;
    while (temp!=NIL) {
        tempParent=temp;
        if(temp->item>x){
            temp=temp->left;
        }
        else if(temp->item<x){
            temp=temp->right;
        }
        else
            break;
    }
    if(temp!=NIL)
        return ;
    auto newNode=new Node<int>(x,tempParent,NIL,NIL);
    if(tempParent==NIL){
        root=newNode;
    }
    else {
        if(x<tempParent->item){
            tempParent->left=newNode;
        }
        else{
            tempParent->right=newNode;
        }
    }
    insertionFixUpOfDoubleRed(newNode);
    return ;
}

void BU_RBtree::insertionFixUpOfDoubleRed(Node<int> *root)
{
    while (root->parent->color == Red) {
        if (root->parent == root->parent->parent->left) {
            if (root->parent->parent->right->color == Red) {  //case 1
                root->parent->color = Black;
                root->parent->parent->right->color = Black;
                root->parent->parent->color = Red;
                root = root->parent->parent;
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->right) {   //case 2
                    root = root->parent;
                    rotationWithRightChild(getParentReference(root));
                }
                rotationWithLeftChild(gp);    //case 3
                gp->color = Black;
                gp->right->color = Red;
                break;
            }
        } else {
            if (root->parent->parent->left->color == Red) {
                root->parent->color = Black;
                root->parent->parent->left->color = Black;
                root->parent->parent->color = Red;
                root = root->parent->parent;
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->left) {
                    root = root->parent;
                    rotationWithLeftChild(getParentReference(root));
                }
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                break;
            }
        }
    }
    this->root->color = Black;
}

void BU_RBtree::remove(int v)
{
    auto temp=root;
    while (temp!=NIL) {
        if(temp->item>v){
            temp=temp->left;
        }
        else if(temp->item<v){
            temp=temp->right;
        }
        else
            break;
    }
    if(temp==NIL){
        return ;
    }
    auto y=temp;
    auto yOriginalColor=y->color;
    Node<int> * x=nullptr;
    if(temp->left==NIL){
        x=temp->right;
        replace(y,x);

    }else if(temp->right==NIL){
        x=temp->left;
        replace(y,y->left);
    }else{
        auto minNode=findMinValueNode(temp->right);
        //这里使用一种简单的方式,保留源节点,仅仅替换值的方式间接删除找到的节点
        y->item=minNode->item;
        y=minNode;
        x=y->right;
        yOriginalColor=y->color;
        if(y->parent==temp)
            x->parent=y;//算法导论中这几行没看明白,暂时把形式保存在这里好了.
        //喔,调试源码偶然发现了,这一步当x为哨兵节点是很重要,因为哨兵节点也参与之后调整过程
        //向上回溯时,需要赋值x=x.parent.哨兵节点父节点每次都会变化,所以这里必须强制更新
        //以防之后修复时，NIL节点的父节点指向其他节点.
        //这里虽然采用删除方式不同,但还是保留了算法导论中的结构
        else {
            ;
        }
        replace(y,y->right);
    }
    delete y;
    y=nullptr;
    if(yOriginalColor==Black)
        removeFixUpOfLostOfBlack(x);
}

void BU_RBtree::print() const
{
   print(root);
}

void BU_RBtree::removeFixUpOfLostOfBlack(Node<int> *root)
{
    while (root->color == Black && root != this->root) {
        Node<int> *root_brother = nullptr;
        if (root == root->parent->left) {
            root_brother = root->parent->right;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                root_brother = root->parent->right;
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                root = root->parent;
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->right->color == Black) {	//case 3
                    rotationWithLeftChild(root_parent->right);
                    root_parent->right->color = Black;
                    root_parent->right->right->color = Red;
                    root_brother=root_parent->right;
                }
                rotationWithRightChild(root_parent);	//case 4
                root_parent->color = root_parent->left->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                //利用枚举默认值也是和我们的规定值相等,否则下面要判断
                root = this->root;
            }
        } else {
            root_brother = root->parent->left;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                rotationWithLeftChild(gp);
                gp->color = Black;
                gp->right->color = Red;
                root_brother = root->parent->left;
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                root = root->parent;
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->left->color == Black) {	//case 3
                    rotationWithRightChild(root_parent->left);
                    root_parent->left->color = Black;
                    root_parent->left->left->color = Red;
                }
                rotationWithLeftChild(root_parent);	//case 4
                root_parent->color = root_parent->right->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                root = this->root;
            }
        }
    }
    root->color = Black;
}

void BU_RBtree::replace(Node<int> *y, Node<int> *x)
{
    if(y==root)
        root=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;
    x->parent=y->parent;
}

BU_RBtree::Node<int> *BU_RBtree::findMinValueNode(const Node<int> *root)
{
    auto temp=root;
    while (temp->left!=NIL) {
        temp=temp->left;
    }
    return const_cast<Node<int> *>(temp);
}

void BU_RBtree::print(Node<int> * const &root_) const
{
    if(root_!=NIL){
        print(root_->left);
        std::cout<<root_->item<<(root_->color==Color::Red?'r':'b')<<" ";
        print(root_->right);
    }
}
