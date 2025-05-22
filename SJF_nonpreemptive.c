#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, wt, tat, ct;
    int completed; 
};

void findWT_TAT_CT(struct Process p[], int n, int gantt_chart[], int gantt_time[], int *gc_index) {
    int time = 0, completed = 0;

    while (completed < n) {
        int min_index = -1;
        int min_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                min_index = i;
            }
        }

        if (min_index == -1) {
            time++;
            continue;
        }

        gantt_chart[*gc_index] = p[min_index].pid;
        gantt_time[*gc_index] = time;
        (*gc_index)++;

        p[min_index].wt = time - p[min_index].at;
        p[min_index].ct = time + p[min_index].bt;
        p[min_index].tat = p[min_index].ct - p[min_index].at;
        p[min_index].completed = 1; 
        completed++;

        time = p[min_index].ct;
    }
    gantt_time[*gc_index] = time; 
}

void printResults(struct Process p[], int n) {
    float total_wt = 0, total_tat = 0;

    printf("\nProcess\tAT\tBT\tWT\tTAT\tCT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat, p[i].ct);
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }

    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

void printGanttChart(int gantt_chart[], int gantt_time[], int gc_index) {
    printf("\nGantt Chart:\n ");

    for (int i = 0; i < gc_index; i++) printf("------");
    printf("\n|");

    for (int i = 0; i < gc_index; i++) printf(" P%d |", gantt_chart[i]);
    printf("\n ");

    for (int i = 0; i < gc_index; i++) printf("------");
    printf("\n");

    for (int i = 0; i <= gc_index; i++) printf("%d\t", gantt_time[i]);
    printf("\n");
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter Process ID, Arrival Time and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].completed = 0;
    }

    int gantt_chart[100], gantt_time[100], gc_index = 0;

    findWT_TAT_CT(p, n, gantt_chart, gantt_time, &gc_index);
    printResults(p, n);
    printGanttChart(gantt_chart, gantt_time, gc_index);

    return 0;
}
