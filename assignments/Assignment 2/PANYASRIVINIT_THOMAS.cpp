#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
using namespace std;
#define PAGESIZE 4096
typedef unsigned char BYTE;


typedef struct chunkhead{
    unsigned int size = size;
    unsigned int info = 0;
    unsigned char *next, *prev = NULL;
}chunkhead;

int heapsize = 0;
chunkhead *startofheap = NULL;


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
        printf("no heap, "); 
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


void myfree(BYTE *address){
    
    chunkhead *addr = (chunkhead*)address - sizeof(chunkhead);
    chunkhead *prev = (chunkhead *)addr->prev;
    chunkhead *next = (chunkhead *)addr->next;
   
}


BYTE* mymalloc(int size){
  
    chunkhead *cpy = startofheap;
    chunkhead *best;
    chunkhead *current;
    chunkhead *new_chunk;
    BYTE *p;
    int smallest= 0;
   
    
        //first check how many pages
    int pages = (size + sizeof(chunkhead))/PAGESIZE;
    if((size % PAGESIZE)!=PAGESIZE){
        pages++;
    }
    size= (PAGESIZE *pages);


    if(heapsize == 0)
    {
        startofheap = (chunkhead *)sbrk(0);
        sbrk(size);
        cpy = startofheap;
        cpy->next, cpy->prev = NULL;
        cpy->info = 1;
        cpy->size = size;
        heapsize = size;
        return (BYTE *)cpy + sizeof(chunkhead);
    }
    if (cpy->info == 0 && cpy->size == size) //check if startofheap is perfect fit chunk
    {
        cpy->info = 1;
        return (BYTE*)cpy + (sizeof(chunkhead));
    }
    //find the best fit chunk
    while(cpy)
    {
        if (cpy->info == 0) //looking for perfect fit unoccupied chunk
        {
            if (cpy->size == size)
            {
                cpy->info = 1;
                return (BYTE*)cpy + sizeof(chunkhead);
            }
            else 
            {
                best = (chunkhead*)((BYTE*)cpy + (cpy->size) - size);
                p = (BYTE*)cpy;
                smallest = cpy->size;
            }
        }

        cpy = (chunkhead*)(cpy->next);

    }

    if(smallest)
    {       


            //sbreak doesnt need to be used here because we are splitting already allocated memory
            //address of new chunk = previous address + size of struct + size
            cpy = (chunkhead *)p;
            cpy->size -= size + sizeof(chunkhead); 
            new_chunk = (chunkhead*)cpy->next;
            cpy->info = 0;
            best->prev = (BYTE*)cpy;
            best->size = size;
            best->info = 1;
            best->next = (BYTE *)new_chunk;
            cpy->next = (BYTE *)best;
            new_chunk->prev = (BYTE *)cpy;
        return (BYTE *)best + sizeof(chunkhead);                          


        
    }
    else //case where a best existing chunkhead could not be found so we must create a new chunkhead and move the program break
    {
        cpy = startofheap;
        sbrk(size);
        heapsize += size + sizeof(chunkhead);
        new_chunk = cpy;
        cpy = (chunkhead*)((BYTE*)cpy + cpy->size);
        cpy->info = 1;
        cpy->size = size;
        cpy->next = NULL;
        cpy->prev = (BYTE*)new_chunk;
        new_chunk->next = (BYTE*)cpy;
        return (BYTE*)cpy + sizeof(chunkhead);
    }
    return NULL;
}


int main() 
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
    return 0;
}
