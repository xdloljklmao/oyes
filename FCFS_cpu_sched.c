#include <stdio.h>

struct Process {
    int pid, arrival_time, burst_time, waiting_time, turnaround_time, completion_time;
};

void sortProcesses(struct Process p[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival_time > p[j].arrival_time ||
                (p[i].arrival_time == p[j].arrival_time && p[i].burst_time > p[j].burst_time)) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void findTimes(struct Process p[], int n) {
    int current_time = 0;
    
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time; 
        }
        p[i].waiting_time = current_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        p[i].completion_time = current_time + p[i].burst_time;
        current_time = p[i].completion_time;
    }
}

void printGanttChart(struct Process p[], int n) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) printf("------");
    printf("\n|");

    for (int i = 0; i < n; i++) printf(" P%d |", p[i].pid);
    printf("\n ");

    for (int i = 0; i < n; i++) printf("------");
    printf("\n");

    for (int i = 0; i < n; i++) printf("%d\t", p[i].completion_time - p[i].burst_time);
    printf("%d\n", p[n-1].completion_time);
}

void findAvgtime(struct Process p[], int n) {
    sortProcesses(p, n); 
    findTimes(p, n);

    printf("\nProcess\tAT\tBT\tWT\tTAT\tCT\n");
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time, p[i].completion_time);
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);

    printGanttChart(p, n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter Process ID, Arrival Time and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &p[i].pid, &p[i].arrival_time, &p[i].burst_time);
    }

    findAvgtime(p, n);
    return 0;
}
