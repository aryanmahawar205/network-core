#include "header.h"
#include <time.h>  // For date and time functions

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to get the current date and time
void get_current_time(char *time_str) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(time_str, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", time_info);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char time_str[BUFFER_SIZE];
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

        // Get current date and time
        get_current_time(time_str);

        // Calculate required size for the response
        size_t response_size = strlen(buffer) + strlen(" [Server Time: ]") + strlen(time_str) + 1;

        // Allocate memory for the response
        char *response = (char *)malloc(response_size);
        if (!response) {
            perror("Memory allocation failed");
            continue;
        }

        // Prepare response with date and time
        snprintf(response, response_size, "%s [Server Time: %s]", buffer, time_str);

        // Send response back to client
        sendto(sockfd, response, strlen(response), 0, 
              (struct sockaddr *)&client_addr, addr_len);
        printf("Sent to client: %s\n", response);

        // Free allocated memory
        free(response);
    }

    close(sockfd);
    return 0;
}
