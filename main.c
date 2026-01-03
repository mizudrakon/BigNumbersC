#include <stdio.h>
#include <stdlib.h>
#include "str_number.h"
#ifdef DEFS
//test definitions
#endif

#define SIZE 2

void list_nums()
{
    //char base = max_digit(b+1);
    /*the thing is: we usually need the max_digit which is 15, not the actual base which would be 16
     not sure if I should change the code to enter the actual base...*/
    int b = 10;
    STRINT* a = new_strint(b,5);
    for (int i = 1; i <= 40; i++)
        printf("max digit for base %d: %c\n", i, max_digit(i));
    printf("list of all numeric characters for base %d:\n",b);
    for (int i = 1; i <= 140; i++){
        if (is_cnum_digit(i,a))
            printf("%c is a digit for base %d\n",i,b);
    }
    putchar('\n');
}

void iterator_test(STRINT* num)
{
    STRINT_ITERATOR* fw_it = make_fw_iterator(num);
    STRINT_ITERATOR* bw_it = make_bw_iterator(num);
    if (it_l(fw_it,bw_it))
        printf("LESS OPERATOR WORKS\n");
    else
        printf("LESS OPERATOR DOESN'T WORK!\n");
    //STRINT_ITERATOR* bw_it = make_bw_iterator(num);
    //MIRRORING loop:
#ifdef ITERATORS_BASIC
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
    while (!it_eq(bw_it,num->Begin))
    {
        printf("%c\n",to_symbol(*(bw_it->data_it)));
        next(bw_it);
    }
    printf("%c\n",to_symbol(*(bw_it->data_it)));
    next(bw_it);
    printf("%c\n",to_symbol(*(bw_it->data_it)));
#else
    int i = 0;
    int stop = length(num);
    while (it_leq(fw_it,bw_it) && i < stop)
    {
        printf("fw: %c bw: %c\n",to_symbol(*(fw_it->data_it)),to_symbol(*(bw_it->data_it)));
        next(fw_it);
        next(bw_it);
        i++;
    }
#endif
    free((void*)fw_it);
    free((void*)bw_it);
}

void tests(int base, size_t len)
{
    printf("Tests in base %d, partition length %ld\n",base,len);
    STRINT *a = new_strint(base,len);
    print_strint(a,stdout);
    printf("enter number: ");
    read_strint(a, stdin);
    printf("enter number: ");
    STRINT *b = new_strint(base,len);
    read_strint(b, stdin);
    printf("regular print: \n");
    printf("a:\n");
    print_strint(a,stdout);
    printf("\nb:\n");
    print_strint(b,stdout);
}

/*  limit - repetitions
    int (f)
    op_sign
    a, b, c
*/

#define BS 10
#define SZ 2
void test_arithmetic()
{
    char a[] = "A10.txt";
    char b[] = "B10.txt";  
    printf("operating in base: %d\n", BS);
    int (*func_array[SZ])(STRINT*,STRINT*,STRINT*) = {
        &add,
        &subtract
    };
    char signs[SZ] = {'+','-'};
    
    STRINT* A = new_strint(10,5);
    FILE* fi;
    if ((fi = fopen(a,"r")) != NULL)
    {
        read_strint(A,fi);
    }
    else 
    {
        deleteSTRINT(A);
        return;
    }
    if (fclose(fi) == EOF) {
        deleteSTRINT(A);
        return;
    }
    printf("regular print: \n");
    printf("A:\n");
    print_strint(A,stdout);
    printf("\n");

    STRINT* B = new_strint(10,5);
    if ((fi = fopen(b,"r")) != NULL)
    {
        read_strint(B,fi);
    }
    else 
    {
        deleteSTRINT(A);
        deleteSTRINT(B);
        return;
    }
    if (fclose(fi) == EOF) {
        deleteSTRINT(A);
        deleteSTRINT(B);
        return;
    }

    STRINT* T = new_strint(10,5);
    printf("A: ");
    print_strint(A,stdout);
    printf("\n");
    printf("B: ");
    print_strint(B,stdout);
    printf("\n");
    printf("T: ");
    print_strint(T,stdout);
    printf("\n");

    printf("A %c B = T:\n", signs[0]);
    func_array[0](A,B,T);
    printf("A: ");
    print_strint(A,stdout);
    printf("\n");
    printf("B: ");
    print_strint(B,stdout);
    printf("\n");
    printf("T: ");
    print_strint(T,stdout);
    printf("\n");

    printf("T %c B = A:\n", signs[1]);
    func_array[1](T,B,A);
    printf("A: ");
    print_strint(A,stdout);
    printf("\n");
    printf("B: ");
    print_strint(B,stdout);
    printf("\n");
    printf("T: ");
    print_strint(T,stdout);
    printf("\n");

    deleteSTRINT(A);
    deleteSTRINT(B);
    deleteSTRINT(T);
}

void print_digits(int base, int range)
{
    printf("Digits for base %d:\n", base);
    STRINT* a = new_strint(base,5);
    for (int i = 0; i < range; i++)
    {
        // I need increase and decrease functions
        printf("%d:\n", i);
        print_strint(a, stdout);
    }
}

void shift_tests()
{
    FILE* f;
    if ((f = fopen("num.txt","r")) == NULL){
        printf("failed opening file\n");
        return;
    } 
    STRINT* num = new_strint(10,5);
    read_strint(num, f);
    if (fclose(f) == EOF)
    {
        printf("failed closing file\n");
        return;
    }
    print_strint(num,stdout);
    printf("\n");
    //while(pop_back(num))
    for (int i = 0; i < 4; i++)
    //while(shift_right(num))
    {
        //shift_right(num,3);
        shift_left(num,3);
        print_strint(num,stdout);
        printf("\nlength: %ld\n", length(num));
    }
    deleteSTRINT(num);
}

void mult_test()
{
    size_t pt_size = 5;
    size_t base = 10;
    FILE* f;
    if ((f = fopen("mult_test.txt","r")) == NULL){
        printf("failed opening file\n");
        return;
    } 
    printf("Multiplication test:\n");
    while (getc(f) != 'q')
    {
        STRINT* a = new_strint(base,pt_size);
        read_strint(a,f);
        STRINT* b = new_strint(base,pt_size);
        read_strint(b,f);
        STRINT* r = new_strint(base,pt_size);
        read_strint(r,f);
        STRINT* t = new_strint(base,pt_size);

        print_strint(a,stdout);
        printf("\n");
        print_strint(b,stdout);
        printf("\n=\n");
        mult(a,b,t);
        print_strint(t,stdout);
        printf("\n");
        print_strint(r,stdout);
        printf("?\n");
        int test = equal(r,t);
        if (test)
            printf("good\n");
        
        deleteSTRINT(a);
        deleteSTRINT(b);
        deleteSTRINT(r);
        deleteSTRINT(t);
        if (!test)
        {
            printf("bad\n");
            break;
        }
    }
    fclose(f);
}

int main(void)
{
#ifdef SETBASE
    size_t pt_size = 5;
    size_t base = 16;
#endif

    //tests(base,pt_size);
    //test_arithmetic();
    //shift_tests();
    mult_test();
}