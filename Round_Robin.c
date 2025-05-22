#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};

void roundRobin(struct Process p[], int n, int quantum) {
    int t = 0, completed = 0;
    int gantt_chart[100], gantt_time[100], gc_index = 0;
    
    while (completed < n) {
        int all_done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                all_done = 0;
                gantt_chart[gc_index] = p[i].id;
                gantt_time[gc_index++] = t;
                
                if (p[i].remaining_time > quantum) {
                    t += quantum;
                    p[i].remaining_time -= quantum;
                } else {
                    t += p[i].remaining_time;
                    p[i].waiting_time = t - p[i].arrival_time - p[i].burst_time;
                    p[i].turnaround_time = t - p[i].arrival_time;
                    p[i].remaining_time = 0;
                    completed++;
                }
            }
        }
        if (all_done) break;
    }
    gantt_time[gc_index] = t;

    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].arrival_time, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
    }

    printf("\nGantt Chart:\n ");
    for (int i = 0; i < gc_index; i++) {
        printf("------");
    }
    printf("\n|");
    for (int i = 0; i < gc_index; i++) {
        printf(" P%d |", gantt_chart[i]);
    }
    printf("\n ");
    for (int i = 0; i < gc_index; i++) {
        printf("------");
    }
    printf("\n");
    for (int i = 0; i <= gc_index; i++) {
        printf("%d\t", gantt_time[i]);
    }
    printf("\n");
}

int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("\nProcess P%d\n", p[i].id);
        printf("Enter arrival time: ");
        scanf("%d", &p[i].arrival_time);
        printf("Enter burst time: ");
        scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
    }
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    roundRobin(p, n, quantum);
    return 0;
}
