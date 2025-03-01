#include "header.h"

#define PORT 56750

// Function to convert an integer to binary string
void intToBinary(int num, char *binary) {
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = ((num >> i) & 1) ? '1' : '0';
    }
    binary[8] = '\0';
}

// Function to convert IP address to binary, octal, and hexadecimal
void convertIP(char *ip, char *binary, char *octal, char *hex) {
    unsigned int bytes[4];
    sscanf(ip, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]);

    char temp[9];
    binary[0] = '\0';
    octal[0] = '\0';
    hex[0] = '\0';

    for (int i = 0; i < 4; i++) {
        // Binary
        intToBinary(bytes[i], temp);
        strcat(binary, temp);
        strcat(binary, " ");

        // Octal
        char oct[4];
        sprintf(oct, "%o", bytes[i]);
        strcat(octal, oct);

        // Hexadecimal
        char hx[3];
        sprintf(hx, "%02X", bytes[i]);
        strcat(hex, hx);
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

        int n = read(connfd, recvBuff, sizeof(recvBuff) - 1);  // Receive IP address
        recvBuff[n] = 0;
        printf("Received IP address: %s\n", recvBuff);

        char binary[64], octal[32], hex[32];
        convertIP(recvBuff, binary, octal, hex);  // Convert IP address

        char result[256];
        sprintf(result, "Binary: %s\nOctal: %s\nHexadecimal: %s\n", binary, octal, hex);

        write(connfd, result, strlen(result));  // Send converted representations back

        close(connfd);  // Close connection
    }
}
