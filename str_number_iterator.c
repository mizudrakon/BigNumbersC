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
    if (it->data_it == it->mom->LAST_)
    {
        it->part_it = it->mom->End->part_it;
        it->data_it = it->mom->End->data_it;
        return 1;
    }
    if (it_eq(it,it->mom->End)) {

#ifdef DEBUG
        printf("ERROR: forward iterator called on end\n");
#endif
        return 0;
    }
    it->data_it++;
    if (it->data_it == it->part_it->DATA + it->mom->PARTSZ_)
    {
        it->part_it = it->part_it->NEXT;
        it->data_it = it->part_it->DATA;
#ifdef DEBUG
        putc('|',stdout);
#endif
        return 1;
    }
    return 1; //return true 
}

//#define DEBUG
int iterator_bw(STR_INT_ITERATOR* it)
{
    if (it->part_it == it->mom->HEAD_ && it->data_it == it->mom->HEAD_->DATA)
    { 
#ifdef DEBUG
        printf("ERROR: backward iterator called on begin\n");
#endif
        return 0;
    }
    it->data_it--;
    if (it->data_it < it->part_it->DATA)
    {
        if (it->part_it == it->mom->HEAD_) return 0;
        it->part_it = it->part_it->PREV;
        it->data_it = it->part_it->DATA + it->mom->PARTSZ_ - 1;
#ifdef DEBUG
        putc('|',stdout);
#endif
    }
    return 1;
}
//#undef DEBUG
int next(STR_INT_ITERATOR* it)
{
    if (it->direction_fw)
    {
#ifdef DEBUG
        printf("forward iterator next called\n");
#endif
        iterator_fw(it);
    }
    else
        iterator_bw(it);
    return 1;
}

char value(STR_INT_ITERATOR* it)
{
    return *(it->data_it);
}

int it_eq(const STR_INT_ITERATOR* a, const STR_INT_ITERATOR* b)
{
    return a->data_it == b->data_it;
}

int it_l(const STR_INT_ITERATOR* left, const STR_INT_ITERATOR* right)
{
    if (left->mom != right->mom)
    {
        printf("\nComparing incompatible iterators!!!\n");
        return 0;
    }
    if (left->part_it->PART_NUMBER > right->part_it->PART_NUMBER)
        return 0;
    else if (left->part_it == right->part_it)
    {
        return left->data_it < right->data_it;
    }
    return 1;
}

int it_leq(const STR_INT_ITERATOR* left, const STR_INT_ITERATOR* right)
{
    return it_eq(left, right) || it_l(left, right);
}

//#define DEBUG
void swap_digit(STR_INT_ITERATOR* left, STR_INT_ITERATOR* right)
{
#ifdef DEBUG
    printf("swapping %c and %c\n", to_symbol(*(left->data_it)), to_symbol(*(right->data_it)));
#endif
    char help = *(left->data_it);
    *(left->data_it) = *(right->data_it);
    *(right->data_it) = help;
}
//$ITERATOR
