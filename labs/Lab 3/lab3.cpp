#include <stdlib.h>
#include <stdio.h>
#define PAGESIZE 1024

unsigned char myheap[1048576];


typedef struct chunkhead{
    unsigned int size;
    unsigned int info = 0;
    unsigned char *next, *prev = NULL;
}chunkhead;

chunkhead *head = (chunkhead *)myheap;



unsigned char* mymalloc(int size){
  
    struct chunkhead *cpy = head;
   
    //first check how many pages
    int pages = size/PAGESIZE;
    if((size % PAGESIZE)!=PAGESIZE){
        pages++;
    }
    int final_size= (PAGESIZE *pages);
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


void myfree(unsigned char *address){
    chunkhead *chunk= (chunkhead*)address - sizeof(chunkhead);
    chunkhead *prev = (chunkhead *)chunk->prev;
    chunkhead *next = (chunkhead *)chunk->next;
    chunk->info = 0;
    if((prev != 0 && next != 0) && prev->info == 0 && next->info == 0)
    {  //if freeing between two occupied chunkheads
        prev->next = (unsigned char *)next;
    }
    else if(next != 0 && next->info == 0)
    {  //if next is free but prev is not free
        chunk->next = next->next;
        chunk->size = next->size + chunk->size;
    }
    else if(prev != 0 && prev->info == 0)
    {
      prev->next = (unsigned char *)next;
      prev->size =  prev->size + chunk->size;
      //if prev is free and next isn't
    }
    return;
}

void analyse()
{
    chunkhead *temp = head;
    int i = 1;
    while(temp != 0)
    {
        printf("Chunk #%d:\n", i++);
        printf("Size = %d bytes\n", temp->size);
        if(temp->info == 0)
            printf("Free\n");
        else
            printf("Occupied\n");
        printf("Next = %p\n", temp->next);
        printf("Prev = %p\n\n",temp->prev);
        temp = (chunkhead *)temp->next;
    }
    return;
}
int main(){
    
head->info = 0;
    head->size = 1048576 - sizeof(chunkhead);
    head->next = head->prev = 0;

    // unsigned char *a,*b,*c,*d, *e, *f, *g, *h;
    // a = mymalloc(1000);
    // b = mymalloc(100000);

    unsigned char *a,*b,*c;
    a = mymalloc(1000);
    b = mymalloc(1000);
    c = mymalloc(100000);
    myfree(c);
    analyse();
    return 0;


}