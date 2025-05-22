#include <stdio.h>

struct Process {
    int id, at, bt, rm, wt, tat, ct;
};

void SJFPreemptive(struct Process p[], int n, int gantt_chart[], int gantt_time[], int *gc_index) {
    int time = 0, completed = 0;

    for (int i = 0; i < n; i++)
        p[i].rm = p[i].bt;

    while (completed < n) {
        int min_index = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rm > 0) {
                if (min_index == -1 || p[i].rm < p[min_index].rm) {
                    min_index = i;
                }
            }
        }

        if (min_index == -1) {
            time++;  
            continue;
        }

        if (*gc_index == 0 || gantt_chart[*gc_index - 1] != p[min_index].id) {
            gantt_chart[*gc_index] = p[min_index].id;
            gantt_time[*gc_index] = time;
            (*gc_index)++;
        }

        p[min_index].rm--;
        time++;

        if (p[min_index].rm == 0) {  
            completed++;
            p[min_index].ct = time;
            p[min_index].tat = p[min_index].ct - p[min_index].at;
            p[min_index].wt = p[min_index].tat - p[min_index].bt;
        }
    }
    gantt_time[*gc_index] = time;  
}

void Result(struct Process p[], int n) {
    float total_wt = 0, total_tat = 0;

    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].wt, p[i].tat);
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }

    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

void GanttChart(int gantt_chart[], int gantt_time[], int gc_index) {
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
    for (int i = 0; i < n; i++)
        scanf("%d %d %d", &p[i].id, &p[i].at, &p[i].bt);

    int gantt_chart[100], gantt_time[100], gc_index = 0;

    SJFPreemptive(p, n, gantt_chart, gantt_time, &gc_index);
    Result(p, n);
    GanttChart(gantt_chart, gantt_time, gc_index);

    return 0;
}
