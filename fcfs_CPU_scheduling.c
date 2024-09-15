#include <stdio.h>
#define MAX 30

int main()
{
    int i, n, bt[MAX], at[MAX], wt[MAX], tat[MAX], temp[MAX];
    float awt = 0, atat = 0;
    
    printf("Enter the number of processes - ");
    scanf("%d", &n);
    
    printf("Enter the arrival and burst times of the processes...\n");
    for (i = 0; i < n; i++)
        scanf("%d %d", &at[i], &bt[i]);
    
    // Initialize the time to 0
    temp[0] = 0;
    
    printf("Process\t Burst Time\t Arrival Time\t Waiting Time\t Turn Around Time\n");
    
    for (i = 0; i < n; i++)
    {
        if (temp[i] < at[i]) {
            // CPU is idle, so update time to arrival of the current process
            temp[i] = at[i];
        }
        
        wt[i] = temp[i] - at[i];  // Waiting time calculation
        tat[i] = wt[i] + bt[i];   // Turnaround time calculation
        
        // Update the end time of the current process
        temp[i+1] = temp[i] + bt[i];
        
        awt += wt[i];
        atat += tat[i];
        
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", i+1, bt[i], at[i], wt[i], tat[i]);
    }
    
    awt /= (float)n;
    atat /= (float)n;
    
    printf("Average waiting time - %.2f\n", awt);
    printf("Average turnaround time - %.2f\n", atat);
    
    return 0;
}