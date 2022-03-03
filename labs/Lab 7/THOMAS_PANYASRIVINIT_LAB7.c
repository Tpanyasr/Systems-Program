#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>


typedef unsigned char byte;


typedef struct mypipe 
 { 
 byte* pipebuffer; 
 int buffersize; 
 int start_occupied; 
 int end_occupied; 
 } mypipe; 


//initializes (malloc) the pipe with a size of "size" and sets start and end. 
void init_pipe(mypipe* pipe, int size); 
 
 
//writes "size" bytes from buffer into the pipe, returns size 
int mywrite(mypipe *pipe, byte* buffer, int size); 
 
//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty 
int myread(mypipe* pipe, byte* buffer, int size); 



int main()
{

    byte text[100]; 
    mypipe pipeA; 
    
    init_pipe(&pipeA, 32); 
   // mywrite(&pipeA, "hello world", 12); 
 //   mywrite(&pipeA, "it's a nice day", 16); 
    
    myread(&pipeA, text, 12); 
    printf("%s\n", text); 
    myread(&pipeA, text, 16); 
    printf("%s\n", text); 
    
   // mywrite(&pipeA, "and now we test the carryover", 30); 
    myread(&pipeA, text, 30); 
    printf("%s\n", text);
        
    //free pipe buffer
    free((void *) (pipeA.pipebuffer));

    return 0;
}


//initializes (malloc) the pipe with a size of "size" and sets start and end. 
void init_pipe(mypipe* pipe, int size)
{
    pipe->buffersize = size;
    pipe->end_occupied = 0;
    pipe->start_occupied = 0;
    pipe->pipebuffer = (byte *)malloc(sizeof(byte) * size);
    return;

    }
 
//writes "size" bytes from buffer into the pipe, returns size 
int mywrite(mypipe *pipe, byte* buffer, int size)
{
    
    //starts writing from the end until all bytes are written or space is full/we reach start index
    int sizeoriginal = size; 
    int i = 0;
    while(size > 0)
    {
        int index = (pipe->end_occupied + i)%pipe->buffersize;
        //if we loop back and try to write in the index of the start, break;
        if(index == pipe->start_occupied && size != sizeoriginal )
        {
                pipe->end_occupied = (pipe->end_occupied + i) % pipe->buffersize;
                return i;
        }
        pipe->pipebuffer[index] = buffer[i];
        i++;
        size--;

    }
    //
    pipe->end_occupied = (pipe->end_occupied + i) % pipe->buffersize;
    //returns the bytes read
    return i;
    
    
}


 
//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty 
int myread(mypipe* pipe, byte* buffer, int size)
{

    int i = 0;
    int index;
    while(size >0)
    {
   
        index = (pipe->start_occupied + i) % pipe->buffersize;
        //if there are more bytes that user wants to be read than are available, return bytes read. 
        if(pipe->end_occupied == index)
        {
            return i;
        }
        else
        {
            buffer[i] = pipe->pipebuffer[index];
            size--;
            i++;
        }
    }
    //the new starting index of the pipe where the read ends. 
    pipe->start_occupied = (pipe->start_occupied + i) % pipe->buffersize;
    return size;
}



