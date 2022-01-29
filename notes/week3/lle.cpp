#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;


template <class A> //data part is unknown and is defined later in the code

class lle
{
    public:
    A data;
    char text[100];
    lle *prev, *next;
    lle()
    {
        prev = next = NULL;
    }
};

int main()
{
    
    // lle <int> a;
    // a.data = 8;

    // lle<string> b;

    // b.data = "hello";

    // vector<int> numbers;
    // vector<int> *numbers = new vector<int>[100]; 
    //you can create an array of vector classes if you want to make it complex

    // numbers.push_back(5);
    // numbers.push_back(4);
    // numbers.push_back(7);

    // numbers.push_back(99);


    //numbers.resize(10); can define a fixed size and still grow it.


    //vectors vanish as soon as the function ends and are self managing

    vector<string> numbers;

    numbers.push_back("hello");
    numbers.push_back("world");
    numbers.push_back("hello again");

    for(int i = 0; i <numbers.size(); i++)
        cout << numbers[i] <<endl;




    return 0;
}
