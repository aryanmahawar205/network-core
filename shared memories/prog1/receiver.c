// shared memory receiver program

#include "header-sharedMemories.h"

int main()
{
    void *shared_memory;
    char buff[100];
    int shmid;
    
    shmid = shmget((key_t)1122, 1024, 0666);
    printf("Key of the shared memory is - %d\n", shmid);
    
    shared_memory = shmat(shmid, NULL, 0); // process attached to shared memory segment
    printf("Process attached at memory address - %p\n", shared_memory);
    
    printf("Data read from shared memory is - %s\n", (char *)shared_memory);
    
    return 0;
}