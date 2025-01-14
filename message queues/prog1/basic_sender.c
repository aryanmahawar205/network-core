// sender (sends messages to the message queue)

#include "D:\network-core\header-messageQueues.h"

#define MAX_TEXT 512   // maximum length of the message that can be sent

// structure of message queue should start with a long int
struct message
{
    long int messageType;
    char messageContent[MAX_TEXT]; // to store the data that will be written into the message queue
};

int main()
{
    int running = 1;
    int msgid;
    struct message msg;
    
    char buffer[50]; // buffer array to store user input
    
    // create a message queue with key = 12345 with permissions 0666
    // IPC_CREAT means that we are going to create the message queue
    key_t key = 12345; // same key in sender and receiver
    msgid = msgget(key , 0666|IPC_CREAT);
    
    if (msgid == -1)
    {
        printf("Error in creating the message queue\n");
        return 1;
    }

    // run a while loop to start sending messages to the queue
    while (running)
    {
        // temporarily store the user input message into buffer
        printf("Enter a message to be sent (end to stop) - ");
        fgets(buffer, 50, stdin);
        
        // this value is assigned here cause it will be used in the receiver process
        msg.messageType = 1;
        
        strcpy(msg.messageContent, buffer);
        
        // the msgsend function takes in 4 parameters
        // firstly the message ID, secondly the structure variable (or bascially the blueprint of the message)
        // thirdly the maximum size of the message and fouthly the last parameter controls if the queue is full or the system limit is reached
        // therefore the fourth parameter 0 means suspend the sending process and wait for space to be available in the queue
        // 1 woule mean to not wait and return / terminate immediately
        if (msgsnd(msgid, &msg, MAX_TEXT, 0) == -1) // msgsnd returns -1 if the message is not sent
        {   
            printf("Message not sent\n");
            return 2;
        }
        
        // check for the first three character to be equal to = "end" for termination
        if (strncmp(buffer, "end", 3) == 0)
            running = 0;
    }
    
    return 0;
 }