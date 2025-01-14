#include <stdio.h>
#include <unistd.h>

int main()
{
    int pipeFDs[2];
    int returnStatus;
    int pID;
    
    char writeMessage[20] = {"Creating Pipes"};
    char readMessage[20];
    
    returnStatus = pipe(pipeFDs);
    if (returnStatus == -1)
    {
        printf("An error occured, could not  create a pipe...");
        return 1;
    }
    
    pID = fork(); // create a child process
    
    if (pID == 0)
    {
        read(pipeFDs[0], readMessage, sizeof(readMessage));
        printf("Child Process -> Reading from pipe %s\n", readMessage);
    }
    
    else
    {
        printf("Parent Process -> Writing into pipe %s\n", writeMessage);
        write(pipeFDs[1], writeMessage, sizeof(writeMessage));
    }
    
    return 0;
}