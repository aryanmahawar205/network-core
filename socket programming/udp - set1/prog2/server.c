#include "header.h"

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to reverse a string
void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to the address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        ssize_t received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                                   (struct sockaddr *)&client_addr, &addr_len);
        if (received < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[received] = '\0';  // Null-terminate the message
        printf("Received from client: %s\n", buffer);

        // Reverse the message
        reverse_string(buffer);

        // Send reversed message back to client
        sendto(sockfd, buffer, strlen(buffer), 0, 
              (struct sockaddr *)&client_addr, addr_len);
        printf("Sent to client: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
