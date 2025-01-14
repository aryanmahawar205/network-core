#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

/*
FROM LINUX MAN PAGE OF OPEN FUNCTION:
OPENING THE READ OR WRITE END OF A FIFO BLOCKS UNTIL THE OTHER END IS ALSO OPENED (BY ANOTHER PROCESS OR THREAD)
basically - if you open a FIFO for writing, the open call hangs or blocks until another process comes along and opens the same FIFO for reading and vice versa
*/

int main()
{
    // first parameter of mkfifo is the actual FIFO file to be created and the next parameter are its file permissions in octal
    if (mkfifo("myfifo1", 0777) == -1)
    {
        // meaning something else has happened and we actually could not create the FIFO file
        // we do this because when we rerun the code after creating the FIFO file, it leads to the error
        // so this is just a check condition for that to see if it alredy exists or not
        if (errno != EEXIST)
        {
            printf("Could not create FIFO file\n");
            return 1;
        }
    }
    
    // second parameter is why you want to open this file? (O_WRONLY - open write only)
    // in FIFOs, the open function return the file descriptors
    int fd = open("myfifo1", O_WRONLY);
    
    if (fd == -1)
    {
        printf("Could not open FIFO\n");
        return 2;
    }
    
    int x = 97;
    if (write(fd, &x, sizeof(int)) == -1)
    {
        printf("Could not write into FIFO file\n");
        return 3;
    }
    close(fd);
    
    return 0;
}