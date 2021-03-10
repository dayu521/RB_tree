#ifndef BTREE_H
#define BTREE_H
#include <iostream>

namespace  {
//Degree,we can think of it as the least half of child node
inline constexpr int DEGREE=2;
}
/*btree node structure
 *     k0  k1  k2  k3  k4
 *   c0  c1  c2  c3  c4  c5
 * key索引总对应它之前的child索引
 * 总key=DEGREE*2-1
 * 总child=DEGREE*2
 * key以非降序排列
 * 一个key必定只能处于两个child之间，但不是每个child都处于两个key之间(可以看到，c0和c5是列外情况)
 */

//impletement three functions,insert(),remove() and print() only
//这个实现倾向条理清晰，但没有效率
class BTree
{
public:
    BTree();
    ~BTree();
    auto insert(int data)->bool;
    bool remove(int data);
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
        BTree().split_full_child(nb,0);
    }
public:
    //节点的key的数量为k,代表就有k+1个孩子.
    template<int N=DEGREE>
    struct BNode
    {
        int v_[2*N-1]{0};       //array for keys
        unsigned char n_of_key{0};  //key sizes
        bool is_leaf{false};
        BNode * child_[2*N]{nullptr};   //array for child
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

private:
    void print(BNodePointer sub_root)const;
    bool insert(BNodePointer & sub_root, int data);
    bool remove(BNodePointer & sub_root,int data_);
    void dealloc_tree(BNodePointer & sub_root); //incorrect!
    void split_full_child(BNodePointer sub_root,int child_idx);
    unsigned int find_child_index(BNodePointer sub_root,int data);
//    void key_insert(BNodePointer sub_root,idx);
    void key_insert_on(BNodePointer sub_root,int idx,int data);

    //for remove
private:
    int key_idx(BNodePointer node_,int data_) const;
    void key_remove_on(BNodePointer node_,int idx_);
    void child_remove_on(BNodePointer node_,int idx_);
    //合并idx_左右两个孩子
    void coalesce_node(BNodePointer parent_node_,int idx_);
    int find_min(BNodePointer sub_root)const;
    int find_max(BNodePointer sub_root)const;
//It's useless for the time!
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
