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
    int y = 0;
    clock_t a = clock();
    cout << a <<endl;

    for(int i; i < 100000; i++)
    {
        y++;
    }
    a = clock();
    cout << a << endl;
    return 0;
}
