#include "str_number.h"

/*appEND_ is special because it pushes num->END_ further, and that's why it needs to call new_si_part*/
int append(STR_INT* num, char digit)
{
    //we need to enlarge num
    if (num->End->data_it == num->TAIL_->DATA + num->PARTSZ_)
    {
        if (new_si_part(num)){
            fprintf(stderr, "new part creation failed!\n");
            return 1;
        }
        num->End->part_it = num->TAIL_;
        num->End->data_it = num->TAIL_->DATA;
        num->LAST_ = num->End->data_it;
        num->TAIL_LENGTH_ = 1;//we start from 0, the previous parts are all full
    }
    //num size is fine
    else 
    {
        num->TAIL_LENGTH_++;
        num->LAST_++;
    }
    //insert digit and move END_ further
    *(num->LAST_) = digit;
    num->End->data_it++;
    return 0;
}

int insert(STR_INT_ITERATOR* num_it, char digit)
{
    if (it_eq(num_it,num_it->mom->End)) {
        append(num_it->mom, digit);
        //END_ was push, possibly into an entirely different area in memory
        //-> END_ previous END_ might be just after the last part DATA chunk
        num_it->data_it = num_it->mom->LAST_;
        return 0;
    }
    //or insert
    *num_it->data_it = digit;
    return 0;
}

void formated_print_str_int(STR_INT* num, FILE* f, char brk, size_t line_len)//prints a number string to chosen output
{
    size_t charCount = 0;
    STR_INT_ITERATOR* bw_it = make_bw_iterator(num);
    while (iterator_bw(bw_it))
    {
        putc(to_symbol(*(bw_it->data_it)),f);
        charCount++;
        if (brk != 0 && charCount == line_len){
            putc(brk,f);
            charCount = 0;
        }
    }
    putc(brk,f);
    free((void*)bw_it);
}

//BACKWARD PRINTING is actually just straightforward printing, because we keep the DATA backward
void backward_print_str_int(STR_INT* num, FILE* f, char brk, size_t line_len)
{
    int fin = 0;
    size_t charCount = 0;
    for (STR_INT_PART* part_it = num->HEAD_; part_it != NULL; part_it = part_it->NEXT)
    {   
        if (fin) break;
        for (char* data_it = part_it->DATA; data_it < part_it->DATA + num->PARTSZ_; data_it++)
        {
            if (part_it == num->TAIL_ && data_it == num->LAST_ + 1)
            {    
                fin = 1;
                break;
            }
            putc(to_symbol(*data_it),f);
            charCount++;
            if (brk != 0 && charCount == line_len){
                putc(brk,f);
                charCount = 0;
            }
        }
    }
    putc(brk,f);
}

void print_str_int(STR_INT* num, FILE* f)
{
    formated_print_str_int(num, f, 0, 0);
}


//READ NUBMER FROM INPUT:
int read_num(STR_INT* num, FILE* f)
{
    printf("reading STR_INT\n");
    char c;
    num->TAIL_LENGTH_ = 0;
    num->LAST_ = num->HEAD_->DATA - 1;
    //we need to allow only numbers < base
    //IGNORE white spaces or any possibly separating symbols in front
    c = getc(f);
    c = to_cnum(c);
    printf("looking for a number!\n");
    while (c == 0 || !is_cnum_digit(c, num))//ignore leading zeros
    {
        printf("is %c a digit? %d\n", to_symbol(c), num->BASE_);
        c = getc(f);
        if (c == '$') 
        {
            printf("escape character before number input");
            return 1;//$ is escape character
        }
        c = to_cnum(c);
    }
    printf("found a number and reading...\n");
    //READING THE NUMBER:
    //STR_INT_PART* part_it = num->HEAD_;
    //char* data_it = num->HEAD_->DATA; //iterator
    while (is_cnum_digit(c,num))
    {
        append(num, c);
        c = getc(f);
        c = to_cnum(c);
    }
    putc('\n',stdout);
    print_str_int(num, stdout);
    putc('\n',stdout);
    //HERE we mirror the elements of the linked arrays of char, so that every number has its lowest digit on 1
#define MIRROR
#ifdef MIRROR
    STR_INT_ITERATOR* fw_it = make_fw_iterator(num);
    //STR_INT_ITERATOR* bw_it = make_bw_iterator(num);
    //MIRRORING loop:
    printf("End is: %c\n",to_symbol(*(num->End->data_it-1)));
    printf("testing iterators:\n");
    printf("%c\n",to_symbol(*(fw_it->data_it)));
    printf("1. forward iterator and End:\n");
    if (it_eq(fw_it,num->Begin))
        printf("fw_it is recognized as equal to Begin\n");
    else
        printf("fw_it is NOT recognized as equal to Begin\n");
    printf("number part is %ld digits long\n", num->PARTSZ_);
    printf("number has %ld parts\n", num->TOTAL_PARTS_);
    printf("tail-length is %ld\n", num->TAIL_LENGTH_);
    printf("number has %ld digits\n", length(num));
    while (!it_eq(fw_it,num->End))
    {
        printf("%c\n",to_symbol(*(fw_it->data_it)));
        next(fw_it);
    }
    printf("loop done\n");
    if (it_eq(fw_it,num->End))
        printf("fw_it is recognized as equal to End\n");
    else
        printf("fw_it is NOT recognized as equal to End\n");
    
    printf("2. backward iterator");
    STR_INT_ITERATOR* bw_it = make_bw_iterator(num);
    while (!it_eq(bw_it,num->Begin))
    {
        printf("%c\n",to_symbol(*(bw_it->data_it)));
        next(bw_it);
    }
    printf("%c\n",to_symbol(*(bw_it->data_it)));
    next(bw_it);
    printf("%c\n",to_symbol(*(bw_it->data_it)));
    /*
    while (!it_eq(fw_it,num->End))
    {
        printf("%c ",to_symbol(*(fw_it->data_it)));
        next(fw_it);
    }
    putc('\n',stdout);
    */
    /*
    printf("Now mirroring!\n");
    while (!it_eq(fw_it,bw_it))
    {
        swap(fw_it, bw_it);
        next(fw_it);
        next(bw_it);
    }
    */
    free((void*)fw_it);
#endif
    //free((void*)bw_it);
    return 0;//zero errors
}

/*
int mark(char* num, char base)
{
    char* num_it = num;
    while (is_cnum_digit(*num_it,base)) num_it++;
    *num_it = '\0';
    return 0;
}
*/
