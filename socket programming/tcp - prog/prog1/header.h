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

#define BUFFER_SIZE 1024
#define SERVER_PORT 5000
#define PRINT_FORMAT(string1, string2) printf("%-15s\t-|-\t%-15s\n", string1, string2)

#endif
