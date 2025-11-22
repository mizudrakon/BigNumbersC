#include <stdio.h>
#include <stdlib.h>
#include "str_number.h"
#ifdef DEFS
//test definitions
#endif

#define SIZE 2
void tests(int base, size_t len)
{
    const int sz = 2;
    printf("Tests in base %d, partition length %ld\n",base,len);
    int (*func_array[SIZE])(STR_INT*,STR_INT*,STR_INT*) = {
        &add,
        &subtract
    };
    char signs[SIZE] = {'+','-'};
    STR_INT *a = new_str_int(base,len);
    print_str_int(a,stdout);
    printf("enter number: ");
    read_num(a, stdin);
    printf("enter number: ");
    STR_INT *b = new_str_int(base,len);
    read_num(b, stdin);
    printf("regular print: \n");
    printf("a:\n");
    print_str_int(a,stdout);
    printf("b:\n");
    print_str_int(b,stdout);
    for (int i = 0; i < sz; i++)
    {
        printf("a %c b:\n", signs[i]);
        func_array[i](a,b,b);
        print_str_int(b,stdout);
    }
}

/*  limit - repetitions
    int (f)
    op_sign
    a, b, c
*/
void test_arithmetic(int (f)(STR_INT*,STR_INT*,STR_INT*), char op_sign, STR_INT* first, STR_INT* second, STR_INT* target)
{
    int bs;
    printf("Insert base:");
    scanf("\n%d",&bs);
    //char base = max_digit(bs+1);
    printf("base is: %c\n", bs);
    print_str_int(first,stdout);
    printf("regular print: \n");
    printf("first:\n");
    print_str_int(first,stdout);
    printf("second:\n");
    print_str_int(second,stdout);
    printf("first %c second:\n", op_sign);
    //STR_INT* c = new_str_int(bs,5);
    f(first,second,target);
    //str_int_add(a,b,b);
    print_str_int(target,stdout);
    printf("deleting first,second,target\n");
    putchar('\n');
    deleteSTR_INT(first);
    deleteSTR_INT(second);
}

void print_digits(int base, int range)
{
    printf("Digits for base %d:\n", base);
    STR_INT* a = new_str_int(base,5);
    for (int i = 0; i < range; i++)
    {
        // I need increase and decrease functions
        printf("%d:\n", i);
        print_str_int(a, stdout);
    }
}

int main(int argc, char** argv)
{
#define LIST_NUMS 
#ifdef LIST_NUMS
    int b = 16;
    //char base = max_digit(b+1);
    /*the thing is: we usually need the max_digit which is 15, not the actual base which would be 16
     not sure if I should change the code to enter the actual base...*/
    for (int i = 1; i <= 40; i++)
        printf("max digit for base %d: %c\n", i, max_digit(i));
    printf("list of all numeric characters for base %c:\n",b);
    for (int i = 1; i <= 140; i++){
        if (is_digit(i,max_digit(b+1)))
            printf("%c is digit for base %c = %d\n",i,max_digit(b+1),b);
    }
    putchar('\n');
#endif
#ifdef TEST
    STR_INT* a, *b, *c;
    test_func(3, str_int_add, '+', a, b, b);
    test_func(2, str_int_minus, '-', a, b, b);
#endif
#ifdef TEST2
    //realization: char IS an 8 bit number...
    //C doesn't support overloading
    char a = 3;
    char b = 5;
    if (b < 10)
        b += a;
    printf("%d\n",b);
#endif
}
