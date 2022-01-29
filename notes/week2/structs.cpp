#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;


class testc
{
    public:
    int i = 12345;
    float f = 1.0f;
};
typedef unsigned char BYTE;

int main()
{
    testc a;
    BYTE*p = (BYTE *)&a;
    int *ip = (int *)p;
    float *fp = (float *)(p+4);
    cout <<*ip <<endl;
    cout <<*fp <<endl;

    return 0;
}