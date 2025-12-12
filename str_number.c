#include "str_number.h"

const char MAX_POSSIBLE_BASE_ = 'z' - 'a' + 11;

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
    mom->SIGN = '+';
    return 0;//no failure
}

STR_INT* new_str_int(char base, size_t part_len)
{
    if (base > MAX_POSSIBLE_BASE_)
    {
        printf("base %d not supported!", base);
        return NULL;
    }
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
    strnum->LAST_ = strnum->HEAD_->DATA;
    strnum->Begin = make_fw_iterator(strnum);
    strnum->End = make_bw_iterator(strnum);
    return strnum;
}

//cleaning STR_INT data
void deleteSTR_INT(STR_INT* corpse)
{
    free((void*)corpse->Begin);
    free((void*)corpse->End);
    STR_INT_PART* part_it = corpse->HEAD_;
    for (; part_it != NULL; part_it = part_it->NEXT)
    {
        free((void*)part_it->DATA);
        if (part_it->PREV!=NULL)
            free((void*)part_it->PREV);
    }
    free((void*)part_it);
    free((void*)corpse);

#ifdef DEBUG
    printf("deleted STR_INT\n");
#endif
}
//$CONSTRUCTOR

//^FUNCTIONS

size_t length(STR_INT* num)
{
    return (num->TOTAL_PARTS_ - 1) * num->PARTSZ_ + num->TAIL_LENGTH_;
}

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

// conversion from char as character to char as number
char to_cnum(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        c = (c >= 'A' && c <= 'Z') ? c - 'A' : c - 'a';
        return c + 10;
    }
    return '$';
}

int is_cnum_digit(char cnum, const STR_INT* num)
{
    return (cnum == '$') ? 0 : cnum <= (num->BASE_ - 1);
}

// does the character c represent a digit given the base of our number?
int is_char_digit(char c, const STR_INT* num)
{
    return is_cnum_digit(to_cnum(c), num);
}

char to_symbol(const char cnum)
{
    if (cnum < 10)
        return cnum + '0';
    return cnum - 10 + 'A';
}
