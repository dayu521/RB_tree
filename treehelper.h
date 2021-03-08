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

#endif // TREEHELPER_H
