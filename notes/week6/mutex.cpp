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
    char t1[] = "The wiser gives in! A sad truth, it establishes the world domination of stupidity.\n";
    char t2[] = "We look for the truth, but we only want to find it where we like it.\n";
    char outtext [1000];
    if(fork() == 0)
    {
        for(int i = 0;;i++)
        {
            if(i%2==0)
            {
                strcpy(text, t1);
            }
            else
            {
                strcpy(text, t2);
            }
        }
    }
    else
    {
        while(1)
        {
            strcpy(outtext, text);
            printf("%s\n", outtext);
        }
        wait(0);
    }
    return 0;
}
