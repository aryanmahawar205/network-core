#include "header.h"

char encodedData[DATA_LENGTH + 1][9];

void flipBits(char* checksum) {
    for (int i = 0; i < 8; i++) {
        if (checksum[i] == '1') checksum[i] = '0';
        else checksum[i] = '1';
    }
}

void intToBinary(int num, char* binaryStr) {
    for (int i = 7; i >= 0; i--) {
        binaryStr[i] = (num & 1) + '0';
        num >>= 1;
    }
    binaryStr[8] = '\0';
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < DATA_LENGTH + 1; i++) {
        memset(encodedData[i], '0', sizeof(encodedData[i]));
    }

    int clientSocket = 0;
    struct sockaddr_in serverAddr;

    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&serverAddr, '0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    int totalSum = 0;

    for (int i = 0; i < DATA_LENGTH; i++) {
        int inputNum = 0;
        printf("Enter dataword %d: ", i + 1);
        scanf("%d", &inputNum);
        intToBinary(inputNum, encodedData[i]);
        totalSum += inputNum;
    }

    intToBinary(totalSum, encodedData[DATA_LENGTH]);
    flipBits(encodedData[DATA_LENGTH]);

    for (int i = 0; i < DATA_LENGTH + 1; i++) {
        if (write(clientSocket, encodedData[i], strlen(encodedData[i])) > 0) {
            PRINT_FORMAT("[Sent]", encodedData[i]);
        } else {
            perror("Write failed");
        }
    }

    char ackBuffer[BUFFER_SIZE] = {0};

    if (read(clientSocket, ackBuffer, BUFFER_SIZE) > 0) {
        PRINT_FORMAT("[Received]", ackBuffer);
    } else {
        perror("Read failed");
    }

    close(clientSocket);
    return 0;
}
