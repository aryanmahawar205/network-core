#include "header.h"

void add_odd_parity(char* binary_data) {
    int length = strlen(binary_data);
    int count_ones = 0;

    for (int i = 0; i < length; i++) {
        if (binary_data[i] == '1') {
            count_ones++;
        }
    }

    if (count_ones % 2 == 0) {
        binary_data[length] = '1';
    } else {
        binary_data[length] = '0';
    }

    binary_data[length + 1] = '\0';
}

void number_to_binary(int number, char* binary_string) {
    int index = 0;
    while (number > 0) {
        binary_string[index++] = (number % 2) + '0';
        number /= 2;
    }
    binary_string[index] = '\0';

    int left = 0, right = index - 1;
    while (left < right) {
        char temp = binary_string[left];
        binary_string[left] = binary_string[right];
        binary_string[right] = temp;
        left++;
        right--;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    int input_number;
    printf("Enter a number: ");
    scanf("%d", &input_number);

    number_to_binary(input_number, buffer);
    add_odd_parity(buffer);

    if (write(client_socket, buffer, strlen(buffer)) > 0) {
        PRINT_FORMAT("[Sent]", buffer);
    } else {
        perror("Write failed");
    }

    memset(buffer, 0, BUFFER_SIZE);
    if (read(client_socket, buffer, BUFFER_SIZE) > 0) {
        PRINT_FORMAT("[Received]", buffer);
    } else {
        perror("Read failed");
    }

    close(client_socket);
    return 0;
}
