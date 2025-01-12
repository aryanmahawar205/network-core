#include "D:\network-core\header.h"

// child writes to the pipe, an integer and parent reads and does some processing with it (prints it)

int main()
{
    // define the array of file descriptors for the pipe
    // fd[0] - read end
    // fd[1] - write end
    int fd[2];
    
    // create/open the pipe
    if (pipe(fd) == -1)
    {
        printf("An error occured with opening the pipe\n");
        return 1;
    }
    
    // create another process (child process)
    int id = fork();
    
    if (id == -1)
    {
        printf("Could not fork a child process\n");
        return 2;
    }
    
    // in child process
    if (id == 0)
    {
        close(fd[0]);
        int x;
        printf("Input a number - ");
        scanf("%d", &x);
        if (write(fd[1], &x, sizeof(int)) == -1)
        {
            printf("An error occured while writing to the pipe\n");
            return 3;
        }
        close(fd[1]);
    }
    
    // in parent process
    else
    {
        close(fd[1]);
        int y;
        if (read(fd[0], &y, sizeof(int)) == -1)
        {
            printf("An error occured while reading from the pipe\n");
            return 4;
        }
        close(fd[0]);
        printf("Got from child process - %d\n", y);
    }
    
    return 0;
}