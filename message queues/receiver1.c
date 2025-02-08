#include "header.h"

#define MAX_TEXT 512

struct message {
    long int messageType;
    char messageContent[MAX_TEXT];
};

// Function to compute the VRC (even parity) bit and append it to the dataword
char compute_vrc(char *dataword) {
    int count = 0;
    
    // Count the number of 1's in the dataword
    for (int i = 0; i < strlen(dataword); i++) {
        if (dataword[i] == '1') {
            count++;
        }
    }
    
    // If count is odd, append '1', else append '0' (even parity)
    if (count % 2 == 0) {
        return '0'; // even parity, append '0'
    } else {
        return '1'; // odd parity, append '1' to make it even parity
    }
}

int main() {
    int running = 1;
    int msgid;
    struct message msg;
    
    // value 0 indicates messages will be received in the same order as they are put into the queue
    long int messageToReceive = 0;
    
    key_t key = 12345; // same key in sender and receiver
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    while (running) {
        msgrcv(msgid, &msg, MAX_TEXT, messageToReceive, 0);
        
        // check if the message is "end" before printing
        if (strncmp(msg.messageContent, "end", 3) == 0) {
            running = 0; // stop the receiver loop
        } else {
            // Compute the VRC and append it to the received dataword
            char vrc = compute_vrc(msg.messageContent);
            strcat(msg.messageContent, &vrc); // Append the VRC bit to the binary string
            
            // Send the codeword (dataword + VRC bit) back to the sender
            if (msgsnd(msgid, &msg, MAX_TEXT, 0) == -1) {
                printf("Message not sent\n");
                return 3;
            }

            // Print the codeword
            printf("Codeword (with VRC) sent to sender: %s\n", msg.messageContent);
        }
    }
    
    // Destroy the message queue finally
    msgctl(msgid, IPC_RMID, 0);
    
    return 0;
}

