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
    
    *p = 0;
    // at the moment that you create the fork everything in your virtual memory gets copied over 
    if(fork() == 0)//child process
    {
        cout << "I am the child" << endl;
        *p = 6;
        return (0);


    }
    else //parent process
    {
        i = 999;
        cout << "I am the parent" << endl;
    }
    wait(0);

    cout <<"p = " << *p <<endl;
    munmap(p, 4);
    return 0;
}
