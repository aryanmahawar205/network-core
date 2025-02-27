#include "header.h"

void check_odd_parity(char* codeword) {
    int length = strlen(codeword);
    int count_ones = 0;

    for (int i = 0; i < length - 1; i++) {
        if (codeword[i] == '1') {
            count_ones++;
        }
    }

    if ((count_ones % 2) == (codeword[length - 1] - '0')) {
        strcpy(codeword, "-VE ACK");
    } else {
        strcpy(codeword, "+VE ACK");
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server started listening on port %d...\n\n", SERVER_PORT);

    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr*)NULL, NULL)) < 0) {
            perror("Accept failed");
            continue;
        }

        memset(buffer, 0, BUFFER_SIZE);
        if (read(client_socket, buffer, BUFFER_SIZE) > 0) {
            PRINT_FORMAT("[Received]", buffer);
            check_odd_parity(buffer);

            if (write(client_socket, buffer, strlen(buffer)) > 0) {
                PRINT_FORMAT("[Sent]", buffer);
            } else {
                perror("Write failed");
            }
        } else {
            perror("Read failed");
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
