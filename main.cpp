#include <iostream>
#include <vector>
#include <functional>

//#include <loki/Functor.h>

//#include <thread>
//#include<signal.h>
//#include<unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

#include "td_rbtree.h"
#include "bu_rbtree.h"
#include "btree.h"
using namespace std;

void test_btree();
void test_rbtree();

class Sx
{
public:
    virtual void fuck(){cout<<"Sx:fuck you baby!"<<endl;}
    void fuck(int ){}
    void print_a() const{cout<<"a:"<<a<<endl;}
    int a=3;
};

class Sxx:public Sx
{
public:
    void fuck(){cout<<"Sxx:fuck you baby!"<<endl;}
};

template<typename F>
struct LL;

template<typename F>
struct LL<F ()>
{

};
LL<int (*)()> ln={};

int main()
{
//    test_btree();
//    test_rbtree();
    Sxx *s;
//    s->fuck(5);
    return 0;
}

void test_rbtree()
{
    TD_RBtree t1;
    BU_RBtree t2;
    srand(time(nullptr));
    int ii=50;
    std::vector<int> a(ii);
    for(int i=0;i<ii;i++)
        a[i]=rand()%50;
//    a={4,7,3,9,1,6,8,5,2,0};
    for(int i=0;i<a.size();i++)
        cout<<a[i]<<" ";
    cout<<endl;
    auto test=[ii](auto & tree,vector<int> kk={})->std::vector<int>{
//        int k[]={10,85,15,70,20,60,30,50,65,80,90,40,5,55};
//        for(int i=0;i<sizeof (k)/sizeof (int);i++)
//            tree.insert(k[i]);
        for(int i=0;i<ii;i++)
            tree.insert(kk[i]);
        tree.print();
        cout<<endl;
//        for(int i=0;i<sizeof(k)/sizeof (int);i++)
//            tree.remove(k[i]);
//        tree.remove(65);
//        tree.print();
//        cout<<endl;

//        tree.recolor(3);
        if(tree.check())
            cout<<"合法"<<endl;
        else
            cout<<"不合法"<<endl;
        return {};
    };
    test(t1,a);//==test(t2,a);
//    test(t2,a);
}
void test_btree()
{
    BTree b;
    int a[]{2,8,3,66,28,94,31,51,90,0,38,71};
    for(auto i:a){
        if(b.insert(i))
            continue;
        break;
    }
    b.print();
    cout<<endl;
//    b.remove(94);
    for (auto i:{2,66,28}){
        b.remove(i);
        b.print();
        cout<<endl;
    }
}
