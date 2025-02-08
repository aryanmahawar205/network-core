#include "D:\network-core\pipes\header1.h"

// Function to reverse a string
void reverse_string(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int main()
{
    // Define two pipes for bidirectional communication
    int fd1[2]; // Pipe for child to parent communication
    int fd2[2]; // Pipe for parent to child communication
    
    // Create pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {
        printf("An error occurred while opening the pipes\n");
        return 1;
    }
    
    // Create another process (child process)
    int id = fork();
    
    if (id == -1)
    {
        printf("Could not fork a child process\n");
        return 2;
    }
    
    // In child process
    if (id == 0)
    {
        close(fd1[0]); // Close unused read end of first pipe
        close(fd2[1]); // Close unused write end of second pipe

        char message[100];
        
        while (1)
        {
            printf("Child: Input a message (type 'END' to stop): ");
            fgets(message, sizeof(message), stdin);
            
            // Remove newline character from the string
            message[strcspn(message, "\n")] = '\0';
            
            // Send message to parent
            if (write(fd1[1], message, strlen(message) + 1) == -1)
            {
                printf("An error occurred while writing to the pipe\n");
                return 3;
            }

            // If the message is "END", exit the loop and terminate the program
            if (strcmp(message, "END") == 0)
                break;
            
            // Receive reversed message from parent
            if (read(fd2[0], message, sizeof(message)) == -1)
            {
                printf("An error occurred while reading from the pipe\n");
                return 4;
            }

            printf("Child received reversed message: %s\n", message);
        }

        close(fd1[1]);
        close(fd2[0]);
    }
    
    // In parent process
    else
    {
        close(fd1[1]); // Close unused write end of first pipe
        close(fd2[0]); // Close unused read end of second pipe
        
        char message[100];
        
        while (1)
        {
            // Receive message from child
            if (read(fd1[0], message, sizeof(message)) == -1)
            {
                printf("An error occurred while reading from the pipe\n");
                return 4;
            }

            // If the message is "END", exit the loop and terminate the program
            if (strcmp(message, "END") == 0)
                break;

            // Reverse the message
            reverse_string(message);

            // Send the reversed message back to child
            if (write(fd2[1], message, strlen(message) + 1) == -1)
            {
                printf("An error occurred while writing to the pipe\n");
                return 3;
            }

            printf("Parent sent reversed message: %s\n", message);
        }

        close(fd1[0]);
        close(fd2[1]);
    }
    
    return 0;
}

