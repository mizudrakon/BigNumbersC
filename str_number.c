#include "str_number.h"
//^CONSTRUCTOR
//this adds a new part to a specified mother, does it need to return a pointer?
int new_si_part(STR_INT* mom)
{
    STR_INT_PART* part;
    if ( (part = (STR_INT_PART*) malloc(sizeof(STR_INT_PART))) == NULL )
    {    printf("str_int_part basic malloc failed\n"); return 1;}
    part->MOTHER = mom;//assign mother
    if (mom->HEAD_ == NULL)//if mother doesn't have a HEAD_ -> part is HEAD_
    {
        mom->HEAD_ = part;
        part->PREV = NULL;
    }
    else 
    {
        part->PREV = mom->TAIL_;//new part's prev is mother's current tail
        mom->TAIL_->NEXT = part;//the part becomes the new tail
    }
    part->NEXT = NULL;
    mom->TAIL_ = part;
    mom->TOTAL_PARTS_++;//motehr now hes +1 part
    part->PART_NUMBER = mom->TOTAL_PARTS_;//the new part's number is the current total n/n
    if ((part->DATA = (char*)malloc(mom->PARTSZ_ * sizeof(char))) == NULL)//allocate storage
    {    printf("str_int_part data malloc failed\n"); return 1;}
    return 0;//no failure
}

STR_INT* new_str_int(char base, size_t part_len)
{
    STR_INT* strnum;
    if ((strnum = (STR_INT*) malloc(sizeof(STR_INT))) == NULL)//allocate struct
        printf("new_str_int malloc failed\n");
    strnum->PARTSZ_ = part_len;
    strnum->BASE_ = base;
    strnum->HEAD_ = NULL;
    //create the first part that is both HEAD_ and tail
    if (new_si_part(strnum))
    {
        fprintf(stderr, "failed creating a new string_int_part!");
        return NULL;
    }//no prev or next yet
    strnum->TAIL_ = strnum->HEAD_;
    //we want init as 0 to be safe-r...
    strnum->TAIL_LENGTH_ = 1;
    strnum->HEAD_->DATA[0] = 0;
    strnum->END_ = strnum->HEAD_->DATA+1;
    return strnum;
}

//cleaning STR_INT data
int deleteSTR_INT(STR_INT* corpse)
{
    STR_INT_PART* part_it = corpse->HEAD_;
    for (; part_it != NULL; part_it = part_it->NEXT)
    {
        free((void*)part_it->DATA);
        if (part_it->PREV!=NULL)
            free((void*)part_it->PREV);
    }
    free((void*)part_it);
    free((void*)corpse);
}
//$CONSTRUCTOR


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
    it->data_it = mom->END_;
    it->direction_fw = 0;
    return it;
}

int iterator_fw(STR_INT_ITERATOR* it)
{
    if (it->data_it == it->mom->END_) {
        //printf("ERROR: forward iterator called on end\n");
        return 0;
    }
    it->data_it++;
    if (it->data_it == it->mom->END_)
        return 0;
    else if (it->data_it == it->part_it->DATA + it->mom->PARTSZ_)
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
        iterator_fw(it);
    else
        iterator_bw(it);
}

int it_eq(const STR_INT_ITERATOR* a, const STR_INT_ITERATOR* b)
{
    return a->data_it == b->data_it;
}
//$ITERATOR

//^FUNCTIONS
//we want a useful char from number
//basically the symbol representing the largest allowed digit 1-9 a(10)-z(36)
char max_digit(size_t b)
{
    if (b <= 9){
        return b + '0' - 1; //the largest digit is one lower then the base 
    }
    if (b == 10) return '9';
    if (b <= 36){
        return b - 10 + 'a' - 1;
    }
    return '$';
}

/*numbers 10+ are represented by letters, so we need to test characters and not just regular numerals
c is char to be tested against the base (c < base, since base isn't a numeral in it's own system)
* this is a basic is_digit function only to tell us if the number is */
int is_digit(char c, const char maxDigit){
    //reduce capital letters to small, since we're not sure which come first
    if (c >= 'A' && c <= 'Z')
    {
        c = 'a' + (c - 'A');
    }
    //the case we need to deal with the 10+ base
    if (maxDigit >= 'a' && maxDigit <= 'z'){
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= maxDigit);
    }
    return c >= '0' && c <= maxDigit;
}

/*A horrible function that returns a truth value BUT also changes the char value that is being evaluated
 * the thing is our str_int uses char ordinal values of chars, not the symbols, so this is how we want to keep them
 * the value used by reference is actually a copy from the source already and we don't care what happens to it here*/
int is_digit_convert(char* p_c, const char* p_base){
    //reduce capital letters to small, since we're not sure which come first
    if (*p_c >= '0' && *p_c <= '9')
        *p_c = *p_c - '0';
    else if (*p_c >= 'A' && *p_c <= 'Z')
        *p_c = 10 + (*p_c - 'A');
    else if (*p_c >= 'a' && *p_c <= 'z')
        *p_c = 10 + (*p_c - 'a');
    else return 0; 
    //return truth value
        return *p_c >= 0 && *p_c < *p_base;
}

char to_symbol(const char* cnum)
{
    if (*cnum < 10)
        return *cnum + '0';
    return *cnum - 10 + 'A';
}

// THIS IS WRONG! It doesn't comply with the design of having the most significant number at the back of STR_INT!
//READ NUBMER FROM INPUT:
int read_num_from_input(STR_INT* num, FILE* f)
{
    char c;
    num->TAIL_LENGTH_ = 0;
    //we need to allow only numbers < base
    //IGNORE white spaces or any possibly separating symbols in front
    c = getc(f);
    while (c == '0' || is_digit(c, max_digit(num->BASE_)) == 0)//ignore leading zeros
    {
        if (c == '$') return 1;//$ is escape character
        c = getc(f);
    }
    printf("found a number and reading...\n");
    //READING THE NUMBER:

    STR_INT_PART* part_it = num->HEAD_;
    char* data_it = num->HEAD_->DATA; //iterator
    while (is_digit_convert(&c,&num->BASE_))
    {
        //if we reach the end of the data array:
        if (data_it == part_it->DATA + num->PARTSZ_) //last element is data+num->partSZ-1, so this is the overflow
        {
            if (new_si_part(num)){
                fprintf(stderr, "new part creation failed!\n");
                return 1;
            }
            part_it = part_it->NEXT;//created new part is already next in the linked list
            data_it = part_it->DATA;
            num->TAIL_LENGTH_ = 0;//we start from 0, the previous parts are all full
        }
        *data_it++ = c;
        num->TAIL_LENGTH_++;
        c = getc(f);

    }
    if (data_it < part_it->DATA + num->PARTSZ_) *data_it = '\0';//marks the end, but this is not gonna be used
    num->END_ = data_it;

    //HERE we mirror the elements of the linked arrays of char, so that every number has its lowest digit on 1
    //VARIABLES:
    int fw_count = 0;
    int bw_count = 0;
    char tmp;
    STR_INT_PART* fw_part = num->HEAD_;
    STR_INT_PART* bw_part = num->TAIL_;
    char* fw_DATA = fw_part->DATA;
    char* bw_DATA = bw_part->DATA+num->TAIL_LENGTH_-1;
    //MIRRORING loop:
    while (1)
    {
        //CHECKS if the mirroring should stop:
        //we only need to cross into other parts when the two pointers point to different parts
        if (fw_part != bw_part){
            //forward DATA iterator overflowing
            if (fw_DATA == fw_part->DATA + num->PARTSZ_)
            {
                fw_part = fw_part->NEXT;
                fw_DATA = fw_part->DATA;
            }
            //backward DATA iterator underflowing
            if (bw_DATA == (bw_part->DATA-1))
            {
                bw_part = bw_part->PREV;
                bw_DATA = bw_part->DATA+(num->PARTSZ_-1);
            }
            
        }
        if (fw_part == bw_part->NEXT)
            break;
        //reached the middle
        if (fw_part == bw_part && fw_DATA >= bw_DATA) 
            break;
        //SWITCH:
        //printf("switching %c <-> %c\n", *fw_DATA+'0', *bw_DATA+'0');
        tmp = *fw_DATA;
        *fw_DATA = *bw_DATA;
        *bw_DATA = tmp;
        //point to next
        fw_DATA++;
        bw_DATA--;
    }

    return 0;//zero errors
}

int read_num(STR_INT* num, FILE* f)
{
    STR_INT* reversed_num;
    if(!read_num_from_input(reversed_num,f))
    {
        fprintf(stderr, "failed to read the number from file!\n");
        return 1;
    }
    //NOW copy the number backwards using the STR_INT_ITERATOR
}

/*appEND_ is special because it pushes num->END_ further, and that's why it needs to call new_si_part*/
int str_int_appEND_(STR_INT* num, char digit)
{
    //we need to enlarge num
    if (num->END_ == num->TAIL_->DATA+num->PARTSZ_)
    {
        if (new_si_part(num)){
            fprintf(stderr, "new part creation failed!\n");
            return 1;
        }
        num->END_ = num->TAIL_->DATA;
        num->TAIL_LENGTH_ = 1;//we start from 0, the previous parts are all full
    }
    //num size is fine
    else num->TAIL_LENGTH_++;
    //insert digit and move END_ further
    *num->END_++ = digit;
    return 0;
}

int str_int_insert(STR_INT_ITERATOR* num_it, char digit)
{
    //iterator at the END_, so appEND_
    if (num_it->data_it == num_it->mom->END_) {
        str_int_appEND_(num_it->mom, digit);
        //END_ was push, possibly into an entirely different area in memory
        //-> END_ previous END_ might be just after the last part DATA chunk
        num_it->data_it = num_it->mom->END_-1;
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
        if (*bw_it->data_it < 10)
            putc(*bw_it->data_it + '0',f);
        else
            putc(*bw_it->data_it - 10 + 'A',f);
        charCount++;
        if (charCount == line_len){
            putc(brk,f);
            charCount = 0;
        }
    }
    putc('\n',f);
    free((void*)bw_it);
}

//BACKWARD PRINTING is actually just straightforward printing, because we keep the DATA backward
void backward_print_str_int(STR_INT* num, FILE* f, int brk, size_t line_len)
{
    int fin = 0;
    for (STR_INT_PART* part_it = num->HEAD_; part_it != NULL; part_it = part_it->NEXT)
    {   
        int i = 0;
        if (fin) break;
        for (char* data_it = part_it->DATA; data_it < part_it->DATA + num->PARTSZ_; data_it++)
        {
            if (part_it == num->TAIL_ && data_it == num->END_)
            {    
                fin = 1;
                break;
            }

            if (*data_it < 10)
                putc(*data_it + '0',f);
            else
                putc(*data_it - 10 + 'A',f);
        }
    }
    putc('\n',f);
}

void print_str_int(STR_INT* num, FILE* f)
{
    formated_print_str_int(num, f, 0, 0);
}


int mark(char* num, char base)
{
    char* num_it = num;
    while (is_digit(*num_it,base)) num_it++;
    *num_it = '\0';
    return 0;
}

//ARITHMETICS:
int base_not_eq(STR_INT* a, STR_INT* b){
    if (a->BASE_ != b->BASE_){ 
        printf("ERROR: only add numbers of same base! %c != %c", a->BASE_+'0', b->BASE_+'0');
        return 1;
    }
    return 0;
}

int str_int_same(STR_INT* a, STR_INT*b){
    return a->HEAD_ == b->HEAD_;
}

STR_INT_ITERATOR* target_setup(STR_INT_ITERATOR* a_it, STR_INT_ITERATOR* b_it, STR_INT* target){
    if (str_int_same(a_it->mom, target)){
        return a_it;
    }
    else if (str_int_same(b_it->mom, target)){
        return b_it;
    }
    //target is a new str_int innitiated to 0
    target->BASE_ = a_it->mom->BASE_;//we need to common base
    target->END_--;//move END_ from pointing to DATA[1] to DATA[0]
    target->TAIL_LENGTH_ = 0;//decrease from 1 to 0
    STR_INT_ITERATOR* t_it = make_fw_iterator(target);
    return t_it;
}

int str_int_add(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    int isTargetNew = 0; 
    STR_INT_ITERATOR* a_it = make_fw_iterator(a);
    STR_INT_ITERATOR* b_it = make_fw_iterator(b);
    STR_INT_ITERATOR* t_it = target_setup(a_it, b_it, target);

    char overflow = 0;//overflow tmp
    int a_cont = 1;
    int b_cont = 1;
    while (a_cont && b_cont)
    {
        char sum = *a_it->data_it + *b_it->data_it + overflow;
        str_int_insert(t_it, sum % a->BASE_);
        overflow = sum / a->BASE_;
        if (!it_eq(a_it,t_it) && !it_eq(b_it,t_it)) iterator_fw(t_it);
        a_cont = iterator_fw(a_it);
        b_cont = iterator_fw(b_it);
    }
    while (a_cont){
        str_int_insert(t_it, *a_it->data_it + overflow);
        overflow = 0;
        if (!it_eq(a_it, t_it)) iterator_fw(t_it);
        a_cont = iterator_fw(a_it);
    }
    while (b_cont){
        str_int_insert(t_it, *b_it->data_it + overflow);
        overflow = 0;
        if (!it_eq(b_it, t_it)) iterator_fw(t_it);
        b_cont = iterator_fw(b_it);
    }
    if (overflow){
        str_int_insert(t_it, 1);
    }
    //deallocation
    if (!it_eq(a_it, t_it) && !it_eq(b_it, t_it))
        free((void*)t_it);
    free((void*)a_it);
    free((void*)b_it);
    return 0;
}

int str_int_minus(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (base_not_eq(a,b)){ 
        return 1;
    }
    int isTargetNew = 0; 
    STR_INT_ITERATOR* a_it = make_fw_iterator(a);
    STR_INT_ITERATOR* b_it = make_fw_iterator(b);
    STR_INT_ITERATOR* t_it = target_setup(a_it, b_it, target);

    char overflow = 0;//overflow tmp
    int a_cont = 1;
    int b_cont = 1;
    char sum = 0; 
    while (a_cont && b_cont)
    {
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
        str_int_insert(t_it, sum);
        if (!it_eq(a_it,t_it) && !it_eq(b_it,t_it)) iterator_fw(t_it);
        a_cont = iterator_fw(a_it);
        b_cont = iterator_fw(b_it);
    }
    while (a_cont && overflow){//the main complication is that the a digit can be 0 -> base - overflow
        if (*a_it->data_it < overflow) 
        {
            sum = a->BASE_ - overflow;
        }
        else sum = *a_it->data_it - overflow;
        
        str_int_insert(t_it, sum);
        overflow = 0;
        if (!it_eq(a_it, t_it))
            iterator_fw(t_it);
        a_cont = iterator_fw(a_it);
    }
    while (b_cont){//a digit is always 0 and overflow is always 1
        str_int_insert(t_it, a->BASE_ - *b_it->data_it - overflow);//t_it doesn't move
        if (!it_eq(b_it, t_it))
            iterator_fw(t_it);
        overflow = 1;
        b_cont = iterator_fw(b_it);
    }
    if (overflow){
        str_int_insert(t_it, a->BASE_ - 1);
        iterator_fw(t_it);
    }
    //we need to mark the END_ correctly by backtracking to last non-0 
    if (t_it->data_it == t_it->mom->END_)
        iterator_bw(t_it);
    while (*t_it->data_it == 0) iterator_bw(t_it);
    iterator_fw(t_it);
    t_it->mom->END_ = t_it->data_it;

    //deallocation:
    if (!it_eq(a_it, t_it) && !it_eq(b_it, t_it))
        free((void*)t_it);
    free((void*)a_it);
    free((void*)b_it);
    return 0;
}

STR_INT* convert(){}