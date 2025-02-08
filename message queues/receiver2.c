#include "D:\network-core\message queues\header2.h"

#define MAX_TEXT 512

// structure of message queue should start with a long int
struct message {
    long int messageType;
    char messageContent[MAX_TEXT];
};

// Function to convert decimal to binary
void decimal_to_binary(int n, char *bin) {
    int i = 0;
    while (n > 0) {
        bin[i] = (n % 2) + '0'; // get the binary digit
        n = n / 2;
        i++;
    }
    bin[i] = '\0';
    
    // Reverse the binary string
    int len = strlen(bin);
    for (int j = 0; j < len / 2; j++) {
        char temp = bin[j];
        bin[j] = bin[len - j - 1];
        bin[len - j - 1] = temp;
    }
}

// Function to convert decimal to octal
void decimal_to_octal(int n, char *oct) {
    int i = 0;
    while (n > 0) {
        oct[i] = (n % 8) + '0'; // get the octal digit
        n = n / 8;
        i++;
    }
    oct[i] = '\0';
    
    // Reverse the octal string
    int len = strlen(oct);
    for (int j = 0; j < len / 2; j++) {
        char temp = oct[j];
        oct[j] = oct[len - j - 1];
        oct[len - j - 1] = temp;
    }
}

// Function to convert decimal to hexadecimal
void decimal_to_hexadecimal(int n, char *hex) {
    int i = 0;
    while (n > 0) {
        int rem = n % 16;
        if (rem < 10) {
            hex[i] = rem + '0'; // get the hex digit
        } else {
            hex[i] = rem - 10 + 'A'; // get the hex letter
        }
        n = n / 16;
        i++;
    }
    hex[i] = '\0';
    
    // Reverse the hexadecimal string
    int len = strlen(hex);
    for (int j = 0; j < len / 2; j++) {
        char temp = hex[j];
        hex[j] = hex[len - j - 1];
        hex[len - j - 1] = temp;
    }
}

int main() {
    int msgid;
    struct message msg;
    
    long int messageToReceive = 0;
    
    key_t key = 12345; // same key in sender and receiver
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    while (1) {
        msgrcv(msgid, &msg, MAX_TEXT, messageToReceive, 0);
        
        // If message is "end", stop the receiver loop
        if (strncmp(msg.messageContent, "end", 3) == 0) {
            break;
        } else {
            int decimal = atoi(msg.messageContent); // Convert the string to an integer
            
            // Arrays to store binary, octal, and hexadecimal strings
            char binary[50], octal[50], hexadecimal[50];
            
            // Convert decimal number to binary, octal, and hexadecimal
            decimal_to_binary(decimal, binary);
            decimal_to_octal(decimal, octal);
            decimal_to_hexadecimal(decimal, hexadecimal);
            
            // Format the result and send it back to the sender
            snprintf(msg.messageContent, MAX_TEXT, "%s %s %s", binary, octal, hexadecimal);
            
            // Send the result back to the sender
            if (msgsnd(msgid, &msg, MAX_TEXT, 0) == -1) {
                printf("Message not sent\n");
                return 3;
            }

            // Print the result
            printf("Binary: %s, Octal: %s, Hexadecimal: %s\n", binary, octal, hexadecimal);
        }
    }
    
    // Destroy the message queue
    msgctl(msgid, IPC_RMID, 0);
    
    return 0;
}

