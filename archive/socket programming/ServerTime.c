#include "kvp.h"

#define PORT 56750

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in server_addr,client_addr; 
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];
    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);   // Socket Creation

    memset(&server_addr, '0', sizeof(server_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT); 

    bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)); // Address Binding

    listen(listenfd, 10);  // Listening

    while(1)
    {
        // Accept a client connection
	connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addr_len);
   
        ticks = time(NULL);
	time(&ticks);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 
	
	// Get client IP and port
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);
    	printf("Client connected from IP: %s, Port: %d\n", client_ip, ntohs(client_addr.sin_port));

        close(connfd);   // Close Connection
        sleep(1);
     }
}


