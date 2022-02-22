#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

typedef unsigned char BYTE;


//race conditions
//mutex
//critical section


//deadlock is when both processes are locked out of the critical section 
int main()
{
    char *text = (char *)mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *flag0 = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *flag1= (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *turn = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    char t1[] = "The wiser gives in! A sad truth, it establishes the world domination of stupidity.\n";
    char t2[] = "We look for the truth, but we only want to find it where we like it.\n";
    *flag0 = 0;
    *flag1 = 0;
    *turn =0;
    char outtext [1000];
    

    if(fork() == 0)
    {
        for(int i = 0;;i++)
        {
            *flag0 = 1;
            *turn = 1;
            while((*turn == 1) && (*flag1 == 1));
            if(i%2 ==0)
            {
                strcpy(text, t1);
                *flag0 = 0;  
            }
            else
            {
                while((*flag1 = 1) && (*turn == 1));
                strcpy(text, t2);
                *flag0 = 0;

            }
        }
    }
    else
    {
        while(1)
        {
            *flag1 = 1;
            *turn = 0;
            while(*flag0 = 1 && (*turn == 0));
            strcpy(outtext, text);
            printf("%s\n", outtext);
            *flag1 = 0;
        }
    }
    return 0;
}
