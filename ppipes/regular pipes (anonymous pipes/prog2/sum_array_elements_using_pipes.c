#include "D:\network-core\header.h"

// you have an array of elements and you gotta sum up all the elements using pipes
// one of the process sums up one half of the array and the other process sums up the other half
// one of the half sums is sent (written) onto the pipe and the receiving end sums it up with the other half sum
// and the total final sum is finally displayed

int main()
{
    // define the array
    int arr[] = {1, 2, 3, 4, 1, 2};
    int arrSize = sizeof(arr)/sizeof(arr[0]);
    
    // indices for start and end of the array for each process to compute sum
    int start, end;
    
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
        start = 0;
        end = arrSize/2;
    }
    
    // in parent process
    else
    {
        start = arrSize/2;
        end = arrSize;
    }
    
    // calculate the sum or every processes
    int sum = 0;
    for (int i = start; i < end; i++)
        sum += arr[i];
        
    printf("Calculated partial sum - %d\n", sum);
    
    // if in child process, send the partial sum to the parent process
    if (id == 0)
    {
        close(fd[0]);
        if (write(fd[1], &sum, sizeof(sum)) == -1)
        {
            printf("An error occured while writing into the pipe\n");
            return 3;
        }
        close(fd[1]);
    }
    
    // else in parent process, get the partial sum from the child...sum it with your partial sum and return final sum
    else
    {
        close(fd[1]);
        int partialSumFromChild;
        if (read(fd[0], &partialSumFromChild, sizeof(partialSumFromChild)) == -1)
        {
            printf("An error occured while reading from the pipe\n");
            return 4;
        }
        close(fd[0]);
        
        int totalSum = sum + partialSumFromChild;
        
        printf("Total sum computed by parent is - %d", totalSum);
        wait(NULL); // wait for child process to finish execution
    }
    
    return 0;
}