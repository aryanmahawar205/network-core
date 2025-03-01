#include "header.h"

#define PORT 56750

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

    char ipAddress[16];
    printf("Enter IP address in dotted decimal notation: ");
    scanf("%s", ipAddress);

    write(sockfd, ipAddress, strlen(ipAddress));  // Send IP address to server

    n = read(sockfd, recvBuff, sizeof(recvBuff) - 1);  // Receive converted representations
    recvBuff[n] = 0;
    printf("Converted representations from server:\n%s\n", recvBuff);

    close(sockfd);
    return 0;
}
