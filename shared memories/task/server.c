// shared memory receiver (server) program

#include "D:\network-core\header-sharedMemories.h"

void convert_and_send(int choice, int number, char *shared_memory) {
    char result[100];  // Buffer to hold the result

    // Perform the conversion based on the user's choice
    switch (choice) {
        case 1:  // Binary conversion
            {
                char binary[33];  // 32 bits + 1 for the null terminator
                binary[32] = '\0';  // Null-terminate the string
                for (int i = 31; i >= 0; i--) {
                    binary[i] = (number & 1) ? '1' : '0';
                    number >>= 1;
                }
                sprintf(result, "Binary: %s", binary);
            }
            break;
        case 2:  // Octal conversion
            sprintf(result, "Octal: %o", number);  // Use the %o format specifier
            break;
        case 3:  // Hexadecimal conversion
            sprintf(result, "Hexadecimal: %X", number);  // Use the %X format specifier
            break;
        default:
            sprintf(result, "Invalid choice.");
            break;
    }
    
    // Copy the result into shared memory and ensure proper null-termination
    printf("Server: Writing to shared memory: %s\n", result);  // Debug print to confirm data is being written
    strcpy(shared_memory, result);  // This will copy the result to shared memory
}

int main() {
    void *shared_memory;
    char buff[100];
    int shmid;
    
    shmid = shmget((key_t)1122, 1024, 0666);  // Attach to shared memory
    printf("Key of shared memory is - %d\n", shmid);
    
    shared_memory = shmat(shmid, NULL, 0);  // Attach to shared memory segment
    printf("Process attached at memory address - %p\n", shared_memory);
    
    while (1) {
        // Check if the client has sent data (flag == 1)
        if (*((int *)shared_memory + 1) == 1) {
            // Read the data from shared memory
            strcpy(buff, (char *)shared_memory);
            
            // Extract choice and number from the buff
            int choice, number;
            sscanf(buff, "%d:%d", &choice, &number);
            
            // Perform conversion based on the choice
            convert_and_send(choice, number, shared_memory);
            
            // Reset the flag to indicate that the server has processed the data
            *((int *)shared_memory + 1) = 0;  // Reset the flag (processed)
        }
        
        sleep(1);  // Sleep for a short period to avoid busy-waiting
    }
    
    return 0;
}

