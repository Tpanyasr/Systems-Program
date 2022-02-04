#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;


struct listelement
{
    listelement *next, *prev;
    char text[1000];
};

listelement *head = NULL;

listelement *lastelem(){

listelement *p = NULL;
    for(p = head; p->next!=NULL; p=p->next)
    {

    }
    return p;
}
int start()
{
    int input = 0;
    printf("1 push string\n2 print list\n3 delete item\n4 end program\n\nEnter number: ");
    scanf("%d", &input);
    return input;
}
int  main()
{
    int input = 0;
    for(;;)
    {
        int input = start();
        //print list
        if(input == 1)
        {

            listelement *n = new listelement;
            if(head ==NULL)
            {
            head = n; //if head == NULL set new listelement to head
            printf("insert text: ");
            scanf("%s", n->text);
            printf("\n");
            head ->next = NULL;
            head->prev = NULL;
            }
            else
            {
                printf("insert text: ");
                listelement *last = lastelem();
                scanf("%s", n->text);
                last->next = n;
                n->prev = last;
                n->next = NULL;
                printf("\n");
                printf("done push string\n\n");
            }
        }
        if(input == 2)
        {
            if(head ==NULL)
                printf("nothing to print out\n\n");
            else
            {
                printf("\nlist:\n");
            //iterate through the list and delete specific index
                for(listelement *p = head; p!= NULL; p =p->next)
                {
                    printf("%s", p->text);
                    printf("\n");
                }
                printf("\n");
            }
        }
        if(input == 3)
        {
            int index = 0;
            int counter = 0;
            printf("Delete which index?(first item is 1): ");
            scanf("%d", &index);
            if(index <= 0){
                printf("Index does not exist\n\n");
            }
            else if(index == 1 && index >=0)
            {
                if(head->next!= NULL){
                    head = head->next;
                }
                else
                {
                    head = NULL;
                }
            }
            else if (index >=0)
            {
                //iterate through the list and delete specific index
                for(listelement *p = head; p != NULL; p =p->next)
                {
                    if(counter < index)
                        counter++;
                    if(counter == index)
                    {
                        listelement *prev = p->prev;
                        
                        if(p->next)
                        {
                        listelement *next_element = p->next;
                        prev->next = next_element;
                        }
                        else
                        {
                            prev->next = NULL;
                        }
                        
                    }
                }
                if(index>counter){
                    printf("Index does not exist \n\n");
                }
            }
        }
            //delete item in the linked list
        if(input == 4)
        {
            //delete all items in the linked list
            listelement *new_head = NULL;
            while(head){
                new_head = head->next;
                free(head);
                head = new_head;
            }
            return 0;
        }
    }
    return 0;
return 0;
}
