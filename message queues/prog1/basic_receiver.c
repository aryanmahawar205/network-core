// receiver (receives messages from the message queue)

#include "D:\network-core\header-messageQueues.h"

#define MAX_TEXT 512

struct message
{
    long int messageType;
    char messageContent[MAX_TEXT];
};

int main()
{
    int running = 1;
    int msgid;
    struct message msg;
    
    // value 0 indicates messages will be received in the same order as they are put into the queue
    long int messageToReceive = 0;
    
    key_t key = 12345; // same key in sender and receiver
    // since we run the sender code first, and thence the queue is already created
    // consequently, the IPC_CREAT system call will have no impact
    msgid = msgget(key, 0666|IPC_CREAT);
    
    while (running)
    {
        msgrcv(msgid, &msg, MAX_TEXT, messageToReceive, 0);
        
	    // check if the message is "end" before printing
        if (strncmp(msg.messageContent, "end", 3) == 0)
            running = 0; // stop the receiver loop

	    // print the message content only if it's not "end"
	    else
	        printf("Message received -  %s\n", msg.messageContent);
    }
    
    // destroy the message queue finally
    // IPC_RMID means IPC remove ID
    msgctl(msgid, IPC_RMID, 0);
}