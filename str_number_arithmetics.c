#include "str_number.h"

//ARITHMETICS:
int base_not_eq(STRINT* a, STRINT* b){
    if (a->BASE_ != b->BASE_){ 
        printf("ERROR: only add numbers of same base! %c != %c", a->BASE_+'0', b->BASE_+'0');
        return 1;
    }
    return 0;
}

int identical(STRINT* left, STRINT* right){
    return left->HEAD_ == right->HEAD_;
}

int equal(STRINT* left, STRINT* right)
{
    if (identical(left,right))
        return 1;
    if (length(left) != length(right))
        return 0;
    STRINT_ITERATOR* left_it = make_bw_iterator(left);
    STRINT_ITERATOR* right_it = make_bw_iterator(right);
    iterator_bw(left_it);
    iterator_bw(right_it);
    int mark = 1;
    while (!it_eq(left_it,left->Begin))
    {
        if (it_value(left_it) != it_value(right_it))
        {
            mark = 0;
            break;
        }
        iterator_bw(left_it);
        iterator_bw(right_it);
    }
    if (it_value(left_it) != it_value(right_it))
        mark = 0;
    
    free((void*)left_it);
    free((void*)right_it);
    return mark;
}

STRINT_ITERATOR* target_setup(STRINT_ITERATOR* a_it, STRINT_ITERATOR* b_it, STRINT* target){
    if (identical(a_it->mom, target)){
        return a_it;
    }
    else if (identical(b_it->mom, target)){
        return b_it;
    }
    //target is a new strint innitiated to 0
    target->BASE_ = a_it->mom->BASE_;//we need to common base
    target->TAIL_LENGTH_ = 0;//decrease from 1 to 0
    STRINT_ITERATOR* t_it = make_fw_iterator(target);
    return t_it;
}

int add(STRINT* a, STRINT* b, STRINT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    STRINT_ITERATOR* a_it = make_fw_iterator(a);
    STRINT_ITERATOR* b_it = make_fw_iterator(b);
    STRINT_ITERATOR* t_it = target_setup(a_it, b_it, target);

    char overflow = 0;//overflow tmp
    int a_cont = 1;
    int b_cont = 1;
    while (a_cont && b_cont)
    {
        //printf("%d + %d + %d\n", *a_it->data_it, *b_it->data_it, overflow);
        char sum = *a_it->data_it + *b_it->data_it + overflow;
        //printf("= %d\n",sum);
        insert(t_it, sum % a->BASE_);
        overflow = sum / a->BASE_;
        if (!it_eq(a_it,t_it) && !it_eq(b_it,t_it)) iterator_fw(t_it);
        a_cont = (iterator_fw(a_it) && !it_eq(a_it,a->End));
        b_cont = (iterator_fw(b_it) && !it_eq(b_it,b->End));
    }
    while (a_cont){
        insert(t_it, *a_it->data_it + overflow);
        overflow = 0;
        if (!it_eq(a_it, t_it)) iterator_fw(t_it);
        a_cont = (iterator_fw(a_it) && !it_eq(a_it,a->End));
    }
    while (b_cont){
        insert(t_it, *b_it->data_it + overflow);
        overflow = 0;
        if (!it_eq(b_it, t_it)) iterator_fw(t_it);
        b_cont = (iterator_fw(b_it) && !it_eq(b_it,b->End));
    }
    if (overflow){
        insert(t_it, 1);
    }
    //deallocation
    if (!it_eq(a_it, t_it) && !it_eq(b_it, t_it))
        free((void*)t_it);
    free((void*)a_it);
    free((void*)b_it);
    return 0;
}

int subtract(STRINT* a, STRINT* b, STRINT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    STRINT_ITERATOR* a_it = make_fw_iterator(a);
    STRINT_ITERATOR* b_it = make_fw_iterator(b);
    STRINT_ITERATOR* t_it = target_setup(a_it, b_it, target);
#ifdef DEBUG
    printf("subtraction target after setup:\n");
    print_strint(target, stdout);
    printf("\n");
#endif
    char overflow = 0;//overflow tmp
    int a_cont = 1;
    int b_cont = 1;
    char sum = 0; 
    while (a_cont && b_cont)
    {
#ifdef DEBUG
        printf("%d - %d = ", *a_it->data_it , *b_it->data_it + overflow);
#endif
        if (*a_it->data_it >= *b_it->data_it + overflow) //same as add but minus
        {    
            sum = *a_it->data_it - *b_it->data_it - overflow;
            overflow = 0;
        }
        else //we need to add base to a
        {
            sum = *a_it->data_it + a->BASE_ - *b_it->data_it - overflow; 
            overflow = 1;
        }
#ifdef DEBUG
        printf("%d\n", sum);
#endif
        insert(t_it, sum);
        if (!it_eq(a_it,t_it) && !it_eq(b_it,t_it)) iterator_fw(t_it);
        a_cont = (iterator_fw(a_it) && !it_eq(a_it,a->End));
        b_cont = (iterator_fw(b_it) && !it_eq(b_it,b->End));
    }
    while (a_cont && overflow){//the main complication is that the a digit can be 0 -> base - overflow
        if (*a_it->data_it < overflow) 
        {
            sum = a->BASE_ - overflow;
        }
        else sum = *a_it->data_it - overflow;
        
        insert(t_it, sum);
        overflow = 0;
        iterator_fw(t_it);
        a_cont = (iterator_fw(a_it) && !it_eq(a_it,a->End));
    }
    while (b_cont){//a digit is always 0 and overflow is always 1
        insert(t_it, a->BASE_ - *b_it->data_it - overflow);//t_it doesn't move
        if (!it_eq(b_it, t_it))
            iterator_fw(t_it);
        overflow = 1;
        b_cont = iterator_fw(b_it);
    }
    if (overflow){
        insert(t_it, a->BASE_ - 1);
        iterator_fw(t_it);
    }
    //we need to mark the End correctly by backtracking to last non-0 
#ifdef DEBUG
    printf("target before cutting: ");
    print_strint(target,stdout);
    printf("\n");
    //printf("t_it = %d\n", *t_it->data_it);
#endif
    if (it_eq(t_it, target->End))
        iterator_bw(t_it);
    while (*t_it->data_it == 0)
    { 
#ifdef DEBUG
        printf("cutting: %d\n", *t_it->data_it);
#endif
        iterator_bw(t_it);
        if (t_it->part_it->PART_NUMBER < target->TAIL_->PART_NUMBER)
        {
            free((void*)target->TAIL_);
            target->TAIL_ = t_it->part_it;
            target->TOTAL_PARTS_--;
        }
    }
    //printf("t_it = %d\n", *t_it->data_it);
    target->LAST_ = t_it->data_it;
    //printf("target->LAST_ = %d\n", *target->LAST_);
    iterator_fw(t_it);
    free((void*)target->End);
    target->End = t_it;
    
#ifdef DEBUG
    printf("target after cutting: ");
    print_strint(target,stdout);
    printf("\n");
    //printf("Where is the END and LAST?\n");
    //printf("End: %d LAST: %d\n",*target->End->data_it,*target->LAST_);
    //iterator_bw(t_it);
    //printf("t_it: %d\n", *t_it->data_it);
    //deallocation:
    //if (!it_eq(a_it, t_it) && !it_eq(b_it, t_it))
    //    free((void*)t_it);
    //printf("all done, let's free memory\n");
#endif
    free((void*)a_it);
    free((void*)b_it);
    //printf("memory freed\n");
    return 0;
}

int mult(STRINT* a, STRINT* b, STRINT* target)
{
    if (base_not_eq(a,b) || base_not_eq(a,target)) 
        return 1;
    //forward iterators for a,b
    STRINT* t;
    if (identical(a,target) || identical(b,target))
        t = new_strint(target->BASE_,target->PARTSZ_);
    else
        t = target;
    STRINT_ITERATOR* a_it = make_fw_iterator(a);
    STRINT_ITERATOR* b_it = make_fw_iterator(b);
    //we can't use a or b as targets even if they are intended as such
    STRINT_ITERATOR* t_it = make_fw_iterator(t);
    STRINT_ITERATOR* start_p = make_fw_iterator(t);
    char h = 0;
    while (it_l(b_it,b->End))
    {
        char overfl = 0;
        point_it(t_it,start_p);
        while (it_l(a_it, a->End))
        {
            //multiply and add to t
            if (it_eq(t_it,target->End))
            {
                append(t,0);
                t_it->data_it = t->LAST_;
                t_it->part_it = t->TAIL_;
            }
            h = it_value(t_it)+it_value(a_it)*it_value(b_it)+overfl;
            overfl = h / target->BASE_;
            h %= target->BASE_;
            set_it_value(t_it,h);
            iterator_fw(a_it);
            iterator_fw(t_it);
        }
        if (overfl)
        {
            append(t,overfl);
        }
        it_reset(a_it);
        iterator_fw(start_p);
        iterator_fw(b_it);
    }
    if (identical(a,target) || identical(b,target))
    {
        deleteSTRINT(target);
        target = t;
    }
    return 0;
}

/*
void convert(STRINT* num, char new_base)
{
    
}
*/