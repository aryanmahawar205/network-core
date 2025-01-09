#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

void encrypt(char* message, int shift) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = ((message[i] - 'a' + shift) % 26) + 'a';
        } else if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = ((message[i] - 'A' + shift) % 26) + 'A';
        }
    }
}

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Enter the message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove newline character

    encrypt(message, 4);
    send(client_fd, message, strlen(message), 0);
    printf("Encrypted message sent: %s\n", message);

    valread = read(client_fd, buffer, sizeof(buffer) - 1);
    printf("Server response: %s\n", buffer);

    close(client_fd);
    return 0;
}

