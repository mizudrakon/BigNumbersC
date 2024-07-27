#include <stdio.h>
#include <stdlib.h>
#include "str_number.h"
#define TEST 
#ifdef DEFS
//test definitions
#endif

int test_func(int limit, int (f)(STR_INT*,STR_INT*,STR_INT*), char op_sign, STR_INT* a, STR_INT* b, STR_INT* c)
{
    for (int i = 0; i < limit; i++){
        int bs;
        printf("Insert base:");
        scanf("\n%d",&bs);
        char base = max_digit(bs+1);
        printf("base is: %c\n", base);
        a = new_str_int(bs,5);
        print_str_int(a,stdout);
        printf("enter number: ");
        read_num(a, stdin);
        printf("enter number: ");
        b = new_str_int(bs,5);
        read_num(b, stdin);
        printf("regular print: \n");
        printf("a:\n");
        print_str_int(a,stdout);
        printf("b:\n");
        print_str_int(b,stdout);
        printf("a %c b:\n", op_sign);
        //STR_INT* c = new_str_int(bs,5);
        f(a,b,b);
        //str_int_add(a,b,b);
        print_str_int(b,stdout);
        printf("deleting a,b\n");
        putchar('\n');
        deleteSTR_INT(a);
        deleteSTR_INT(b);
        //deleteSTR_INT(c);
    }
}

int main(int argc, char** argv)
{

#ifdef LIST_NUMS
    int b = 16;
    //char base = max_digit(b+1);
    /*the thing is: we usually need the max_digit which is 15, not the actual base which would be 16
     not sure if I should change the code to enter the actual base...*/
    for (int i = 1; i <= 40; i++)
        printf("max digit for %d: %c\n", i, max_digit(i));
    printf("list of all numeric characters for base %c:\n",base);
    for (int i = 1; i <= 140; i++){
        if (is_digit(i,max_digit(base+1)))
            printf("%c is digit for base %c = %d\n",i,max_digit(base+1),b);
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
