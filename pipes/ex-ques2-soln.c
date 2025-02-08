#include "header.h"

// to check for palindrome
int is_palindrome(char *str)
{
    int len = strlen(str);
    
    for (int i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
            return 0; // not a palindrome
    }
    return 1; // is palindrome
}

int main()
{
    // FIFO file paths
    const char *fifo_send = "/tmp/fifo_send";
    const char *fifo_receive = "/tmp/fifo_receive";
    
    // create named pipes (FIFOs)
    if (mkfifo(fifo_send, 0666) == -1)
    {
        perror("mkfifo fifo_send failed");
        exit(1);
    }
    
    if (mkfifo(fifo_receive, 0666) == -1)
    {
        perror("mkfifo fifo_receive failed");
        exit(1);
    }

    // fork to create a receiver process
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    // in sender process
    if (pid > 0)
    {
        char message[100];
        int fifo_send_fd, fifo_receive_fd;

        // open the send FIFO for writing
        fifo_send_fd = open(fifo_send, O_WRONLY);
        if (fifo_send_fd == -1)
        {
            perror("Sender: Failed to open fifo_send");
            exit(1);
        }

        // open the receive FIFO for reading the response
        fifo_receive_fd = open(fifo_receive, O_RDONLY);
        if (fifo_receive_fd == -1)
        {
            perror("Sender: Failed to open fifo_receive");
            exit(1);
        }

        // get the number as a word from the user
        printf("Sender: Enter a number as a word: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // remove newline

        // send the word to the receiver through the FIFO
        write(fifo_send_fd, message, strlen(message) + 1);
        close(fifo_send_fd);

        // receive the response from the receiver (Palindrome/Not Palindrome)
        char response[50];
        read(fifo_receive_fd, response, sizeof(response));
        printf("Sender: Received response: %s\n", response);

        close(fifo_receive_fd);

        // clean up FIFOs and essentially deleting the FIFO files
        unlink(fifo_send);
        unlink(fifo_receive);
    }
    
    // in receiver process
    else
    {
        char message[100];
        int fifo_send_fd, fifo_receive_fd;

        // open the receive FIFO for reading
        fifo_receive_fd = open(fifo_send, O_RDONLY);
        if (fifo_receive_fd == -1)
        {
            perror("Receiver: Failed to open fifo_send");
            exit(1);
        }

        // open the send FIFO for writing the response
        fifo_send_fd = open(fifo_receive, O_WRONLY);
        if (fifo_send_fd == -1)
        {
            perror("Receiver: Failed to open fifo_receive");
            exit(1);
        }

        // receive the word from the sender
        read(fifo_receive_fd, message, sizeof(message));
        close(fifo_receive_fd);

        // check if the received word is a palindrome
        if (is_palindrome(message))
            write(fifo_send_fd, "Palindrome", strlen("Palindrome") + 1); // send "Palindrome" response back to sender
            
        else
            write(fifo_send_fd, "Not Palindrome", strlen("Not Palindrome") + 1); // send "Not Palindrome" responser back to sender
            
        close(fifo_send_fd);
    }

    return 0;
}
