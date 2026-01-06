#include "str_number.h"

/*appEND_ is special because it pushes num->END_ further, and that's why it needs to call new_si_part*/
int append(STRINT* num, char digit)
{
#ifdef DEBUG
    printf("appending %c\n", to_symbol(digit));
#endif
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

int insert(STRINT_ITERATOR* num_it, char digit)
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

int pop_back(STRINT*num)
{
    if (!iterator_bw(num->End) || length(num) == 1)
    {
        //printf("can't pop\n");
        return 0;
    }
    num->TAIL_LENGTH_--;
    if (num->End->data_it == num->TAIL_->DATA)
    {
        STRINT_PART* new_tail = num->TAIL_->PREV;
        free((void*)num->TAIL_);
        num->TAIL_ = new_tail;
        num->TOTAL_PARTS_--;
        num->TAIL_LENGTH_ = num->PARTSZ_;
        num->End->data_it = num->TAIL_->DATA + num->PARTSZ_;
        num->End->part_it = num->TAIL_;
        num->TAIL_->NEXT = NULL;
    }
    num->LAST_ = num->End->data_it - 1;
    return 1;
}

int shift_left(STRINT* num, size_t shift)
{
    for (size_t i = 1; i <= shift; i++)
        append(num, 0);
    STRINT_ITERATOR* it = make_bw_iterator(num);
    STRINT_ITERATOR* prev_it = make_bw_iterator(num);
    //bw iterators start pointing at End right after the appended 0, so extra move is necessary
    iterator_bw(prev_it);
    iterator_bw(it);
    for (size_t i = 1; i < shift; i++)
        iterator_bw(it);
    while (iterator_bw(it))
    {
        swap_digit(it,prev_it);
        iterator_bw(prev_it);
    }
    free((void*)it);
    free((void*)prev_it);
    return 1;
}

int shift_right(STRINT* num, size_t shift)
{
    if (length(num) == 1)
    {
        return 0;
    }

    STRINT_ITERATOR* it = make_fw_iterator(num);
    STRINT_ITERATOR* prev_it = make_fw_iterator(num);
    for (size_t i = 1; i <= shift; i++)
        iterator_fw(it);
    while (it_l(it,num->End))
    {
        swap_digit(it,prev_it);
        iterator_fw(prev_it);
        iterator_fw(it);
    }
    while (shift-- >= 1){
        pop_back(num);
    }
    free((void*)it);
    free((void*)prev_it);
    return 1;
}

void formated_print_strint(STRINT* num, FILE* f, char brk, size_t line_len)//prints a number string to chosen output
{
    size_t charCount = 0;
    STRINT_ITERATOR* bw_it = make_bw_iterator(num);
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
void backward_print_strint(STRINT* num, FILE* f, char brk, size_t line_len)
{
    int fin = 0;
    size_t charCount = 0;
    for (STRINT_PART* part_it = num->HEAD_; part_it != NULL; part_it = part_it->NEXT)
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

void print_strint(STRINT* num, FILE* f)
{
    formated_print_strint(num, f, 0, 0);
}


//READ NUBMER FROM INPUT:
int read_strint(STRINT* num, FILE* f)
{
    char c;
    num->TAIL_LENGTH_ = 0;
    num->LAST_ = num->HEAD_->DATA - 1;
    next(num->End);
    //we need to allow only numbers < base
    //IGNORE white spaces or any possibly separating symbols in front
    c = getc(f);
    c = to_cnum(c);
    while (c == 0 || !is_cnum_digit(c, num))//ignore leading zeros
    {
        c = getc(f);
        if (c == '$') 
        {
            printf("escape character before number input");
            return 1;//$ is escape character
        }
        c = to_cnum(c);
    }
    //READING THE NUMBER:
    while (is_cnum_digit(c,num))
    {
        append(num, c);
        c = getc(f);
        c = to_cnum(c);
    }
    //HERE we mirror the elements of the linked arrays of char, so that every number has its lowest digit on 1
#define MIRROR
#ifdef MIRROR
    STRINT_ITERATOR* fw_it = make_fw_iterator(num);
    STRINT_ITERATOR* bw_it = make_bw_iterator(num);
    next(bw_it); //bw iterator starts at the End = just after the last element
    while (it_l(fw_it,bw_it))
    {
        swap_digit(fw_it, bw_it);
        next(fw_it);
        next(bw_it);
    }
    free((void*)fw_it);
    free((void*)bw_it);
#endif
    return 0;//zero errors
}

int read_strint_string(STRINT* num, char* text)
{
    if (length(num) > 1)
    {
        reset_strint(num);
    }
    size_t size = 0;
    char *c = text;
    while (*c++ != '\0') size++;
    size--;
    *(num->HEAD_->DATA) = to_cnum(text[size--]);
    for (;size >= 1; size--)
    {
        append(num,to_cnum(text[size]));
    }
    append(num,to_cnum(*text));
    return 1;
}

int read_strint_file(STRINT* num, char* file_name)
{
    FILE* sourcef;
    int r = 0;
    if ((sourcef = fopen(file_name,"r")) == NULL)
    {
        printf("failed opening file\n");
        return 0;
    }
    if (read_strint(num, sourcef))
        r = 1;
    if (fclose(sourcef) == EOF)
    {
        printf("failed closing file\n");
        return 0;
    }
    return r;
}