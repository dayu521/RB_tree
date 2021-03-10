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
        split_full_child(new_root,0);//当前root只有一个左子树，且没有key
    }
    return insert(root_,data);
}

bool BTree::remove(int data)
{
    //Coalesce root_ first,if necessary
    if(key_idx(root_,data)==-1&&
            root_->n_of_key==1&&
            root_->child_[0]->n_of_key==BNodeTrait<>::DEGREE-1&&
            root_->child_[1]->n_of_key==BNodeTrait<>::DEGREE-1){
        auto new_root=root_->child_[0];
        coalesce_node(root_,0);
        delete root_;
        root_=new_root;
    }
    return remove(root_,data);
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
    auto i=find_child_index(sub_root,data);
    if(sub_root->is_leaf){
        if(data==sub_root->v_[i])
            return false;
        key_insert_on(sub_root,i,data);
        return true;
    }
    if(sub_root->child_[i]->n_of_key==BNodeTrait<>::n_of_keys){
        split_full_child(sub_root,i);
        if(sub_root->v_[i]<data)
            i++;
    }
    return insert(sub_root->child_[i],data);
}

//incorrect!
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

void BTree::split_full_child(BNodePointer sub_root, int child_idx)
{
    auto x=sub_root;
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
unsigned int BTree::find_child_index(BTree::BNodePointer sub_root, int data)
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
        auto chl_idx=find_child_index(sub_root,data_);
        if(sub_root->child_[chl_idx]->n_of_key>BNodeTrait<>::DEGREE-1){
            return remove(sub_root->child_[chl_idx],data_);
        }else{
            auto to_down=sub_root->child_[chl_idx];
            if(chl_idx==0){
                auto right_sibling=sub_root->child_[chl_idx+1];
                if(right_sibling->n_of_key==BNodeTrait<>::DEGREE-1){
                    coalesce_node(sub_root,chl_idx);
                    return remove(to_down,data_);
                }else{
                    //deal with to_down
                    key_insert_on(to_down,to_down->n_of_key-1,sub_root->v_[chl_idx]);
                    to_down->child_[to_down->n_of_key]=right_sibling->child_[0];
                    //deal with sub_root
                    sub_root->v_[chl_idx]=right_sibling->v_[0];
                    //deal with right_sibling
                    key_remove_on(right_sibling,0);
                    child_remove_on(right_sibling,0);
                    right_sibling->n_of_key--;
                    //此处和剩下几处不做这样的优化:即直接下降到节点的对应孩子,
                    //因为to_down可能是孩子节点,还要再多写一步处理
                    return remove(to_down,data_);
                }
            }else if(chl_idx==sub_root->n_of_key-1){
                auto left_sibling=sub_root->child_[chl_idx-1];
                if(left_sibling->n_of_key>BNodeTrait<>::DEGREE-1){
                    coalesce_node(sub_root,chl_idx-1);
                    return remove(to_down,data_);
                }else {
                    //deal with to_down
                    key_insert_on(to_down,0,sub_root->v_[chl_idx-1]);
                    to_down->child_[0]=left_sibling->child_[left_sibling->n_of_key-1];
                    //deal with sub_root
                    sub_root->v_[chl_idx-1]=left_sibling->v_[left_sibling->n_of_key-1];
                    //deal with left_sibling
                    key_remove_on(left_sibling,left_sibling->n_of_key-1);
                    child_remove_on(left_sibling,left_sibling->n_of_key);
                    left_sibling->n_of_key--;
                    return remove(to_down,data_);
                }
            }else{
                auto left_sibling=sub_root->child_[chl_idx-1];
                auto right_sibling=sub_root->child_[chl_idx+1];
                if((left_sibling->n_of_key==BNodeTrait<>::DEGREE-1)&&
                        (right_sibling->n_of_key==BNodeTrait<>::DEGREE-1)){
                    //与前一个兄弟合并
                    coalesce_node(sub_root,chl_idx-1);
                    return remove(left_sibling,data_);
                }else if(left_sibling->n_of_key>BNodeTrait<>::DEGREE-1){
                    //deal with to_down
                    key_insert_on(to_down,0,sub_root->v_[chl_idx-1]);
                    to_down->child_[0]=left_sibling->child_[left_sibling->n_of_key-1];
                    //deal with sub_root
                    sub_root->v_[chl_idx-1]=left_sibling->v_[left_sibling->n_of_key-1];
                    //deal with left_sibling
                    key_remove_on(left_sibling,left_sibling->n_of_key-1);
                    child_remove_on(left_sibling,left_sibling->n_of_key);
                    left_sibling->n_of_key--;
                    return remove(to_down,data_);
                }else{
                    //deal with to_down
                    key_insert_on(to_down,to_down->n_of_key-1,sub_root->v_[chl_idx]);
                    to_down->child_[to_down->n_of_key]=right_sibling->child_[0];
                    //deal with sub_root
                    sub_root->v_[chl_idx]=right_sibling->v_[0];
                    //deal with right_sibling
                    key_remove_on(right_sibling,0);
                    child_remove_on(right_sibling,0);
                    right_sibling->n_of_key--;
                    return remove(to_down,data_);
                }
            }
        }
    }else{//在当前节点
        if(sub_root->is_leaf){
            key_remove_on(sub_root,key_id);
            sub_root->n_of_key--;
            return true;
        }else{
            auto left_child=sub_root->child_[key_id];
            auto right_child=sub_root->child_[key_id+1];
            if((left_child->n_of_key==BNodeTrait<>::DEGREE-1)&&
                    (right_child->n_of_key==BNodeTrait<>::DEGREE-1)){
                coalesce_node(sub_root,key_id);
//                left可能是叶子节点，所以没有孩子,减少一次函数调用
                if(left_child->is_leaf){
                    key_remove_on(left_child,BNodeTrait<>::DEGREE-1);
                    left_child->n_of_key--;
                    return true;
                }else
                    return remove(left_child,data_);
            }else if(left_child->n_of_key>BNodeTrait<>::DEGREE-1){
                auto max=find_max(left_child);
                sub_root->v_[key_id]=max;
                return remove(left_child,max);
            }else{
                auto min=find_max(right_child);
                sub_root->v_[key_id]=min;
                return remove(right_child,min);
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

void BTree::key_remove_on(BNodePointer node_,int idx)
{
    auto last_index=node_->n_of_key-1;
    auto i=idx;
    while (i<last_index) {
        node_->v_[i]=node_->v_[i+1];
        i++;
    }
//    node_->n_of_key--;
}
void BTree::child_remove_on(BNodePointer node_, int idx_)
{
    auto last_index=node_->n_of_key;
    auto i=idx_;
    while (i<last_index) {
        node_->child_[i]=node_->child_[i+1];
        i++;
    }
}

//The key_idx is between left node and right node
void BTree::coalesce_node(BNodePointer parent_node_, int key_idx_)
{
    auto left=parent_node_->child_[key_idx_];
    auto right=parent_node_->child_[key_idx_+1];
    auto mid_val=parent_node_->v_[key_idx_];
    //先处理父节点
    key_remove_on(parent_node_,key_idx_);
    child_remove_on(parent_node_,key_idx_+1);
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
        i++;
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
    return i->v_[i->n_of_key-1];
}

//把从idx以后的key(包括idx)都依次向后移动，然后在idx处插入
void BTree::key_insert_on(BNodePointer sub_root, int idx, int data)
{
    auto j=sub_root->n_of_key;
    while (j>idx) {
        sub_root->v_[j]=sub_root->v_[j-1];
        j--;
    }
    sub_root->v_[idx]=data; //在idx处插入
    sub_root->n_of_key++;
}
