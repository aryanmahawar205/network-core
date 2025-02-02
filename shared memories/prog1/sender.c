// shared memory sender program

#include "D:\network-core\header-sharedMemories.h"

int main()
{
    void *shared_memory; // to store the address of the shared segment
    char buff[100];
    int shmid;
    
    shmid = shmget((key_t)1122, 1024, 0666|IPC_CREAT); // creates shared memory segment with key 1122
    printf("Key of shared memory segment is - %d\n", shmid);
    
    shared_memory = shmat(shmid, NULL, 0); // process attached to shared memory segment
    printf("Process attached at memory address - %p\n", shared_memory);
    
    printf("Enter some data to write to the shared memory - \n");
    read(0, buff, 100); // get some input from the user
    strcpy(shared_memory, buff); // data written to shared memory
    
    printf("You have written  - %s\n", (char *)shared_memory);
    
    return 0;
}