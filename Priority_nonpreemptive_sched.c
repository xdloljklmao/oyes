#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting;
    int turnaround;
    int priority;
    int completed;
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
        p[i].waiting = 0;
        p[i].turnaround = 0;
        p[i].completed = 0;
    }

    int current_time = 0;
    int completed = 0;

    int gantt_chart[100];  
    int gantt_time[100];   
    int gc_index = 0;

    while (completed < num_of_proc) {
        int selected = -1;

        for (int i = 0; i < num_of_proc; i++) {
            if (!p[i].completed && p[i].arrival_time <= current_time) {
                if (selected == -1 || p[i].priority < p[selected].priority || 
                    (p[i].priority == p[selected].priority && p[i].arrival_time < p[selected].arrival_time)) {
                    selected = i;
                }
            }
        }

        if (selected == -1) {
            current_time++;
            continue;
        }

        gantt_chart[gc_index] = p[selected].id; 
        gantt_time[gc_index++] = current_time;  

        current_time += p[selected].burst_time;

        p[selected].turnaround = current_time - p[selected].arrival_time;
        p[selected].waiting = p[selected].turnaround - p[selected].burst_time;
        p[selected].completed = 1; 

        completed++;
    }

    gantt_time[gc_index] = current_time;

    printf("\nProcess\tAT\tBT\tPriority\tWT\tTAT\n");
    for (int i = 0; i < num_of_proc; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].id, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].waiting, p[i].turnaround);
    }

    // Gantt Chart
    printf("\nGantt Chart:\n");

    printf(" ");
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
    return 0;
}
