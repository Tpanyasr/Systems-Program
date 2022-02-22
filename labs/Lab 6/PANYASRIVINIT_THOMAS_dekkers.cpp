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
    int *thread1wantstoenter = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *thread2wantstoenter = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    int *favoredthread = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    *thread1wantstoenter = 1;
    *thread2wantstoenter = 1;
    *favoredthread = 1;
    // 1 is false 0 is true thread[0] is child thread[1] is parent

    char t1[] = "The wiser gives in! A sad truth, it establishes the world domination of stupidity.\n";
    char t2[] = "We look for the truth, but we only want to find it where we like it.\n";
    char outtext [1000];
    

    if(fork() == 0)
    {
        for(int i = 0;;i++)
        {
            *thread1wantstoenter = 0;
             while(*thread2wantstoenter == 0)
            {
                if(*favoredthread == 2)
                {
                    *thread1wantstoenter = 1;
                    while(*favoredthread ==2);
                    *thread1wantstoenter = 0;
                }
            }

            if(i%2 ==0)
            {
                strcpy(text, t1);
                 
            }
            else
            {
                while(*thread2wantstoenter == 0)
            {
                if(*favoredthread == 2)
                {
                    *thread1wantstoenter = 1;
                    while(*favoredthread ==2);
                    *thread1wantstoenter = 0;
                }
            }
                // while((*flag1 = 1) && (*turn == 1));
                strcpy(text, t2);
               
            }
            *favoredthread = 2;
            *thread1wantstoenter = 1;
            
        }
    }
    else
    {
        while(1)
        {
            sleep(2); // included sleep because when i run this without it, my entire virtual machine crashes.
            *thread2wantstoenter = 0;
             while(*thread1wantstoenter == 0)
            {
                if(*favoredthread == 1)
                {
                    *thread2wantstoenter = 1;
                    while(*favoredthread ==1);
                    *thread2wantstoenter = 0;
                }
            }
            strcpy(outtext, text);
            printf("%s\n", outtext);
            *thread1wantstoenter = 1;

            *favoredthread = 1;
            *thread2wantstoenter = 1;
        }
    }
    return 0;
}
