#ifndef BTREE_H
#define BTREE_H
#include <iostream>

namespace  {
//Degree,we can think of it as the least half of child node
inline constexpr int DEGREE=2;
}

class BTree
{
public:
    BTree();
    ~BTree();
    auto insert(int data)->bool;
    decltype(auto) remove(int data){return remove(root_,data);}
    void print()const
    {
        if(root_->n_of_key>0)
            return print(root_);
    }
public:
    static void test_find_indx()
    {
        BNodePointer b=new BNode<>{.v_{4,8,12},.n_of_key=3};
        BNodePointer nb=new BNode<>{.n_of_key=0,.child_{b}};
        BTree().split(nb,0);
    }
public:
    //节点的key的数量为k,代表就有k+1个孩子.
    template<int N=DEGREE>
    struct BNode
    {
        int v_[2*N-1]{0};
        unsigned char n_of_key{0};
        bool is_leaf{false};
        BNode * child_[2*N]{nullptr};
        static constexpr auto  DEGREE=N;
        static constexpr auto  extent_v=std::extent_v<decltype (v_)>;
    };

    template<typename T=BNode<>>
    struct BNodeTrait
    {
        static constexpr auto Null=nullptr;
        static constexpr auto n_of_keys=T::extent_v;
        static constexpr auto DEGREE=T::DEGREE;
        static constexpr auto KeySize=n_of_keys;
    };
    BNode<> * root_{nullptr};
    using BNodePointer=decltype (root_);

//private:
public:
    void print(BNodePointer sub_root)const;
    bool insert(BNodePointer & sub_root, int data);
    bool remove(BNodePointer & sub_root,int data_);
    void dealloc_tree(BNodePointer & sub_root);
    void split(BNodePointer sr,int child_idx);
    unsigned int find_index(BNodePointer sub_root,int data);
public:
    template<int N=BNodeTrait<>::n_of_keys>
    inline void write_repeat_print(const BNodePointer & sub_root)const
    {
        write_repeat_print<N-1>(sub_root);
        std::cout<<sub_root->v_[N]<<" ";
        print(sub_root->child_[N+1]);
    }
    template< int N>
    inline void write_repeat_call( BNode<> * (&v)[N])
    {
        do_call<N>(v);
    }
    template<int I>
    inline void do_call(BNode<> * (&v)[])
    {
        do_call<I-1>(v);
        dealloc_tree(v[I]);
    }
};
template<>
inline void BTree::write_repeat_print<0>(const BNodePointer & sub_root)const
{
    print(sub_root);
}

template<>
inline void BTree::do_call<0>(BNode<> * (&v)[])
{
    dealloc_tree(v[0]);
}

#endif // BTREE_H
