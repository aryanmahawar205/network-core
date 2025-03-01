#include "header.h"

#define PORT 56750

// Function to validate the IP address based on Classful Network Architecture
int validateIP(char *ip) {
    unsigned int bytes[4];
    if (sscanf(ip, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) != 4) {
        return 0;  // Invalid format
    }

    for (int i = 0; i < 4; i++) {
        if (bytes[i] > 255) {
            return 0;  // Invalid byte value
        }
    }

    // Check IP class and range
    if (bytes[0] >= 0 && bytes[0] <= 127) {
        // Class A
        return 1;
    } else if (bytes[0] >= 128 && bytes[0] <= 191) {
        // Class B
        return 1;
    } else if (bytes[0] >= 192 && bytes[0] <= 223) {
        // Class C
        return 1;
    } else if (bytes[0] >= 224 && bytes[0] <= 239) {
        // Class D (Multicast)
        return 1;
    } else if (bytes[0] >= 240 && bytes[0] <= 255) {
        // Class E (Reserved)
        return 1;
    }

    return 0;  // Invalid range
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

        // Validate the IP address
        if (validateIP(recvBuff)) {
            write(connfd, "VALID", 5);  // Send "VALID" to client
        } else {
            write(connfd, "INVALID", 7);  // Send "INVALID" to client
        }

        close(connfd);  // Close connection
    }
}
