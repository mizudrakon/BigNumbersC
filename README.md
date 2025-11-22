# Large number calculator C

- First of all this is mostly a project to remember the ways of C coming from some serious time with C++;  
- It's supposed to read a large number of a specified base as a array of characters and to be able to write them out.  
- The numeric length of the numbers is unknown, and the struct points to a linked list of smaller parts containing a character array of user specified length.  
- STR_INT is the struct holding pointers to head and tail of the linked list, and all the basic info necessary
- STR_INT_PART wrapps an array of chars and points to prev(ious) and next part
- The numbers are read from input (needs to be specified) by read_num(STR_INT*, char base) - at the moment base is actually the symbol for the largest digit allowed under a (base+1) base.
- the numbers read into the arrays regularly starting from most significant digit, but after this is done, the digits are mirrored to the opposite direction, so the combination of partNumber and data index gives us digits of the same significance.
- print_str_int functions print the number regularly starting with the most significant digit
- backward_print_str_int does the printing in the direction digits are being kept in memory...
- please refer to str_number.h for actual list of functions and information on their use, I'll add up some more user oriented info here later.

## Done

- Storage and input/output of the numbers pretty much dealt with. 
- Can read numbers of different base, and write them  
- Adding two numbers together, either into a new thrid one or rewriting one of the two str_int_add(a,b,target)  
- Subtracting str_int_minu(a,b,target) means a - b = target.

## To Do

- shift left and right (which is mostly useful in multiplication)
- arithmetics: multiplication and difision... that might be it
- subtraction might need more debugging, there are two functions: append, insert that are used to enter the resulting digit into the target... it's not vary efficient or readable, probably could be merged into a single function. insert() actually can append already... anyway, it works for now.

## Additional info

- str_num folder contains an Obsidian canvas somewhat illustrating the function
- some new functions haven't been added to the h file, since they're not intended for use in any application. I might want to remove some more of them, but I also like the h file to be a sort of docummentation file... it's a pickle.

## last changes

- defined identical(a,b)
- defined equal(a,b)
- changed read_num to use iteraor struct
- NOT TESTED
