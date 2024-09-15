// SJF CODE

#include <stdio.h>

#define MAX_PROCESS 100

void sjf_non_preemptive(int arrival[], int burst[], int n) {
    int waiting_time[MAX_PROCESS], turnaround_time[MAX_PROCESS];
    int completion_time[MAX_PROCESS];
    int completed[MAX_PROCESS] = {0}; // Array to keep track of completed processes
    int i, j, time, min_index;
    
    // Initialize waiting and turnaround times to zero
    for (i = 0; i < n; i++) {
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
        completion_time[i] = 0;
    }

    // Initialize current time
    time = 0;
    for (i = 0; i < n; i++) {
        min_index = -1;
        for (j = 0; j < n; j++) {
            // Find the process with the smallest burst time that has arrived and not completed
            if (arrival[j] <= time && !completed[j] && (min_index == -1 || burst[j] < burst[min_index])) {
                min_index = j;
            }
        }

        if (min_index != -1) {
            // Update current time and completion time of the selected process
            time += burst[min_index];
            completion_time[min_index] = time;

            // Calculate waiting time and turnaround time
            waiting_time[min_index] = completion_time[min_index] - arrival[min_index] - burst[min_index];
            turnaround_time[min_index] = completion_time[min_index] - arrival[min_index];

            // Mark this process as completed
            completed[min_index] = 1;
        } else {
            // If no process is available, move time forward to the next arrival time
            int next_arrival = time + 1;
            for (j = 0; j < n; j++) {
                if (!completed[j] && arrival[j] < next_arrival) {
                    next_arrival = arrival[j];
                }
            }
            time = next_arrival;
            i--; // Re-check for the same index in the next iteration
        }
    }

    // Calculate average waiting time and turnaround time
    float total_waiting_time = 0, total_turnaround_time = 0;
    printf("\nProcess\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (i = 0; i < n; i++) {
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n", i, arrival[i], burst[i], completion_time[i], waiting_time[i], turnaround_time[i]);
    }
    printf("\nAverage Waiting Time: %.2f", total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f", total_turnaround_time / n);
}

int main() {
    int arrival[MAX_PROCESS], burst[MAX_PROCESS];
    int n, i;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter arrival time for process %d: ", i);
        scanf("%d", &arrival[i]);
        printf("Enter burst time for process %d: ", i);
        scanf("%d", &burst[i]);
    }

    sjf_non_preemptive(arrival, burst, n);

    return 0;
}