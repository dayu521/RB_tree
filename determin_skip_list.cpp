#include "determin_skip_list.h"

DeterminSkipList::DeterminSkipList()
{
    bottom_=new BaseListNode{};
    bottom_->down_=bottom_->next_=bottom_;

    head_=new ListNode{{.down_=bottom_},nullptr};
    tail_=new ListNode{{.next_=head_,.down_=bottom_},head_};
    head_->next_=head_->pre_=tail_;

    top_head_=new BaseListNode{.next_=tail_,.down_=head_};
}

void DeterminSkipList::insert(int data)
{
    head_->data_=tail_->data_=bottom_->data_=data;
    auto i=top_head_->down_;
    while (i!=bottom_) {
        auto j=i;
        while (data>j->data_) {
            j=j->next_;
        }
        if(j->down_->next_->next_->data_<j->data_){
            BaseListNode * temp{};
            if(j->down_==bottom_)
                temp=new ListNode{};
            else
                temp=new BaseListNode{};
        }
    }
}

DeterminSkipList::iterator DeterminSkipList::begin()
{
    return ListIterator{head_->next_};
}

DeterminSkipList::const_iterator DeterminSkipList::begin() const
{
    return ListIteratorConst{};
}

DeterminSkipList::iterator DeterminSkipList::end()
{
    return ListIterator{tail_};
}

DeterminSkipList::const_iterator DeterminSkipList::end() const
{
    return ListIteratorConst{};
}
