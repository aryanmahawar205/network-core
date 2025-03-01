#include "header.h"

#define PORT 56750

// Encrypts the message using Caesar Cipher
void encryptCaesar(char *message, int shift) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = (message[i] - 'A' + shift) % 26 + 'A';
        } else if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = (message[i] - 'a' + shift) % 26 + 'a';
        }
    }
}

int main(int argc, char *argv[]) {
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    memset(recvBuff, '0', sizeof(recvBuff));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // Create socket

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\n Invalid address or address not supported \n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Connection failed \n");
        return 1;
    }

    char message[1024];
    int shift;

    printf("Enter message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove newline

    printf("Enter shift value: ");
    scanf("%d", &shift);

    encryptCaesar(message, shift);  // Encrypt the message
    printf("Encrypted message: %s\n", message);

    // Send shift value first
    write(sockfd, &shift, sizeof(shift));

    // Send encrypted message
    write(sockfd, message, strlen(message));

    // Receive reversed decrypted message
    n = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
    recvBuff[n] = 0;
    printf("Reversed decrypted message from server: %s\n", recvBuff);

    close(sockfd);
    return 0;
}
