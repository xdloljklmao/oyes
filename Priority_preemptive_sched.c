#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining;
    int turnaround;
    int waiting;
    int priority;
};

int main() {
    int num_of_proc;
    printf("Enter number of processes: ");
    scanf("%d", &num_of_proc);

    struct Process p[num_of_proc];

    for (int i = 0; i < num_of_proc; i++) {
        p[i].id = i + 1;
        printf("\nProcess P%d\n", i + 1);
        printf("Enter arrival time: ");
        scanf("%d", &p[i].arrival_time);
        printf("Enter burst time: ");
        scanf("%d", &p[i].burst_time);
        printf("Enter priority: ");
        scanf("%d", &p[i].priority);
        p[i].remaining = p[i].burst_time;
        p[i].waiting = 0;
        p[i].turnaround = 0;
    }

    int current_time = 0, completed = 0;
    int completion_time[num_of_proc];

    int gantt_chart[100], gantt_time[100], gc_index = 0;

    while (completed < num_of_proc) {
        int selected = -1;

        for (int i = 0; i < num_of_proc; i++) {
            if (p[i].arrival_time > current_time || p[i].remaining == 0) 
                continue;

            if (selected == -1 || 
                p[i].priority < p[selected].priority || 
                (p[i].priority == p[selected].priority && p[i].remaining < p[selected].remaining) ||
                (p[i].priority == p[selected].priority && p[i].remaining == p[selected].remaining && p[i].arrival_time < p[selected].arrival_time)) {
                selected = i;
            }
        }

        if (selected == -1) {
            current_time++;
            continue;
        }

        if (gc_index == 0 || gantt_chart[gc_index - 1] != p[selected].id) {
            gantt_chart[gc_index] = p[selected].id;
            gantt_time[gc_index++] = current_time;
        }

        p[selected].remaining--;
        current_time++;

        if (p[selected].remaining == 0) {
            completion_time[selected] = current_time;
            p[selected].turnaround = completion_time[selected] - p[selected].arrival_time;
            p[selected].waiting = p[selected].turnaround - p[selected].burst_time;
            completed++;
        }
    }

    gantt_time[gc_index] = current_time;

    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for (int i = 0; i < num_of_proc; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
              p[i].id, p[i].arrival_time, p[i].burst_time, p[i].priority,
              p[i].waiting, p[i].turnaround);
    }

    printf("\nGantt Chart:\n ");

    for (int i = 0; i < gc_index; i++) printf("------");
    printf("\n|");

    for (int i = 0; i < gc_index; i++) printf(" P%d |", gantt_chart[i]);
    printf("\n ");

    for (int i = 0; i < gc_index; i++) printf("------");
    printf("\n");

    for (int i = 0; i <= gc_index; i++) printf("%d\t", gantt_time[i]);
    printf("\n");

    return 0;
}
