#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;


class students
{
    public:
    char name[100];
    char major[100];
};
typedef unsigned char BYTE;

int main()
{
    students *p = NULL;
    p = (students*) malloc(sizeof(students)*10); //allocates space for 10 students
    //c++ notation is p = new students[10]
    cin >> p[0].major;
    cout << (*p).major <<endl;
    free(p);
    //c++ notation is "delete[]p" for an array OR "delete p" for one student 
    return 0;
}