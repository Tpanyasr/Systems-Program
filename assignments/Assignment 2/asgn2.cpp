#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
using namespace std;
#define PAGESIZE 1024
typedef unsigned char BYTE;
int heapsize = 0;
chunkhead *startofheap = NULL;


typedef struct chunkhead{
    unsigned int size = size;
    unsigned int info = 0;
    unsigned char *next, *prev = NULL;
}chunkhead;

chunkhead* get_last_chunk() //you can change it when you aim for performance 
 {   
    if(!startofheap) //I have a global void *startofheap = NULL; 
      {
           return NULL;   
      }
    
    chunkhead* ch = (chunkhead*)startofheap;

    for (; ch->next; ch = (chunkhead*)ch->next); 
        return ch; 
 } 
 
 void analyze() 
 { 
    printf("\n--------------------------------------------------------------\n"); 
    if(!startofheap) 
    { 
        printf("no heap"); 
    return; 
 } 



 chunkhead* ch = (chunkhead*)startofheap; 
 for (int no=0; ch; ch = (chunkhead*)ch->next,no++) 
  { 
  printf("%d | current addr: %x |", no, ch); 
  printf("size: %d | ", ch->size); 
  printf("info: %d | ", ch->info); 
  printf("next: %x | ", ch->next); 
  printf("prev: %x", ch->prev); 
  printf("      \n"); 
  } 
printf("program break on address: %x\n",sbrk(0)); 
 } 

void myfree(BYTE *a)
{

}



chunkhead* bestfit(int size)
{

    chunkhead * cpy = (chunkhead *)startofheap;
    chunkhead *best = NULL;
    while(cpy)
    {
        if(cpy ->size >= size && cpy->info == 0)
        {
            if(cpy->size == size)
                return cpy;
            if(best == NULL)
                best = cpy;
            if(best ->size > cpy->size)
                best = cpy;
         }
         else{
             cpy = (chunkhead *)cpy->next;   
         }
         
    }
    return best;
}
BYTE* mymalloc(int size){
  
    chunkhead *cpy = (chunkhead *)startofheap;
    chunkhead *best = NULL;
   
    
        //first check how many pages
    int pages = (size + sizeof(chunkhead))/PAGESIZE;
    if((size % PAGESIZE)!=PAGESIZE){
        pages++;
    }
    int final_size= (PAGESIZE *pages);


    if(heapsize == 0)
    {
        startofheap = (chunkhead *)sbrk(0);
        sbrk(size);
        cpy = startofheap;
        cpy->next, cpy->prev = NULL;
        cpy->info = 1;
        cpy->size = size;
        return (BYTE *)cpy + sizeof(chunkhead);
    }
    //find the best fit chunk
    best = bestfit(size);

    if(best != NULL)
    {       
        if(best->size - final_size ==0)
        {

            best->info = 1;
            heapsize += final_size;
            return (BYTE *)best + sizeof(chunkhead);
        }
        else
        {   

            //sbreak doesnt need to be used here because we are splitting already allocated memory
            int remaining = best->size - final_size;  //set up remainder for the second splitted chunk
            //address of new chunk = previous address + size of struct + size
            chunkhead *new_chunk = (chunkhead *)(BYTE *)(best+ sizeof(chunkhead) + best -> size);            
            heapsize += final_size;   
            best->size = final_size;
            best->info = 1;
            best->next = (BYTE *) new_chunk;

            new_chunk->info = 0;
            new_chunk->size = remaining;
            new_chunk->prev = (BYTE *)best;
            new_chunk->next = best->next;

        return (BYTE *)best + sizeof(chunkhead);                          


        }
    }
    else //case where a best existing chunkhead could not be found so we must create a new chunkhead and move the program break
    {
        best = (chunkhead *)sbrk(0);
        best->info = 1;
        best->size = final_size;
        best->prev = (BYTE *)get_last_chunk();
        best->next = NULL;
        sbrk(final_size);
        heapsize += final_size;
        //create new chunkhead 



    }

        return (BYTE *)best + sizeof(chunkhead);                          
}

void main() 
{
    BYTE*a[100]; 
    analyze();//50% points 
    for(int i=0;i<100;i++) 
        a[i]= mymalloc(1000); 
    for(int i=0;i<90;i++) 
        myfree(a[i]);   
    analyze(); //50% of points if this is correct 
    myfree(a[95]); 
    a[95] = mymalloc(1000); 
    analyze();//25% points, this new chunk should fill the smaller free one 
    //(best fit) 
    for(int i=90;i<100;i++) 
        myfree(a[i]); 
    analyze();// 25% should be an empty heap now with the start address //from the program start
}
