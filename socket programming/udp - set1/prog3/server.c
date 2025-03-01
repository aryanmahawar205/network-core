#include "header.h"
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to count vowels, consonants, white spaces, and digits
void analyze_message(const char *message, char *result) {
    int vowels = 0, consonants = 0, spaces = 0, digits = 0;

    for (int i = 0; message[i] != '\0'; i++) {
        char ch = tolower(message[i]);  // Convert to lowercase for easier checking

        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            vowels++;
        } else if (isalpha(ch)) {  // Check if it's a consonant
            consonants++;
        } else if (isdigit(ch)) {  // Check if it's a digit
            digits++;
        } else if (isspace(ch)) {  // Check if it's a white space
            spaces++;
        }
    }

    // Format the result string
    snprintf(result, BUFFER_SIZE, "Vowels: %d, Consonants: %d, Digits: %d, Spaces: %d",
             vowels, consonants, digits, spaces);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char result[BUFFER_SIZE];
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

        // Analyze the message
        analyze_message(buffer, result);

        // Send the result back to client
        sendto(sockfd, result, strlen(result), 0, 
              (struct sockaddr *)&client_addr, addr_len);
        printf("Sent to client: %s\n", result);
    }

    close(sockfd);
    return 0;
}
