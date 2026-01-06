#ifndef STR_NUMBER_H
#define STR_NUMBER_H
#include <stdio.h>
#include <stdlib.h>


//basically a supplement for the long-number-kept-as-string class
/*STRINT_PART is wrapper for a c array with pointers to previous and next
 it also has a pointer to it's mother which is the strint struct defined next*/
typedef struct strint_part 
{
    size_t PART_NUMBER;//it a kth part in strint::totalParts
    struct strint_part* PREV;
    struct strint_part* NEXT;
    struct strint* MOTHER;//pointer to strint struct that collects the overlaying info
    char* DATA;//array of chars used as numbers -> allocated in construction to PARTSZ
} STRINT_PART;

typedef struct strint_iterator STRINT_ITERATOR;

//structure with all info about the strint coupled in
typedef struct strint
{
    char BASE_;//numeric base of the number
    char SIGN;
    size_t TOTAL_PARTS_;//number of strint_parts in total
    size_t PARTSZ_;//size of a single part
    size_t TAIL_LENGTH_;//how far is the last part of allocated memory filled
    //-> so the length of the whole number is totlaParts * partSz + lastPartLength
    //pointers to first and last part:
    STRINT_PART* HEAD_;
    STRINT_PART* TAIL_;
    STRINT_ITERATOR* Begin;
    STRINT_ITERATOR* End;
    char* LAST_;
} STRINT;

//^CONSTRUCTORS

int new_si_part(STRINT* mom);
/*create new strint_part = c array that we can read the number into
returns int as a truth value: 1 = error, 0 = no error
prev = NULL for head, or some other strint_part
next = NULL untill we need more space, when it gets allocated and filled by the read_num function
parts are always added to the strint struct, and it can eitehr become the head AND tail, or the new tail, with the
old one as its prev*/

STRINT* new_strint(char base, size_t part_len);
//STRINT* new_strint(size_t base_num, size_t part_len);
/*creating a new handle for a number*/

void reset_strint(STRINT* num);

void move_strint(STRINT* source, STRINT* target);

void copy_strint(STRINT* source, STRINT* target);

void deleteSTRINT(STRINT* corpse);
/*cleaning function to free the memory*/

//$CONSTRUCTORS

//^ITERATOR:
typedef struct strint_iterator
{
    STRINT* mom;
    STRINT_PART* part_it;
    char* data_it;
    int direction_fw;
} STRINT_ITERATOR;
/*we need to iterate over the data every time we do anything with strint
 * that means A LOT of repeating pointer declarations */

STRINT_ITERATOR* make_fw_iterator(STRINT* mom);
//iterator that starts at head->data
//#define make_forward_iterator(mom)(return make_fw_iterator(mom))

STRINT_ITERATOR* make_bw_iterator(STRINT* mom);
//iterator that starts at the tail->data+tailLength = 1 past the end
//#define make_backward_iterator(mom)(return make_bw_iterator(mom))

void it_reset(STRINT_ITERATOR* it);

int iterator_fw(STRINT_ITERATOR* it);
//move iterator towards the end

int iterator_bw(STRINT_ITERATOR* it);
//move iterator towards the begin

int next(STRINT_ITERATOR* it);
//universal call to move iterators

char it_value(STRINT_ITERATOR* it);
void set_it_value(STRINT_ITERATOR* it, char cnum);

void point_it(STRINT_ITERATOR* ls, STRINT_ITERATOR* rs);

int it_eq(const STRINT_ITERATOR* a, const STRINT_ITERATOR* b);
//do iterators point to the same thing?
int it_l(const STRINT_ITERATOR* left, const STRINT_ITERATOR* right);
int it_leq(const STRINT_ITERATOR* left, const STRINT_ITERATOR* right);

void swap_digit(STRINT_ITERATOR* left, STRINT_ITERATOR* right);
//$ITERATOR:

//^HELPFUL FUNCTIONS:
char max_digit(size_t b);
//given a base b, get the char representing the maximum single digit

char to_cnum(char c);

int is_cnum_digit(char cnum, const STRINT* num);
//also converts the *p_c into form used in strint (char with required ordinal value)

int is_char_digit(char c, const STRINT* num);
//tests a char for being a numeric given the base

char to_symbol(const char cnum);
//converts small saved in char to it's char symbol

int append(STRINT* num, char digit);
/*appends a digit to the end of the number*/

int insert(STRINT_ITERATOR* num_it, char digit);

int pop_back(STRINT* num);
//removes last element (most significant number)

//shift operations won't be practical in multiplication...
int shift_left(STRINT* num, size_t shift);
//operation equivalent to num * base

int shift_right(STRINT* num, size_t shift);
//operation equivalent to num div base

void formated_print_strint(STRINT* num, FILE* f,char brk, size_t line_len);
//print with possibility of adding new_lines, brk is meant as bool, line len is the required lenght
//but it might be nice to make brk a separator specifier and get the truth value just for line_len x<=0 being false

void print_strint(STRINT* num, FILE* f);
//just print with no \n-s

int read_strint(STRINT* num, FILE* f);
/*read from the specified input into the specified number handler
 * writes the digits one by one into the head part
 * calls new_strint_part() if necessary
 * returns 1 for success and 0 for failure, which might switch, since the opposite might be handier for counting total errors*/

int read_strint_string(STRINT* num, char* text);

int read_strint_file(STRINT* num, char* file_name);

//int mark(char* num, char base);
/*marks the end of the number in the tail
 * this is only useful when we need to assess an unknown array of digits
*/
//$HELPFUL FUNCTIONS:

size_t length(STRINT* num);

//^ARITHEMTICS:
/* FOR ALL BINARY OPERATIONS:
 * target = a operation b
 * target can be specified as a or b, or it needs to be new_strint() first
 */
 
int identical(STRINT* left, STRINT* right);

int equal(STRINT* left, STRINT* right);

int is_zero(STRINT* num);

int is_one(STRINT* num);

int add(STRINT* a, STRINT* b, STRINT* target);
// target = a + b, success return 0, error return 1
#define add_to(a,b)(return add(a,b,a))

int subtract(STRINT* a, STRINT* b, STRINT* target);
// target = a - b, success return 0, error return 1
// minus as an alias for subtract
#define minus(a,b,c)(return subtract(a,b,c))
#define subtract_from(a,b)(return subtract(a,b,a))

int mult(STRINT* a, STRINT* b, STRINT* target);
#define times(a,b)(return mult(a,b,a))

int divide(STRINT* a, STRINT* b, STRINT* target);
#define divide(a,b)(return div(a,b,a))
//$ARITHEMTICS:

#endif