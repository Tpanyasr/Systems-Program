#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/mman.h>
#include <vector>
using namespace std;


typedef unsigned char BYTE;
#define MYFLAG_A 1 //00000001
#define MYFLAG_B 2 //00000010
#define MYFLAG_C 4 //00000100
#define MYFLAG_D 8 //00001000


void fct(int flag)
{
    //...
}
int main()
{
    
   // fct(MYFLAG_A | MYFLAG_D); //00001001
   //mmap returns null and errno if it doesn't have map anonymous.
   BYTE *p = (BYTE *) mmap(NULL, 4 ,PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   
    //munmap is the corresponding free for mmap so you need to destroy w mmap once
   munmap(p, 4);
    return 0;
}
