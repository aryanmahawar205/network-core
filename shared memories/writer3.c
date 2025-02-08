#include "header.h"

#define MAX_TEXT 1024 // maximum size for shared memory

int main()
{
    void *shared_memory;
    char buff[MAX_TEXT];
    int shmid;
    
    // Create shared memory segment with key 1122
    shmid = shmget((key_t)1122, MAX_TEXT, 0666|IPC_CREAT);
    if (shmid == -1)
    {
        perror("Error in shmget");
        return 1;
    }

    printf("Key of shared memory segment is - %d\n", shmid);
    
    // Attach to the shared memory segment
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *) -1)
    {
        perror("Error in shmat");
        return 2;
    }

    printf("Process attached at memory address - %p\n", shared_memory);
    
    // Ask the user to enter a paragraph
    printf("Enter a paragraph to write to shared memory: \n");
    fgets(buff, MAX_TEXT, stdin); // Get input from the user
    
    // Copy the paragraph into shared memory
    strcpy(shared_memory, buff);
    
    printf("You have written to shared memory: \n%s\n", (char *)shared_memory);
    
    return 0;
}

