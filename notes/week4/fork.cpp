#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
using namespace std;


typedef unsigned char BYTE;

int main()
{
    int i = 0;
    int *p = (int *) mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int * childPIDs = (int *)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);  // list of children pids 

    
    *p = 0;
    int child = fork();
    childPIDs[0] = child;
    // at the moment that you create the fork everything in your virtual memory gets copied over 
    if(child == 0)//child process
    {
        return 0;

    }
    else //parent process
    {
        cout << childPIDs[0]<<endl;
        wait(0);
        i = 999;
        cout << "I am the parent" << endl;
    }
    
   
   
    
    munmap(p, 4);
    return 0;
}
