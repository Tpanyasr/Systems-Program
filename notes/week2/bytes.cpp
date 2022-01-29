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
    int o = 8;
    float *fp = (float *)&o;
    //be careful because all the memory addresses are integers 
    //you can have a floating point address pointing at the same
    //integer but it will get interpreted as a float
    float j = *fp;
    cout << &o<< endl;
    for(int i = 0; i <8; i++)
       // cout << (int)p[i] <<endl;

    return 0;
}