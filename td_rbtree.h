#ifndef TD_RBTREE_H
#define TD_RBTREE_H

#include<tuple>

//top-down RB tree
//算法来自<数据结构与算法分析c++描述>
//插入和删除都是使用递归来做的,因此更容易理解些
//特别是删除的实现,感觉代码非常古怪,还有一个我非常不喜欢想把它换掉的地方.
//Especially the implementation of deletion, I feel that the code is very strange, and there is a place I don't like very much and want to change it

//Hello everyone, I have implement the red black tree recently, but I think there are some mistakes. I hope you people can give some advice

class TD_RBtree
{
public:
    TD_RBtree();
    ~TD_RBtree();
    void insert(int data_);
    void remove(int data_);
    void print() const{print(fake_root->right_chl);}
    bool check()const
    {
        int number{};
        if(fake_root->right_chl->color==Color::Black)
            return check(fake_root->right_chl,number);
        return false;
    }
    void recolor(int i=0);
private:
    enum class Color{Red,Black};

    struct Node
    {
        int d{};
        Node * left_chl{nullptr};
        Node * right_chl{nullptr};
        Color color{Color::Red};
//        Node()
    };

    Node * fake_root{nullptr};
    Node * null_node{nullptr};
private:
    void rotate_with_left(Node * &root_);
    void rotate_with_right(Node * &root_);

    template<typename ...T>
    using BContainer=std::tuple<T * &...>;
    using I_Container=BContainer<Node,Node,Node,Node>;

    void rotate_color_fix(I_Container c_);

    void insert(I_Container c,int data_);

    using R_Container=BContainer<Node,Node,Node>;/*std::tuple<Node * &,Node * &,Node * &,bool>*/;

    void remove(R_Container c,int data_);

    enum Which{Max,Min};
    template<Which W>
    constexpr Node * find(Node * const & root_)const;

    void print(Node * const & root_) const;

    bool check(Node * const & root_,int & number_)const;
};

template<TD_RBtree::Which W>
constexpr TD_RBtree::Node *TD_RBtree::find(Node * const &root_) const
{
    auto temp=root_;
    //因为null_node的左右节点是自身，不用害怕没有左右节点
//    if(temp==null_node)
//        return temp;
    if constexpr(W==Min){
        while (temp->left_chl!=null_node)
            temp=temp->left_chl;
    }else if constexpr(W==Max){
        while (temp->right_chl!=null_node)
            temp=temp->right_chl;
    }
    return temp;
}

#endif // TD_RBTREE_H
