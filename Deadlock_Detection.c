#include <stdio.h>
#include <stdbool.h>

struct Process {
    int allocated[10];  
    int request[10];    
    bool finished;      
};

void printTable(int n, int m, struct Process processes[], int available[], int work[]) {
    printf("\nCurrent System State:\n");
    printf("Process   Allocated Resources   Requested Resources       Work     Finished\n");
    printf("---------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < m; j++) {
            printf("   %d ", processes[i].allocated[j]);
        }
        printf("    ");
        for (int j = 0; j < m; j++) {
            printf("   %d ", processes[i].request[j]);
        }
        printf("    ");
        for (int j = 0; j < m; j++) {
            printf("   %d ", work[j]);
        }
        printf("           %s \n", processes[i].finished ? "Yes" : "No");
    }

    printf("Available Resources: ");
    for (int i = 0; i < m; i++) {
        printf("%d  ", available[i]);
    }
    printf("\n---------------------------------------------------------------\n");
}

int main() {
    int n, m; 
    int available[10];  
    int work[10]; 
    struct Process processes[10]; 

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource types: ");
    scanf("%d", &m);
    printf("Enter available instances for each resource (e.g., R1, R2, ...):\n");
    for (int i = 0; i < m; i++) {
        printf("Enter number of instances for R%d: ", i + 1);
        scanf("%d", &available[i]);
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess P%d\n", i);
        printf("Enter allocated resources: ");
        for (int j = 0; j < m; j++) {
            scanf("%d", &processes[i].allocated[j]);
        }

        printf("Enter requested resources: ");
        for (int j = 0; j < m; j++) {
            scanf("%d", &processes[i].request[j]);
        }

        processes[i].finished = false; 
    }
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }

    printTable(n, m, processes, available, work);

    while (1) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!processes[i].finished) { 
                bool canAllocate = true;
                
                for (int j = 0; j < m; j++) {
                    if (processes[i].request[j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    
                    for (int j = 0; j < m; j++) {
                        work[j] += processes[i].allocated[j];  
                    }
                    processes[i].finished = true; 
                    found = true;

                    printf("\nProcess P%d has finished. Updated system state:\n", i);
                    printTable(n, m, processes, available, work);
                }
            }
        }
        if (!found) { 
            break;
        }
    }
    bool deadlock = false;
    for (int i = 0; i < n; i++) {
        if (!processes[i].finished) {
            deadlock = true;
            printf("Deadlock detected! Process P%d is stuck.\n", i);
        }
    }
    if (!deadlock) {
        printf("No deadlock detected! System is safe.\n");
    }
    return 0;
}
