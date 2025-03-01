#include "header.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Get message from user
    printf("Enter message to send: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Send message to server
    sendto(sockfd, buffer, strlen(buffer), 0, 
          (struct sockaddr *)&server_addr, addr_len);
    printf("Message sent to server.\n");

    // Receive response from server
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                               (struct sockaddr *)&server_addr, &addr_len);
    if (received < 0) {
        perror("Receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[received] = '\0';  // Null-terminate the response
    printf("Server response: %s\n", buffer);

    close(sockfd);
    return 0;
}
