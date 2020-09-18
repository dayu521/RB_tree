#include "td_rbtree.h"
#include<iostream>
using namespace std;

TD_RBtree::TD_RBtree()
{
    null_node=new Node{-9999};  //只是为了调试时好分辨这个是空节点
    null_node->color=Color::Black;
    null_node->left_chl=null_node->right_chl=null_node;
    fake_root=new Node{-9998,null_node,null_node,Color::Black};     //同理，这个是假根节点
}

TD_RBtree::~TD_RBtree()
{
    delete null_node;
    delete  fake_root;
}

void TD_RBtree::insert(int data_)
{
    insert({fake_root->right_chl,fake_root,null_node},data_);
    fake_root->right_chl->color=Color::Black;
}

void TD_RBtree::remove(int data_)
{
    fake_root->color=Color::Red;
    remove({fake_root->right_chl,fake_root->left_chl,fake_root},data_);
    fake_root->right_chl->color=Color::Black;
}

void TD_RBtree::rotate_with_left(TD_RBtree::Node *&root_)
{
    Node * left=root_->left_chl;
    root_->left_chl=left->right_chl;
    left->right_chl=root_;
    root_=left;
}

void TD_RBtree::rotate_with_right(TD_RBtree::Node *&root_)
{
    Node * right=root_->right_chl;
    root_->right_chl=right->left_chl;
    right->left_chl=root_;
    root_=right;
}

void TD_RBtree::rotate_color_fix(I_Container c_)
{
    auto &[x,xp,xpp]=c_;
    if(xp->color==Color::Red){
        if(xpp->left_chl==xp){
            if(xp->left_chl!=x)
                rotate_with_right(xp);
            rotate_with_left(xpp);
            xpp->color=Color::Black;
            xpp->right_chl->color=Color::Red;
        }else{
            if(xp->right_chl!=x)
                rotate_with_left(xp);
            rotate_with_right(xpp);
            xpp->color=Color::Black;
            xpp->left_chl->color=Color::Red;
        }
    }
}

void TD_RBtree::insert(TD_RBtree::I_Container c_, int data_)
{
    auto &[x,xp,xpp]=c_;
     if(x==null_node){
        x=new Node{data_,null_node,null_node};
        if(x!=fake_root->right_chl)
            rotate_color_fix(c_);
    }else{
        if(x->left_chl->color==Color::Red && x->right_chl->color==Color::Red){
            x!=fake_root->right_chl?x->color=Color::Red:x->color=Color::Black;
            x->left_chl->color=x->right_chl->color=Color::Black;
            rotate_color_fix(c_);
        }
        if(data_ < x->d){
            insert({x->left_chl,x,xp},data_);
        }else if(data_ > x->d){
            insert({x->right_chl,x,xp},data_);
        }else
            ;
    }
}

void TD_RBtree::remove(R_Container c_, int data_)
{
    auto & [x,t,xp]=c_;
    if(x==null_node)
        return;
    if(x->color==Color::Red){       //单单这个分支真的，实现太烂了，太菜，想不到好方法
        //不需要处理，直接到下一步
        if(data_ < x->d){
            remove({x->left_chl,x->right_chl,x},data_);
        }else if(data_ >x->d){
            remove({x->right_chl,x->left_chl,x},data_);
        }else{
            if(x->left_chl==null_node&&x->right_chl==null_node){
                auto old=x;
                x=null_node;
                delete old;
            }else if(x->right_chl!=null_node){
                auto & min=find<Min>(x->right_chl);
                x->d=min->d;
                remove({x->right_chl,x->left_chl,x},min->d);
            }else{
                auto & max=find<Max>(x->left_chl);
                x->d=max->d;
                remove({x->left_chl,x->right_chl,x},max->d);
            }
        }
    }else if(x->left_chl->color==Color::Black&&x->right_chl->color==Color::Black){        //x有双黑孩子
        if(t->left_chl->color==Color::Black&&t->right_chl->color==Color::Black){        //t有双黑孩子
            x->color=Color::Red;
            t->color=Color::Red;
            xp->color=null_node->color=Color::Black;
        }else if(xp->right_chl==t){
            if(t->right_chl->color==Color::Black){
                rotate_with_left(t);
                rotate_with_right(xp);
                x->color=Color::Red;
                xp->left_chl->color=Color::Black;
            }else{
                rotate_with_right(xp);
                xp->color=Color::Red;
                xp->left_chl->color=xp->right_chl->color=Color::Black;
                x->color=Color::Red;
            }
        }else{
            if(t->left_chl->color==Color::Black){
                rotate_with_right(t);
                rotate_with_left(xp);
                x->color=Color::Red;
                xp->left_chl->color=Color::Black;
            }else{
                rotate_with_left(xp);
                xp->color=Color::Red;
                xp->left_chl->color=xp->right_chl->color=Color::Black;
                x->color=Color::Red;
            }
        }
        //处理结束，开始向对应子节点进行
        if(data_ < x->d){
            remove({x->left_chl,x->right_chl,x},data_);
        }else if(data_ >x->d){
            remove({x->right_chl,x->left_chl,x},data_);
        }else{
            if(x->left_chl==null_node&&x->right_chl==null_node){
                auto old=x;
                x=null_node;
                delete old;
            }else if(x->right_chl!=null_node){
                auto & min=find<Min>(x->right_chl);
                x->d=min->d;
                remove({x->right_chl,x->left_chl,x},min->d);
            }else{
                auto & max=find<Max>(x->left_chl);
                x->d=max->d;
                remove({x->left_chl,x->right_chl,x},max->d);
            }
        }
    }else {
        if(data_ <x->d){
            if(x->left_chl->color==Color::Black){
                auto new_t=x->right_chl;
                auto new_x=x;
                rotate_with_right(x);
                new_x->color=Color::Red;
                new_t->color=Color::Black;
                remove({x->left_chl,x->right_chl,new_t->left_chl},data_);
            }else
                remove({x->left_chl,x->right_chl,x},data_);
        }else if(data_ > x->d ){
            if(x->right_chl->color==Color::Black){
                auto new_t=x->left_chl;
                auto new_x=x;
                rotate_with_left(x);
                new_t->color=Color::Black;
                new_x->color=Color::Red;
                remove({x->right_chl,x->left_chl,new_t->right_chl},data_);
            }else
                remove({x->right_chl,x->left_chl,x},data_);
        }else{
            //null_node是黑节点，因此，叶子节点不会到此处
            //            if(x->left_chl==null_node&&x->right_chl==null_node){
            //                auto old=x;
            //                x=null_node;
            //                delete old;
            //            }else
            //下面的x子节点一定落到红节点，所以不用调整，直接推进到子节点
            if(x->right_chl!=null_node){
                auto & min=find<Min>(x->right_chl);
                x->d=min->d;
                remove({x->right_chl,x->left_chl,x},min->d);
            }else{
                auto & max=find<Max>(x->left_chl);
                x->d=max->d;
                remove({x->left_chl,x->right_chl,x},max->d);
            }
        }
    }
}

void TD_RBtree::print(Node * const &root_) const
{
    if(root_!=null_node){
        print(root_->left_chl);
        std::cout<<root_->d<<(root_->color==Color::Red?'r':'b')<<" ";
        print(root_->right_chl);
    }
}
