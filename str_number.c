#include "str_number.h"

const char MAX_POSSIBLE_BASE_ = 'z' - 'a' + 11;

//^CONSTRUCTOR
//this adds a new part to a specified mother, does it need to return a pointer?
int new_si_part(STRINT* mom)
{
    STRINT_PART* part;
    if ( (part = (STRINT_PART*) malloc(sizeof(STRINT_PART))) == NULL )
    {    printf("strint_part basic malloc failed\n"); return 1;}
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
    {    printf("strint_part data malloc failed\n"); return 1;}
    mom->SIGN = '+';
    return 0;//no failure
}

STRINT* new_strint(char base, size_t part_len)
{
    if (base > MAX_POSSIBLE_BASE_)
    {
        printf("base %d not supported!", base);
        return NULL;
    }
    STRINT* strnum;
    if ((strnum = (STRINT*) malloc(sizeof(STRINT))) == NULL)//allocate struct
        printf("new_strint malloc failed\n");
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

void reset(STRINT* num)
{
    char base = num->BASE_;
    size_t partsz = num->PARTSZ_;
    deleteSTRINT(num);
    num = new_strint(base,partsz);
}

//#define DEBUG
//cleaning STRINT data
void deleteSTRINT(STRINT* corpse)
{
#ifdef DEBUG
    printf("deleting STRINT\n");
#endif
    free((void*)corpse->Begin);
    free((void*)corpse->End);
#ifdef DEBUG
    printf("deleted STRINT Begin and End iterators\n");
#endif
    STRINT_PART* part_it = corpse->HEAD_;
    for (; part_it != NULL; part_it = part_it->NEXT)
    {
        free((void*)part_it->DATA);
        if (part_it->PREV!=NULL)
            free((void*)part_it->PREV);
    }
#ifdef DEBUG
    printf("deleted STRINT data\n");
#endif
    free((void*)part_it);
    free((void*)corpse);

#ifdef DEBUG
    printf("deleted STRINT\n");
#endif
}
//$CONSTRUCTOR

//^FUNCTIONS

size_t length(STRINT* num)
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

int is_cnum_digit(char cnum, const STRINT* num)
{
    return (cnum == '$') ? 0 : cnum <= (num->BASE_ - 1);
}

// does the character c represent a digit given the base of our number?
int is_char_digit(char c, const STRINT* num)
{
    return is_cnum_digit(to_cnum(c), num);
}

char to_symbol(const char cnum)
{
    if (cnum < 10)
        return cnum + '0';
    return cnum - 10 + 'A';
}
