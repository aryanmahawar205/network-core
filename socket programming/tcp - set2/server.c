#include "header.h"

#define PORT 56750

// Decrypts the message using Caesar Cipher
void decryptCaesar(char *message, int shift) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = (message[i] - 'A' - shift + 26) % 26 + 'A';
        } else if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = (message[i] - 'a' - shift + 26) % 26 + 'a';
        }
    }
}

// Reverses the string
void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];
    char recvBuff[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);  // Create socket

    memset(&server_addr, '0', sizeof(server_addr));
    memset(recvBuff, '0', sizeof(recvBuff));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));  // Bind socket

    listen(listenfd, 10);  // Listen for connections

    while (1) {
        connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addr_len);  // Accept connection

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connected from IP: %s, Port: %d\n", client_ip, ntohs(client_addr.sin_port));

        int shift;
        read(connfd, &shift, sizeof(shift));  // Receive shift value

        int n = read(connfd, recvBuff, sizeof(recvBuff) - 1);  // Receive encrypted message
        recvBuff[n] = 0;
        printf("Received encrypted message: %s\n", recvBuff);

        decryptCaesar(recvBuff, shift);  // Decrypt the message
        printf("Decrypted message: %s\n", recvBuff);

        reverseString(recvBuff);  // Reverse the decrypted message
        printf("Reversed decrypted message: %s\n", recvBuff);

        write(connfd, recvBuff, strlen(recvBuff));  // Send reversed message back

        close(connfd);  // Close connection
    }
}
