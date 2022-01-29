#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;



typedef unsigned char BYTE;

// class string
// {
//     private:
//     char *text;
//     public:
//     char *c_str(){return text;}
// };

int main()
{
    
    char text[1000];
    strcpy(text, "hello world");
    string a;

    a = "hello world ";

    a+= "my name is Gobo";
    //a.find();
    string b = a.substr(3, 5);
    strcpy(text, b.c_str());
    cout << b << endl;
    
    return 0;
}
