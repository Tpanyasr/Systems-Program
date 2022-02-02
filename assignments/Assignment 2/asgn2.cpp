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
int heapsize;
void *startofheap = NULL;

typedef struct chunkhead{
    unsigned int size = heapsize;
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

BYTE *mymalloc(int size)
{

// Based on lab 3, remove the static heap array and use brk() and sbrk() commands to make the 
// heap. Also, imlement “best fit”. 
 
// Howto: 
 
//  Be aware, that the heap size starts at 0, which means, you don’t even have a chunk 
// header. Make sure to catch that condition! Maybe with a global variable “heapsize”? 
//  From here on, when mymalloc is called, move the program break as far as you need to 
// allocate your first chunk.  
//  This time make sure, that the chunkhead is part of the PAGESIZE! Also, assume the 
// PAGESIZE is 4096 BYTEs. 
//  From now on, move through the chunklist similar to lab3, but if no chunk is free, move 
// the program break further and assign a new chunk! 
//  If some earlier chunk is free and you allocate less size than that, split as usual. 
//  Move the program break back again when the last chunk in the list is removed! 
//  You analyse function should print the address of the program break! 
//  Best fit: Search the chunks for the smallest possible chunk which satisfies the request. 

return NULL;

}



BYTE* bestfit(int size)
{

    chunkhead * cpy = (chunkhead *)startofheap;
    chunkhead *best = NULL;
    while(cpy)
    {
        if(cpy ->size >= size && cpy->info == 0)
        {
            if(cpy->size == size)
                return (BYTE *)cpy;
            if(best == NULL)
                best = cpy;
            if(best ->size > cpy->size)
                best = cpy;
         }
         else{
             cpy = (chunkhead *)cpy->next;   
         }
         
    }
}
BYTE* mymalloc(int size){
  
    chunkhead *cpy = (chunkhead *)startofheap;
    chunkhead *best = NULL;
   
    
    //if the head is null then initialize the first chunkhead and sbrk
    if(!cpy){
        cpy = (chunkhead *)sbrk(size);
        cpy->size = size;
        cpy->info = 1;
        cpy->next = 0; 
        cpy->prev = 0;

    }
    //first check how many pages
    int pages = (size + sizeof(chunkhead))/PAGESIZE;
    if((size % PAGESIZE)!=PAGESIZE){
        pages++;
    }
    int final_size= (PAGESIZE *pages);

    //find the best fit chunk


    while(cpy)
    {                                    //iterate through all chunks
        if(cpy->size >= final_size && cpy->info == 0)
        {     //free space found
    
            int remaining = cpy->size-final_size - sizeof(chunkhead);        
            cpy->size= final_size;                      
            cpy->info=1;                           //set occupied 
            if(remaining != 0 && remaining >= size)
            {                            
               //address of new chunk = previous address + size of struct + size
                chunkhead *new_chunk = (chunkhead *)( (unsigned char *) cpy + sizeof(chunkhead) + cpy -> size);
                new_chunk->size = remaining;
                new_chunk->info = 0;
                new_chunk->prev = (unsigned char*) cpy;
                chunkhead *next = (chunkhead *)new_chunk->next;
                 if(new_chunk->next){
                    new_chunk->next = cpy->next;
                    next->prev = (unsigned char *)new_chunk;
                }
                cpy->next = (unsigned char *)new_chunk;     
            }
               
            return (unsigned char*)(cpy + sizeof(chunkhead)); //returns the address of the new memory
        }
        else 
        {                                  
            cpy = (chunkhead *)cpy->next;                      
        }
    }
    unsigned char * return_address = NULL;    
    return return_address;                                
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
