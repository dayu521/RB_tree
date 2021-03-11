#ifndef TREEHELPER_H
#define TREEHELPER_H
#include <iostream>
#include <stack>

template<typename NodeType>
void print_m(NodeType sub_root)
{
    using std::cout,std::endl,std::stack;
    stack<NodeType> st{};
    st.push(sub_root);
    while (true) {
        auto sNode=st.top();
        while (sNode!=nullptr) {
            st.push(sNode->left_);
            sNode=st.top();
        }
        st.pop();
        if(st.size()==0)
            break ;
        sNode=st.top();
        st.pop();
        cout<<sNode->data_<<" ";
        st.push(sNode->right_);
    }
}

//试验用的玩具- -
namespace Fake {

    struct Node
    {
        Node * left_;
        Node * right_;
        int a_;
    };

    enum Which{Min,Max};

    template<Which W,typename T=Node>
    struct Fx;

    template<>
    struct Fx<Min>
    {
        static auto next(Node * n)
        {
            return n->left_;
        }
    };

    template<>
    struct Fx<Max>
    {
        static auto next(Node * n)
        {
            return n->right_;
        }
    };

    template<Which W>
    auto next(auto n);

    template<>
    inline auto next<Min>(Node * n)
    {
        return n->left_;
    }
    template<>
    inline auto next<Max>(Node * n)
    {
        return n->right_;
    }

    template<Which W>
    inline Node * h()
    {
        Node * temp;
        while (next<W>(temp)!=nullptr)
            temp=next<W>(temp);
        return temp;
    }

//    auto min=h<Min>();
//    auto max=h<Max>();
}

#endif // TREEHELPER_H

