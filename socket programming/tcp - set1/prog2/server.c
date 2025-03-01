#include "header.h"

char encodedData[DATA_LENGTH + 1][9];

int binaryToInt(char* binaryStr) {
    int num = 0;
    for (int i = 0; i < 8; i++) {
        num = (num << 1) + (binaryStr[i] - '0');
    }
    return num;
}

int verifyChecksum(char data[][9]) {
    int total = 0;
    for (int i = 0; i < DATA_LENGTH + 1; i++) {
        total += binaryToInt(data[i]);
    }
    return (total & 0xFF) == 0;
}

int main() {
    for (int i = 0; i < DATA_LENGTH + 1; i++) {
        memset(encodedData[i], '0', sizeof(encodedData[i]));
    }

    int serverSocket = 0, clientSocket = 0;
    struct sockaddr_in serverAddr;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&serverAddr, '0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(serverSocket, 10) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server started listening on port %d...\n\n", SERVER_PORT);

    while (1) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)NULL, NULL)) < 0) {
            perror("Accept failed");
            continue;
        }

        for (int i = 0; i < DATA_LENGTH + 1; i++) {
            if (read(clientSocket, encodedData[i], sizeof(encodedData[0]) - 1) > 0) {
                encodedData[i][sizeof(encodedData[0]) - 1] = '\0';
                PRINT_FORMAT("[Received]", encodedData[i]);
            } else {
                perror("Read failed");
            }
        }

        char ackBuffer[BUFFER_SIZE] = {0};

        if (verifyChecksum(encodedData)) {
            strcpy(ackBuffer, "+VE");
        } else {
            strcpy(ackBuffer, "-VE");
        }

        if (write(clientSocket, ackBuffer, strlen(ackBuffer)) > 0) {
            PRINT_FORMAT("[Sent]", ackBuffer);
        } else {
            perror("Write failed");
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
