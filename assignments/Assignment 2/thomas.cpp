#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


#define PAGESIZE 4096

typedef unsigned char byte;

typedef struct chunkhead 
{ 
    unsigned int size; 
    unsigned int info; 
    byte *next,*prev;
}chunkhead; 


chunkhead *head = NULL;
int heapsize = 0;

void *prgBrk;


byte *mymalloc(unsigned int size)
{
    int i;
    unsigned int needed;
    unsigned int remaining;
    
    chunkhead *temp = head;
    chunkhead *p;
    chunkhead newChunk;
    chunkhead *best = NULL;
    
    byte *insert; // used to convert temp or best into a byte pointer to index without steps of sizeof(chunkhead)
    
    for(i = 1; (i * PAGESIZE) < (size + sizeof(chunkhead)); i++); // finds the correct amount of pages needed
    
    needed = (i * PAGESIZE) - sizeof(chunkhead); // the needed size of memory

    if(heapsize == 0) // if heap is empty
    {
        head = (chunkhead *) sbrk(0);
        sbrk(i * PAGESIZE);
        prgBrk = sbrk(0);
        newChunk.info = 1;
        newChunk.size = needed;
        newChunk.next = newChunk.prev = 0;
        *(head) = newChunk;
        heapsize += (i * PAGESIZE);
        return (byte *)(head + 1);
    }

    while(temp != 0) // finds the best available chunkhead of correct size
    {
        if(temp->info == 0 && temp->size >= needed)
            if(best == NULL)
                best = temp;
            else if(best->size > temp->size)
                best = temp;
            
        p = temp;
        temp = (chunkhead *)temp->next;
    }
    
    if(best != NULL) // insert if there is correctly sized free chunk 
    {
        if(best->size - needed == 0) // inserting in a chunk of exact size;
        {
            best->info == 1;
            heapsize += (i * PAGESIZE);
            return (byte *)(best + 1);
        }
        else // inserting and splitting the memory
        {
            remaining = best->size - (i * PAGESIZE);
            insert = (byte *)best;
            
            
            newChunk.info = 0; // sets up the split chunk
            newChunk.size = remaining - (sizeof(chunkhead));
            newChunk.prev = insert;
            newChunk.next = best->next;

            heapsize += best->size + sizeof(chunkhead);

            best->size = needed; // changes the open chunk to spec
            best->info = 1;
            insert = insert + sizeof(chunkhead) + best->size;
            best->next = insert; 

            temp = (chunkhead *)insert; //inserts the chunk in to memory
            *(temp) = newChunk;
            
            

            return (byte *)(temp + 1);
        }
    }
    else // inserts at the end of the heap by extending the heap size
    {
        newChunk.info = 1; //sets up allocated chunk
        newChunk.size = needed;
        newChunk.prev = (byte *)p;
        newChunk.next = 0;

        sbrk(i * PAGESIZE); // allocates memory to heap
        heapsize += (i * PAGESIZE);
        
        temp = (chunkhead*)(prgBrk); // inserts the newChunk chunkhead;
        *(temp) = newChunk;
        prgBrk = sbrk(0);
        p->next = (byte *)temp;
        return (byte *)(temp + 1);

    }
    
}

void myfree(byte *address)
{
    chunkhead *temp = (chunkhead *)(address - sizeof(chunkhead)); // the chunkhead's start should be at the start of memory minus the size of a chunkhead 
    
    temp->info = 0;
    
    chunkhead *Prev = (chunkhead *)temp->prev;
    chunkhead *Next = (chunkhead *)temp->next;


    if(Prev != 0 || Next != 0) // if prev or next exists 
    {
        if(Prev != 0 && Prev->info == 0 && Next != 0 && Next->info == 0) // if both are free
        {
            Prev->size = temp->size + Prev->size + Next->size + (2 * sizeof(chunkhead));
            Prev->next = Next->next;
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Prev != 0 && Prev->info == 0) // if only prev is free
        {
            Prev->size = temp->size + Prev->size + sizeof(chunkhead);
            Prev->next = temp->next;
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Next != 0 && Next->info == 0) // if only next is free
        {
            temp->size = temp->size + Next->size + sizeof(chunkhead);
            temp->next = Next->next;
            heapsize -= temp->size + sizeof(chunkhead);
        }
    }
    else
    {
        heapsize -= temp->size + sizeof(chunkhead);
    }

    if(heapsize == 0) // if heap has no allocated memory move program break;
    {
        brk(head);
        head = NULL;
    }

    return;
} 

void analyze() 
 { 
    fprintf(stderr,"\n--------------------------------------------------------------\n"); 
    if(head == NULL) 
    { 
        fprintf(stderr,"no heap\n"); 
        return; 
    } 
    chunkhead* ch = head; 
    for (int no=0; ch; ch = (chunkhead*)ch->next,no++) 
    { 
        fprintf(stderr,"%d | current addr: %x |", no, ch); 
        fprintf(stderr,"size: %d | ", ch->size + (sizeof(chunkhead))); 
        fprintf(stderr,"info: %d | ", ch->info); 
        fprintf(stderr,"next: %x | ", ch->next); 
        fprintf(stderr,"prev: %x", ch->prev); 
        fprintf(stderr,"      \n"); 
    } 
    cout <<"program break on address" <<sbrk(0)<<endl; 
 } 

int main()
{

    byte *a,*b,*c; 
    a = mymalloc(4096*2); 
    b = mymalloc(1000);
    
    analyze();

    myfree(a);
    c = mymalloc(1000);

    analyze();
    myfree(b);
    myfree(c);
    return 0;
}