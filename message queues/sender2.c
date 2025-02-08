#include "header.h"

#define MAX_TEXT 512   // maximum length of the message that can be sent

// structure of message queue should start with a long int
struct message {
    long int messageType;
    char messageContent[MAX_TEXT]; // to store the data that will be written into the message queue
};

int main() {
    int running = 1;
    int msgid;
    struct message msg;
    
    char buffer[50]; // buffer array to store user input
    
    // create a message queue with key = 12345 with permissions 0666
    key_t key = 12345; // same key in sender and receiver
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    if (msgid == -1) {
        printf("Error in creating the message queue\n");
        return 1;
    }

    // run a while loop to start sending messages to the queue
    while (running) {
        // temporarily store the user input message into buffer
        printf("Enter a binary dataword to be sent (end to stop) - ");
        fgets(buffer, 50, stdin);
        printf("\n");

        // remove the newline character from input
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // check for "end" input to stop the process
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }

        // this value is assigned here because it will be used in the receiver process
        msg.messageType = 1;
        
        strcpy(msg.messageContent, buffer);
        
        // Send the dataword (without VRC) via the message queue
        if (msgsnd(msgid, &msg, MAX_TEXT, 0) == -1) {
            printf("Message not sent\n");
            return 2;
        }

        printf("Dataword sent: %s\n", msg.messageContent);
        
        // Wait to receive the codeword (with VRC) from the receiver
        if (msgrcv(msgid, &msg, MAX_TEXT, 0, 0) == -1) {
            printf("Message not received\n");
            return 3;
        }

        printf("Received codeword from receiver: %s\n", msg.messageContent);
    }

    return 0;
}

