#include <iostream>

//#include <loki/Visitor.h>

//#include <thread>
#include<signal.h>
#include<unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "td_rbtree.h"
#include "dt_rbtree.h"

using namespace std;

int main()
{
    TD_RBtree t1;
    DT_RBtree t2;
//    int k[]={5,4,3,6,1};
    auto test=[](auto & tree){
        int k[]={10,85,15,70,20,60,30,50,65,80,90,40,5,55};
        for(int i=0;i<sizeof (k)/sizeof (int);i++)
            tree.insert(k[i]);
        tree.print();
        cout<<endl;
//        for(int i=0;i<sizeof(k)/sizeof (int);i++)
//            tree.remove(k[i]);
        tree.remove(65);
        tree.print();
        cout<<endl;
    };
    test(t1);
    test(t2);
    return 0;
}
