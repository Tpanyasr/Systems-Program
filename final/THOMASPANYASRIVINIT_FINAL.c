#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>
#define ARRAY_LENGTH 32

void synch(int par_id, int par_count, int *ready);
//************************************************************************************************************************
// sets one element of the matrix
void init_elem(int * array)
{
    srand(getpid());
    for(int i = 0; i < ARRAY_LENGTH; i++)
    {
        array[i] = (rand() %1000);
        int b = array[i];
    }
}
//************************************************************************************************************************
//make a copy of the array

void copyarr(int *array, int *copy)
{
    for(int i = 0; i < ARRAY_LENGTH; i++)
    {
        copy[i] = array[i];
    }
}
//************************************************************************************************************************
void synch(int par_id, int par_count, int *ready)
{
    // printf("entered synch id: %d\n", par_id);
    int synchid = ready[par_count]+1;  
    ready[par_id]=synchid; 
    int breakout = 0; 
    while(1) 
    { 
        breakout=1;     
        for(int i=0;i<par_count;i++)  
        { 
            if(ready[i]<synchid) 
            {
                breakout = 0;
                break;
            } 
        } 
        if(breakout==1) 
        { 
            ready[par_count] = synchid;        
            break; 
        } 
    }
}
//************************************************************************************************************************
// lets see if both are the same
int compare_array(int *A,int *B)
{
    for(int a = 0;a<ARRAY_LENGTH;a++)
        if(A[a]!=B[a]) 
            return 0;
    
    //return 1 for 1 0 for 0
    return 1;
}
//************************************************************************************************************************
//print a matrix
void print_array(int *C)
{
    printf("\n[");
    for(int a = 0;a<ARRAY_LENGTH-1;a++)
    {
        printf(" %d,",C[a]);
    }
    printf(" %d", C[ARRAY_LENGTH-1]);
    printf("]\n\n");

}
//************************************************************************************************************************
// odd even sort an array of random integers

void oddEvenSort(int arr[], int n)
{
    int isSorted = 0; // Initially array is unsorted
 
    while (!isSorted)
    {
        isSorted = 1;

        // Perform Bubble sort on odd indexed element
        for (int i=1; i<=n-2; i=i+2)
        {

            if (arr[i] > arr[i+1])
             {
                int saveint = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = saveint;
                isSorted = 0;
              }
        }
 
        // Perform Bubble sort on even indexed element
        for (int i=0; i<=n-2; i=i+2)
        {
            int a = arr[i];
            int b = arr[i+1];
            if (arr[i] > arr[i+1])
            {
                int saveint = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = saveint;
            }
        }
    }
}
//************************************************************************************************************************
void sort_parallel(int *arr, int par_id, int par_count, int * ready)
{
    for(int a = 0; a < ARRAY_LENGTH; a++)
    {

        for (int i = (2 * par_id) +1; i < ARRAY_LENGTH-1; i += (2 * par_count))
        {
            int first = arr[i];
            int second = arr[i + 1];

            if (first > second)
            {
                arr[i] = second;
                arr[i + 1] = first;
            }
        }
        synch(par_id, par_count, ready);
        for (int i = (2 * par_id); i < ARRAY_LENGTH-1; i += (2 * par_count))
        {
            int first = arr[i];
            int second = arr[i + 1];

            if (first > second)
            {
                arr[i] = second;
                arr[i + 1] = first;
            }
        }
        synch(par_id, par_count, ready);

    }
}

//************************************************************************************************************************
int main(int argc, char *argv[])
{
    int * copy = (int *)malloc(sizeof(int)*ARRAY_LENGTH);
    int par_id = 0;    // the parallel ID of this process
    int par_count = 1; // the amount of processes
    int *arr;  // matrices A,B and C
    int *ready;        // needed for synch

    if (argc != 3)
    {
        printf("no shared\n");
    }
    else
    {
        par_id = atoi(argv[1]);
        par_count = atoi(argv[2]);
    }
    if (par_count == 1)
    {
        printf("only one process\n");
    }

    int fd[2];

    if (par_id == 0)
    {

        fd[0] = shm_open("array", O_RDWR | O_CREAT, 0777);
        ftruncate(fd[0], ARRAY_LENGTH * sizeof(int));
        arr = (int *)mmap(NULL, ARRAY_LENGTH * sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd[0], 0);

        fd[1] = shm_open("readyarr", O_RDWR | O_CREAT, 0777);
        ftruncate(fd[1], 1 * sizeof(int));
        ready = (int *)mmap(NULL, par_count * sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd[1], 0);
    }
    else
    {
        sleep(2);
        fd[0] = shm_open("array", O_RDWR, 0777);
        arr = (int *)mmap(NULL, ARRAY_LENGTH * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd[0], 0);

        fd[1] = shm_open("readyarr", O_RDWR, 0777);
        ready = (int *)mmap(NULL, par_count* sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd[1], 0);
        sleep(2); // needed for initalizing synch
    }
    printf("Process ID: %d entered\n", par_id);


    synch(par_id, par_count, ready);

    if (par_id == 0)
    {
        init_elem(arr);
        copyarr(arr, copy);
        printf("\n\nUnsorted Array: \n");
        print_array(arr);
    }
    synch(par_id, par_count, ready);

    sort_parallel(arr, par_id, par_count, ready);

    synch(par_id, par_count, ready);

    if (par_id == 0)
    {

        printf("Sorted Array: \n");
        print_array(arr);

        oddEvenSort(copy, ARRAY_LENGTH);
        //lets test the result:
        if (compare_array(arr, copy))
            printf("full points!\n");
        else 

            printf("buuug!\n");
    }

    close(fd[0]);
    close(fd[1]);
    shm_unlink("array");
    shm_unlink("readyarr");

    return 0;
}