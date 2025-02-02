// shared memory sender (client) program

#include "D:\network-core\header-sharedMemories.h"

int main() {
    void *shared_memory;
    char buff[100];
    int shmid;
    
    shmid = shmget((key_t)1122, 1024, 0666|IPC_CREAT);  // Create shared memory segment
    printf("Key of shared memory segment is - %d\n", shmid);
    
    shared_memory = shmat(shmid, NULL, 0);  // Attach to shared memory segment
    printf("Process attached at memory address - %p\n", shared_memory);
    
    int choice;
    int number;
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Convert to Binary\n");
        printf("2. Convert to Octal\n");
        printf("3. Convert to Hexadecimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }
        
        printf("Enter a number to convert: ");
        scanf("%d", &number);

        // Prepare the data for shared memory (choice:number)
        sprintf(buff, "%d:%d", choice, number);
        strcpy(shared_memory, buff);
        
        // Set flag to indicate data is available for server to process
        *((int *)shared_memory + 1) = 1;  // Set flag to 1 (data available)
        
        // Wait for the server to process the data and reset the flag
        while (*((int *)shared_memory + 1) != 0) {
            sleep(1);  // Sleep to avoid busy-waiting
        }
        
        // Print the result received from the server
        printf("Received from server: %s\n", (char *)shared_memory);
    }
    
    return 0;
}

