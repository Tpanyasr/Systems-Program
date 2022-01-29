#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
using namespace std;


#define PAGESIZE 4096 //ak

int main()
{
    void *p;
    int x;
    int i =  brk(p); //-1 not good, 0 ok
    //errno
    void *a = sbrk(0); // sbrk wants an integer because it grows by the arguement wherever it starts.
    //sbrk returns previous address of the program break
    //a is basically the address where this block starts

    // void *a = sbrk(0); returns the address of the current program break
    // brk(a+s); is equal to sbrk(s);

    //the above arrangement is basically malloc
    brk(a + PAGESIZE);
    a = sbrk(0);
    sbrk(-PAGESIZE);
    a = sbrk(0);

    return 0;
}
