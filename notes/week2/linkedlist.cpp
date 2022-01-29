

#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

class lle
{
    public:
    int i;
    lle *next;
    lle *prev;
    lle()
    {
        next = NULL;
    }
};
int  main()
{
    lle *head = NULL;
    for(;;)
    {
        int input;
        cin >> input;
        if(input == 0)
        {
            //remove (free or delete) every list element
            return 0;
        }
        if(input == 1){
            for(lle *p = head; p != NULL; p =p->next)
            {
                cout <<p->i <<endl;
            }
        }//print list
        if(input == 2)
        {
            lle *n = new lle;
            if(head ==NULL)
            {
            head = n; //if head == NULL
            }
            else
            {
                lle *p = NULL;
                for(p = head; p->next!=NULL; p=p->next)
                {
                    p->next = n;
                    n->prev = p;
                }
            }
        }
    }
    return 0;
}
