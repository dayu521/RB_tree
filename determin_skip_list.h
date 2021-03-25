#ifndef DETERMINSKIPLIST_H
#define DETERMINSKIPLIST_H
#include<iterator>

namespace SDS {
}
struct BaseListNode
{
    BaseListNode * next_;
    BaseListNode * down_;
    int data_;
};

struct ListNode : BaseListNode
{
    BaseListNode * pre_;
};

struct ListIterator
{
    typedef std::ptrdiff_t				difference_type;
    typedef std::bidirectional_iterator_tag	iterator_category;
    typedef int				value_type;
    typedef int*				pointer;
    typedef int&				reference;

    ListIterator(BaseListNode * p):p_(p){}
    ListIterator & operator ++()
    {
        p_=p_->next_;
        return *this;
    }

    ListIterator operator ++(int )
    {
        ListIterator temp(*this);
        p_=p_->next_;
        return temp;
    }

    reference operator *()const
    {
        return p_->data_;
    }

    pointer operator ->()const
    {
        return &p_->data_;
    }
    //重载&有很大麻烦

    ListIterator & operator --()
    {
        p_=static_cast<ListNode *>(p_)->pre_;
        return *this;
    }

    ListIterator operator --(int )
    {
        ListIterator temp(*this);
        p_=static_cast<ListNode *>(p_)->pre_;
        return temp;
    }

    friend inline bool operator ==(const ListIterator &a,const ListIterator &b)
    {
        return a.p_==b.p_;
    }

private:
    BaseListNode * p_;
};
struct ListIteratorConst
{
    typedef ptrdiff_t				difference_type;
    typedef std::bidirectional_iterator_tag	iterator_category;
    typedef int				value_type;
    typedef const int*			pointer;
    typedef const int&			reference;
};
//1-2-3确定性跳跃表
class DeterminSkipList
{
public:
    DeterminSkipList();
    void insert(int data);
    void remove(int data);
    using iterator=ListIterator;
    using const_iterator=ListIteratorConst;
    iterator begin();
    const_iterator begin()const;
    iterator end();
    const_iterator end()const;
private:
    BaseListNode * top_head_;
    ListNode * head_;
    ListNode * tail_;
    BaseListNode * bottom_;
};


#endif // DETERMINSKIPLIST_H
