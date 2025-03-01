#ifndef HEADER_H
#define HEADER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

#define DATA_LENGTH 5
#define BUFFER_SIZE 1024
#define SERVER_PORT 5000
#define PRINT_FORMAT(str1, str2) printf("%-15s\t-|-\t%-15s\n", str1, str2)

#endif
