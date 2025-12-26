#include "str_number.h"

//ARITHMETICS:
int base_not_eq(STR_INT* a, STR_INT* b){
    if (a->BASE_ != b->BASE_){ 
        printf("ERROR: only add numbers of same base! %c != %c", a->BASE_+'0', b->BASE_+'0');
        return 1;
    }
    return 0;
}

int identical(STR_INT* left, STR_INT* right){
    return left->HEAD_ == right->HEAD_;
}

int equal(STR_INT* left, STR_INT* right)
{
    if (identical(left,right))
        return 1;
    STR_INT_ITERATOR* left_it = make_fw_iterator(left);
    STR_INT_ITERATOR* right_it = make_fw_iterator(right);
    int mark = 1;
    while (it_eq(left_it,left->End))
    {
        if (left_it->data_it != right_it->data_it || it_eq(right_it,right->End))
        {
            mark = 0;
            break;
        }
        iterator_fw(left_it);
        iterator_fw(right_it);
    }
    if (it_eq(right_it, right->End))
        mark = 0;
    
    free((void*)left_it);
    free((void*)right_it);
    return mark;
}

STR_INT_ITERATOR* target_setup(STR_INT_ITERATOR* a_it, STR_INT_ITERATOR* b_it, STR_INT* target){
    if (identical(a_it->mom, target)){
        return a_it;
    }
    else if (identical(b_it->mom, target)){
        return b_it;
    }
    //target is a new str_int innitiated to 0
    target->BASE_ = a_it->mom->BASE_;//we need to common base
    target->TAIL_LENGTH_ = 0;//decrease from 1 to 0
    STR_INT_ITERATOR* t_it = make_fw_iterator(target);
    return t_it;
}

int add(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    STR_INT_ITERATOR* a_it = make_fw_iterator(a);
    STR_INT_ITERATOR* b_it = make_fw_iterator(b);
    STR_INT_ITERATOR* t_it = target_setup(a_it, b_it, target);

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

int subtract(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    STR_INT_ITERATOR* a_it = make_fw_iterator(a);
    STR_INT_ITERATOR* b_it = make_fw_iterator(b);
    STR_INT_ITERATOR* t_it = target_setup(a_it, b_it, target);
#ifdef DEBUG
    printf("subtraction target after setup:\n");
    print_str_int(target, stdout);
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
    print_str_int(target,stdout);
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
    print_str_int(target,stdout);
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

int mult(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (base_not_eq(a,b)) 
        return 1;
    //forward iterators for a,b
    STR_INT_ITERATOR* a_it = make_fw_iterator(a);
    STR_INT_ITERATOR* b_it = make_fw_iterator(b);
    //we can't use a or b as targets even if they are intended as such
    STR_INT* t_it = new_str_int(target->BASE_,target->PARTSZ_);
    STR_INT* start_p = new_str_int(target->BASE_,target->PARTSZ_);
    
    while (it_l(a_it,a->End))
    {
        while (it_l(b_it, b->End))
        {
            //multiply and add to t
        }
        it_reset(b_it);
        iterator_fw(start_p);
        //set_it(t_it,start_p); need to define
    }
    return 0;
}

/*
void convert(STR_INT* num, char new_base)
{
    
}
*/