#include "str_number.h"

//^ITERATOR 
//make forward iterator
STR_INT_ITERATOR* make_fw_iterator(STR_INT* mom)
{
    STR_INT_ITERATOR* it = (STR_INT_ITERATOR*) malloc(sizeof(STR_INT_ITERATOR));
    it->mom = mom;
    it->part_it = mom->HEAD_;
    it->data_it = mom->HEAD_->DATA;
    it->direction_fw = 1;
    return it;
}

STR_INT_ITERATOR* make_bw_iterator(STR_INT* mom)
{
    STR_INT_ITERATOR* it = (STR_INT_ITERATOR*) malloc(sizeof(STR_INT_ITERATOR));
    it->mom = mom;
    it->part_it = mom->TAIL_;
    it->data_it = mom->LAST_+1;
    it->direction_fw = 0;
    return it;
}

int iterator_fw(STR_INT_ITERATOR* it)
{
    if (it->data_it == it->mom->LAST_ || it_eq(it,it->mom->End)) {
        //printf("ERROR: forward iterator called on end\n");
        return 0;
    }
    it->data_it++;
    if (it->data_it == it->part_it->DATA + it->mom->PARTSZ_)
    {
        it->part_it = it->part_it->NEXT;
        it->data_it = it->part_it->DATA;
        return 1;
    }
    return 1; //return true 
}

int iterator_bw(STR_INT_ITERATOR* it)
{
    if (it->part_it == it->mom->HEAD_ && it->data_it == it->mom->HEAD_->DATA)
    { 
        //printf("ERROR: backward iterator called on begin\n");
        return 0;
    }
    it->data_it--;
    if (it->data_it < it->part_it->DATA)
    {
        if (it->part_it == it->mom->HEAD_) return 0;
        it->part_it = it->part_it->PREV;
        it->data_it = it->part_it->DATA + it->mom->PARTSZ_ - 1;
    }
    return 1;
}

int next(STR_INT_ITERATOR* it)
{
    if (it->direction_fw)
    {
        //printf("forward iterator next called\n");
        iterator_fw(it);
    }
    else
        iterator_bw(it);
    return 1;
}

int it_eq(const STR_INT_ITERATOR* a, const STR_INT_ITERATOR* b)
{
    return a->data_it == b->data_it;
}


void swap(STR_INT_ITERATOR* left, STR_INT_ITERATOR* right)
{
    printf("swapping %c and %c\n", to_symbol(*(left->data_it)), to_symbol(*(right->data_it)));
    char help = *(left->data_it);
    *(left->data_it) = *(right->data_it);
    *(right->data_it) = help;
}
//$ITERATOR
