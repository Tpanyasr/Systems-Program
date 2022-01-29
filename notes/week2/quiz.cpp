#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;



typedef unsigned char BYTE;
typedef struct mystruct
{
    int i;
    char c;
}mystruct;

int main()
{

    char text[] = "Titanic";
    char *p = text;
    char c = *(p + 1);
    printf("%c", c);
    return 0;
}
