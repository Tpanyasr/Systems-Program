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
using namespace std;

typedef unsigned char BYTE;

int main()
{
if(fork() == 0)
    {
        if(fork() == 0)
        {
            cout <<"A"<<endl;
            return 0;
        }
        wait(0);
        cout <<"B"<<endl;
        return 0;
    }
    else
    {
        wait(0);
        cout << "C"<<endl;

    }
    return 0;
}
