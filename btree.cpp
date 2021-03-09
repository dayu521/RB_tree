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

//假设sub_root拥有的key大于0
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


bool BTree::remove(BTree::BNodePointer &sub_root, int data_)
{
    auto key_id=key_idx(sub_root,data_);
    if(key_id==-1){//不在当前节点
        if(sub_root->is_leaf)
            return true;
    }else{//在当前节点
        if(sub_root->is_leaf){
            key_remove(sub_root,key_id);
            return true;
        }else{
            if((sub_root->child_[key_id]->n_of_key==BNodeTrait<>::DEGREE-1)&&
                    (sub_root->child_[key_id]->n_of_key==BNodeTrait<>::DEGREE-1)){

            }

        }
    }
}

int BTree::key_idx(BNodePointer node_,int data_) const
{
    auto j=-1,i=0;
    while (i<node_->n_of_key) {
        if(data_!=node_->v_[i]){
            i++;
            continue;
        }
        j=i;
        break;
    }
    return j;
}

void BTree::key_remove(BNodePointer node_,int idx)
{
    auto last_index=node_->n_of_key-1;
    auto i=idx;
    while (i<last_index) {
        node_->v_[i]=node_->v_[i+1];
        i++;
    }
    node_->n_of_key--;
}
void BTree::child_remove(BNodePointer node_, int idx_)
{
    auto last_index=node_->n_of_key;
    auto i=idx_;
    while (i<last_index) {
        node_->child_[i]=node_->child_[i+1];
        i++;
    }
}

void BTree::coalesce_node(BNodePointer parent_node_, int key_idx_)
{
    auto left=parent_node_->child_[key_idx_];
    auto right=parent_node_->child_[key_idx_+1];
    auto mid_val=parent_node_->v_[key_idx_];
    //先处理父节点
    key_remove(parent_node_,key_idx_);
    child_remove(parent_node_,key_idx_+1);
    //处理当前节点
    left->v_[BNodeTrait<>::DEGREE-1]=mid_val;
    auto i=0;
    while (i<BNodeTrait<>::DEGREE-1) {
        left->v_[BNodeTrait<>::DEGREE+i]=right->v_[i];
        i++;
    }
    i=0;
    while (i<BNodeTrait<>::DEGREE) {
        left->child_[i+BNodeTrait<>::DEGREE]=right->child_[i];
    }
    left->n_of_key=2*BNodeTrait<>::DEGREE-1;
    delete right;
}



int BTree::find_min(BNodePointer sub_root) const
{
    auto i=sub_root;
    while (!i->is_leaf) {
        i=i->child_[0];//所有节点必有关键字，访问第一个关键字是安全的
    }
    return i->v_[0];
}

int BTree::find_max(BNodePointer sub_root) const
{
    auto i=sub_root;
    while (!i->is_leaf) {
        i=i->child_[i->n_of_key-1];
    }
    return i->v_[0];
}
