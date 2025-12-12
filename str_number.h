#ifndef STR_NUMBER_H
#define STR_NUMBER_H
#include <stdio.h>
#include <stdlib.h>


//basically a supplement for the long-number-kept-as-string class
/*STR_INT_PART is wrapper for a c array with pointers to previous and next
 it also has a pointer to it's mother which is the str_int struct defined next*/
typedef struct str_int_part 
{
    size_t PART_NUMBER;//it a kth part in str_int::totalParts
    struct str_int_part* PREV;
    struct str_int_part* NEXT;
    struct str_int* MOTHER;//pointer to str_int struct that collects the overlaying info
    char* DATA;//array of chars used as numbers -> allocated in construction to PARTSZ
} STR_INT_PART;

typedef struct str_int_iterator STR_INT_ITERATOR;

//structure with all info about the str_int coupled in
typedef struct str_int
{
    char BASE_;//numeric base of the number
    char SIGN;
    size_t TOTAL_PARTS_;//number of str_int_parts in total
    size_t PARTSZ_;//size of a single part
    size_t TAIL_LENGTH_;//how far is the last part of allocated memory filled
    //-> so the length of the whole number is totlaParts * partSz + lastPartLength
    //pointers to first and last part:
    STR_INT_PART* HEAD_;
    STR_INT_PART* TAIL_;
    STR_INT_ITERATOR* Begin;
    STR_INT_ITERATOR* End;
    char* LAST_;
} STR_INT;

//^CONSTRUCTORS

int new_si_part(STR_INT* mom);
/*create new str_int_part = c array that we can read the number into
returns int as a truth value: 1 = error, 0 = no error
prev = NULL for head, or some other str_int_part
next = NULL untill we need more space, when it gets allocated and filled by the read_num function
parts are always added to the str_int struct, and it can eitehr become the head AND tail, or the new tail, with the
old one as its prev*/

STR_INT* new_str_int(char base, size_t part_len);
//STR_INT* new_str_int(size_t base_num, size_t part_len);
/*creating a new handle for a number*/

void deleteSTR_INT(STR_INT* corpse);
/*cleaning function to free the memory*/

//$CONSTRUCTORS

//^ITERATOR:
typedef struct str_int_iterator
{
    STR_INT* mom;
    STR_INT_PART* part_it;
    char* data_it;
    int direction_fw;
} STR_INT_ITERATOR;
/*we need to iterate over the data every time we do anything with str_int
 * that means A LOT of repeating pointer declarations */

STR_INT_ITERATOR* make_fw_iterator(STR_INT* mom);
//iterator that starts at head->data
#define make_forward_iterator(mom)(return make_fw_iterator(mom))

STR_INT_ITERATOR* make_bw_iterator(STR_INT* mom);
//iterator that starts at the tail->data+tailLength = 1 past the end
#define make_backward_iterator(mom)(return make_bw_iterator(mom))

int iterator_fw(STR_INT_ITERATOR* it);
//move iterator towards the end

int iterator_bw(STR_INT_ITERATOR* it);
//move iterator towards the begin

int next(STR_INT_ITERATOR* it);
//universal call to move iterators

int it_eq(const STR_INT_ITERATOR* a, const STR_INT_ITERATOR* b);
//do iterators point to the same thing?
int it_l(const STR_INT_ITERATOR* left, const STR_INT_ITERATOR* right);
int it_leq(const STR_INT_ITERATOR* left, const STR_INT_ITERATOR* right);

void swap(STR_INT_ITERATOR* left, STR_INT_ITERATOR* right);
//$ITERATOR:

//^HELPFUL FUNCTIONS:
char max_digit(size_t b);
//given a base b, get the char representing the maximum single digit

char to_cnum(char c);

int is_cnum_digit(char cnum, const STR_INT* num);
//also converts the *p_c into form used in str_int (char with required ordinal value)

int is_char_digit(char c, const STR_INT* num);
//tests a char for being a numeric given the base

char to_symbol(const char cnum);
//converts small saved in char to it's char symbol

int append(STR_INT* num, char digit);
/*appends a digit to the end of the number*/

int insert(STR_INT_ITERATOR* num_it, char digit);

int pop_back(STR_INT*num);
//removes last element (most significant number)

void formated_print_str_int(STR_INT* num, FILE* f,char brk, size_t line_len);
//print with possibility of adding new_lines, brk is meant as bool, line len is the required lenght
//but it might be nice to make brk a separator specifier and get the truth value just for line_len x<=0 being false

void print_str_int(STR_INT* num, FILE* f);
//just print with no \n-s

int read_num(STR_INT* num, FILE* f);
/*read from the specified input into the specified number handler
 * writes the digits one by one into the head part
 * calls new_str_int_part() if necessary
 * returns 1 for success and 0 for failure, which might switch, since the opposite might be handier for counting total errors*/

//int mark(char* num, char base);
/*marks the end of the number in the tail
 * this is only useful when we need to assess an unknown array of digits
*/
//$HELPFUL FUNCTIONS:

size_t length(STR_INT* num);

//^ARITHEMTICS:
/* FOR ALL BINARY OPERATIONS:
 * target = a operation b
 * target can be specified as a or b, or it needs to be new_str_int() first
 */
int identical(STR_INT* left, STR_INT* right);

int equal(STR_INT* left, STR_INT* right);

int add(STR_INT* a, STR_INT* b, STR_INT* target);
// target = a + b, success return 0, error return 1
#define add_to(a,b)(return add(a,b,a))

int subtract(STR_INT* a, STR_INT* b, STR_INT* target);
// target = a - b, success return 0, error return 1
// minus as an alias for subtract
#define minus(a,b,c)(return subtract(a,b,c))
#define subtract_from(a,b)(return subtract(a,b,a))

int mult(STR_INT* a, STR_INT* b, STR_INT* target);
#define mult(a,b)(return mult(a,b,a))

int divide(STR_INT* a, STR_INT* b, STR_INT* target);
#define divide(a,b)(return div(a,b,a))
//$ARITHEMTICS:

#endif