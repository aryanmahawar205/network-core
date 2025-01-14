#include "D:\network-core\header-pipes.h"

// parent sends 5...child gets 5...calculates factorial and returns back to parent. (create two pipes)

int main()
{
    // set the number 5
    int number = 5;
    
    // define the array of file descriptors for the pipe
    // fd[0] - read end
    // fd[1] - write end
    int fdOne[2];
    int fdSecond[2];
    
    // create/open the pipe fdOne
    if (pipe(fdOne) == -1)
    {
        printf("An error occured while opening the pipe fdOne\n");
        return 1;
    }
    
    // create/open the pipe fdSecond
    if (pipe(fdSecond) == -1)
    {
        printf("An error orrcured while opening the pipe fdSecond\n");
        return 2;
    }
    
    // create another process (child process)
    int id = fork();
    
    if (id == -1)
    {
        printf("Could not fork a child process\n");
        return 3;
    }
    
    // in child process (child reads from parent via fdOne and writes the factorial to fdSecond)
    if (id == 0)
    {
        close(fdOne[1]);
        close(fdSecond[0]);
        int recvNumber;
        
        if (read(fdOne[0], &recvNumber, sizeof(recvNumber)) == -1)
        {
            printf("An error occured while reading from the pipe fdOne\n");
            return 4;
        }
        close(fdOne[0]);
        
        // compute factorial of recvNumber
        int factorial = 1;
    
        for (int i = 1; i <= recvNumber; i++)
            factorial *= i;
        
        // write back the factorial to the fdSecond pipe to be read by the parent
        if (write(fdSecond[1], &factorial, sizeof(factorial)) == -1)
        {
            printf("An error occured while writing into the pipe fdSecond\n");
            return 5;
        }
        close(fdSecond[1]);
    }
    
    // in parent process (parent sends the number to the child via fdOne and receives the computer factorial via fdSecond which is then displayed as final output)
    else
    {
        close(fdOne[0]);
        close(fdSecond[1]);
        
        int result;
        
        if (write(fdOne[1], &number, sizeof(number)) == -1)
        {
            printf("An error occured while writing into pipe fdOne\n");
            return 6;
        }
        close(fdOne[1]);
        
        // read computer factorial from child via fdSecond and display as final output
        if (read(fdSecond[0], &result, sizeof(result)) == -1)
        {
            printf("An error occured while reading from the pipe fdSecond\n");
            return 7;
        }
        close(fdSecond[0]);
        wait(NULL); // wait for child process to finish its execution
        
        printf("The computed factorial returned back to parent via pipe fdSecond is - %d", result);
    }
    
    return 0;
}