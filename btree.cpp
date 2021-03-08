#include "btree.h"
#include <iostream>

BTree::BTree()
{
    root_=new BNode<>{.is_leaf=true};
}

BTree::~BTree()
{
//    dealloc_tree(root_);
}

auto BTree::insert(int data)->bool
{
    if(root_->n_of_key==BNodeTrait<>::KeySize){
        auto new_root=new BNode{};
        new_root->child_[0]=root_;
        root_=new_root;
        split(new_root,0);//当前root只有一个左子树，且没有key
    }
    return insert(root_,data);
}

//sub_root拥有的key大于0
void BTree::print(BNodePointer sub_root) const
{
    using std::cout,std::endl;

    if(!sub_root->is_leaf){
        auto i=0u;
        for(;i<sub_root->n_of_key;i++){
            print(sub_root->child_[i]);
            cout<<sub_root->v_[i]<<" ";
        }
        print(sub_root->child_[i]);
    }else {
        auto i=0u;
        for(;i<sub_root->n_of_key;i++)
            cout<<sub_root->v_[i]<<" ";
    }
}
//暂时不处理相等键情况，默认都插入
bool BTree::insert(BTree::BNodePointer &sub_root, int data)
{
    auto i=find_index(sub_root,data);
    if(sub_root->is_leaf){
        if(data==sub_root->v_[i])
            return false;
        auto j=sub_root->n_of_key;
        while (j>i) {
            sub_root->v_[j]=sub_root->v_[j-1];
            j--;
        }
        sub_root->v_[i]=data;
        sub_root->n_of_key++;
        return true;
    }
    if(sub_root->child_[i]->n_of_key==BNodeTrait<>::n_of_keys){
        split(sub_root,i);
        if(sub_root->v_[i]<data)
            i++;
    }
    return insert(sub_root->child_[i],data);
}

void BTree::dealloc_tree(BTree::BNodePointer &sub_root)
{
    if(sub_root!=BNodeTrait<>::Null){
//        dealloc_tree(sub_root->child_[0]);
//        dealloc_tree(sub_root->child_[1]);
//        dealloc_tree(sub_root->child_[2]);
//        dealloc_tree(sub_root->child_[3]);
        write_repeat_call(sub_root->child_);
        delete sub_root;
        sub_root=BNodeTrait<>::Null;
    }
}

void BTree::split(BTree::BNodePointer sr, int child_idx)
{
    auto x=sr;
    auto y=x->child_[child_idx];//The node to be split
    auto z=new BNode<>{.n_of_key=BNodeTrait<>::DEGREE-1,.is_leaf=y->is_leaf};//The new right node
    //Deal with node z first
    auto i=0u;
    while (i<=BNodeTrait<>::DEGREE-2) {
        z->v_[i]=y->v_[i+BNodeTrait<>::DEGREE];   //Copy key
        i++;
    }
    if(y->is_leaf==false){
        i=0;
        while (i<=BNodeTrait<>::DEGREE-1) {
            z->child_[i]=y->child_[i+BNodeTrait<>::DEGREE]; //Copy child
            i++;
        }
    }
    y->n_of_key=BNodeTrait<>::DEGREE-1;
    //Then parent
    i=child_idx;
    decltype (i) max_v_idx=x->n_of_key;
    while (max_v_idx>i) {
        x->v_[max_v_idx]=x->v_[max_v_idx-1];
        max_v_idx--;
    }
    x->v_[max_v_idx]=y->v_[BNodeTrait<>::DEGREE-1];
    decltype (i) max_c_idx=x->n_of_key+1;
    while (max_c_idx>i+1) {
         x->child_[max_c_idx]=x->child_[max_c_idx-1];
         max_c_idx--;
    }
    x->child_[max_c_idx]=z;
    x->n_of_key++;
}

//we don't call this function when sub_root is full
//return proper index of child node
unsigned int BTree::find_index(BTree::BNodePointer sub_root, int data)
{
    auto i=0u;
    while (i<sub_root->n_of_key&&sub_root->v_[i]<data) {
        i++;
    }
    if(i==sub_root->n_of_key)
        return i;
    return i;
}

