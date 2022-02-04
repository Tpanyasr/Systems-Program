#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define PAGESIZE 4096
typedef unsigned char byte;
typedef struct chunkhead
{
    unsigned int size;
    unsigned int info;
    unsigned char *next, *prev;
}chunkhead;
#define CHUNK_SIZE sizeof(chunkhead)

int heapsize = 0;
chunkhead *startofheap = NULL;

unsigned char *mymalloc(unsigned int size);
void myfree(unsigned char *address);
void analyze();

int main()
{
    byte *a[100];
    clock_t ca, cb;
    ca = clock();
    analyze();
    for(int i = 0; i<100; i++)
        a[i] = mymalloc(1000);
    for(int i = 0; i < 90; i++)
        myfree(a[i]);
    analyze();
    myfree(a[95]);
    a[95] = mymalloc(1000);
    analyze();
    for(int i = 90; i < 100; i++)
        myfree(a[i]);
    analyze();
    cb = clock();
    printf("\nduration: %f\n", (double)(cb - ca)); 
}

unsigned char *mymalloc(unsigned int size)
{
    unsigned int smallest = 0;
    byte *p;
    chunkhead *chc;
    chunkhead *ch = startofheap;
    chunkhead *pch;
    unsigned int pg = (size + CHUNK_SIZE) / PAGESIZE;
    if (pg == 0)
        size = PAGESIZE;
    else
        size = ((int)pg + 1) * PAGESIZE;
    if (heapsize == 0)
    {
        startofheap = (chunkhead*)sbrk(0);
        sbrk(size);
        ch = startofheap;
        ch -> info = 1;
        ch -> prev = NULL;
        ch -> size = size;
        ch -> next = NULL;
        heapsize = size;
        return (byte*)ch + (CHUNK_SIZE);
    }
    if (ch->info == 0 && ch->size == size) //check if head is perfect fit chunk
    {
        ch->info = 1;
        return (byte*)ch + (CHUNK_SIZE);
    }
    for (;(ch->next) != 0; ch = (chunkhead*)(ch->next))
    {
        if (ch->info == 1 || size > ch->size) //looking for previous occupied chunk
            pch = ch;
        else if (ch->info == 0) //looking for perfect fit unoccupied chunk
        {
            if (ch->size == size)
            {
                ch->info = 1;
                return (byte*)ch + CHUNK_SIZE;
            }
            else //we don't have one perfect fit unoccupied chunk but we have extra spaces
            {
                chc = (chunkhead*)((byte*)ch + (ch->size) - size);
                p = (byte*)ch;
                smallest = ch->size;
            }
        }
    }
    if (smallest != 0)
    {
        ch = (chunkhead*)p;
        pch = (chunkhead*)(ch->next);
        chc->info = 1;
        ch->info = 0;
        ch->size -= (size) + CHUNK_SIZE;
        chc->prev = (byte*)ch;
        chc->size = size;
        ch->next = (byte*)chc;
        chc->next = (byte*)pch;
        pch->prev = (byte*)chc;
        return (byte*)ch + CHUNK_SIZE;
    }
    else //no unoccupied spaces
    {
        sbrk(size);
        heapsize += size + CHUNK_SIZE;
        pch = ch;
        ch = (chunkhead*)((byte*)ch + ch->size);
        ch->info = 1;
        ch->size = size;
        ch->next = NULL;
        ch->prev = (byte*)pch;
        pch->next = (byte*)ch;
        return (byte*)ch + CHUNK_SIZE;
    }
    return NULL;
}

void myfree(unsigned char *address)
{
    chunkhead *ch = (chunkhead*)(address - CHUNK_SIZE);
    ch->info = 0;
    chunkhead *chn = ((chunkhead*)(ch->next));
    chunkhead *chp = ((chunkhead*)(ch->prev));
    if (chp == NULL) //free the head
    {
        if (chn->info == 0)
        {
            ch->size += chn->size + CHUNK_SIZE;
            ch->next = chn->next;
            ch->prev = NULL;
            if (((chunkhead*)(ch->next)) != NULL)
                ((chunkhead*)(ch->next))->prev = (byte*)ch;
        }
        else
            ch -> info = 0;
    }
    else if (chn == NULL) //free the last chunk
    {
        heapsize -= ch->size;
        byte *p = sbrk(0);
        brk((byte*)p - ch->size + CHUNK_SIZE);
        if (chp == NULL || heapsize == 0) //free the only chunk
        {
            heapsize = 0;
            brk(startofheap);
            startofheap = NULL;
        }
        if (chp->info == 0)
        {
            if (chp->prev != NULL)
            {
                chp = (chunkhead*)(chp->prev);
                chp->next = NULL;
                brk(sbrk(0) - chp->size);
            }
            else
            {
                heapsize = 0;
                brk(startofheap);
                startofheap = NULL;
            }
        }
        else
            chp->next = ch->next;
    }
    else if (chn->info == 0 && chp->info == 0)
    {
        ch->size += (chn->size) + (chp->size) + CHUNK_SIZE*2;
        if (((chunkhead*)(chn->next)) != NULL)
            ((chunkhead*)(chn->next))->prev = (byte*)ch;
        if (((chunkhead*)(chp->prev)) != NULL)
            ((chunkhead*)(chp->prev))->next = (byte*)ch;
        ch->next = chn->next;
        ch->prev = chp->prev;
        chp->size = ch->size;
        chp->next = ch->next;
        chp->prev = ch->prev;
    }
    else if (chn->info == 0 || chp->info == 0)
    {
        if (chn->info == 0)
        {
            ch->size += chn->size + CHUNK_SIZE;
            ch->next = chn->next;
            if (((chunkhead*)(ch->next)) != NULL)
                ((chunkhead*)(ch->next))->prev = (byte*)ch;
        }
        else
        {
            chp->size += ch->size;
            if (((chunkhead*)(chp->prev)) != NULL)
                ((chunkhead*)(chp->prev))->next = (byte*)chp;
            if (((chunkhead*)(chn->next)) != NULL)
                ((chunkhead*)(ch->next))->prev = (byte*)chp;
            chp->next = ch->next;
            chn->prev = (byte*)chp;
        }
    }
}

void analyze()
{
    printf("\n--------------------------------------------------------------\n");
    if (!startofheap)
    {
        printf("no heap, program break on address: %x\n", sbrk(0));
        return;
    }
    chunkhead* ch = (chunkhead*)startofheap;
    for(int no = 0; ch; ch = (chunkhead*)ch->next, no++)
    {
        printf("%d | current addr: %x |", no, ch);
        printf("size: %d | ", ch->size);
        printf("info: %d | ", ch->info);
        printf("next: %x | ", ch->next);
        printf("prev: %x", ch->prev);
        printf("      \n");
    }
    printf("program break on address: %x\n", sbrk(0));
}