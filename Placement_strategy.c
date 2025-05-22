#include <stdio.h>
#include <stdlib.h>

struct Process {
    int size;  
    int allocated;  
};

void printMemoryState(int blocks[], int block_sizes[], int num_blocks, struct Process processes[], int num_processes) {
    printf("\nCurrent Memory Block State:\n");
    for (int i = 0; i < num_blocks; i++) {
        printf("Block B%d (Original Size: %dKB, Remaining: %dKB) ", i + 1, block_sizes[i], blocks[i]);
    }
    printf("\n\nProcess Allocations:\n");
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].allocated > 0) {
            printf("Process P%d (Size: %dKB) allocated %dKB\n", i + 1, processes[i].size, processes[i].allocated);
        } else {
            printf("Process P%d (Size: %dKB) not allocated\n", i + 1, processes[i].size);
        }
    }
    printf("\n");
}

void first_fit(int blocks[], int block_sizes[], int num_blocks, struct Process processes[], int num_processes) {
    printf("\nFirst Fit Allocation:\n");
    for (int i = 0; i < num_processes; i++) {
        int allocated = 0;
        for (int j = 0; j < num_blocks; j++) {
            if (blocks[j] >= processes[i].size) {
                processes[i].allocated = processes[i].size;
                blocks[j] -= processes[i].size;
                printf("Process P%d (Size: %dKB) allocated to Block B%d (Remaining Block Size: %dKB)\n", 
                    i + 1, processes[i].size, j + 1, blocks[j]);
                allocated = 1;
                break;
            }
        }
        if (!allocated) {
            printf("Process P%d could not be allocated.\n", i + 1);
        }
    }
}

void next_fit(int blocks[], int block_sizes[], int num_blocks, struct Process processes[], int num_processes) {
    static int last_block = 0;
    printf("\nNext Fit Allocation:\n");
    for (int i = 0; i < num_processes; i++) {
        int allocated = 0;
        for (int j = last_block; j < num_blocks; j++) {
            if (blocks[j] >= processes[i].size) {
                processes[i].allocated = processes[i].size;
                blocks[j] -= processes[i].size;
                printf("Process P%d (Size: %dKB) allocated to Block B%d (Remaining Block Size: %dKB)\n", 
                    i + 1, processes[i].size, j + 1, blocks[j]);
                last_block = j;
                allocated = 1;
                break;
            }
        }
        if (!allocated) {
            for (int j = 0; j < last_block; j++) {
                if (blocks[j] >= processes[i].size) {
                    processes[i].allocated = processes[i].size;
                    blocks[j] -= processes[i].size;
                    printf("Process P%d (Size: %dKB) allocated to Block B%d (Remaining Block Size: %dKB)\n", 
                        i + 1, processes[i].size, j + 1, blocks[j]);
                    last_block = j;
                    allocated = 1;
                    break;
                }
            }
        }
        if (!allocated) {
            printf("Process P%d could not be allocated.\n", i + 1);
        }
    }
}

void best_fit(int blocks[], int block_sizes[], int num_blocks, struct Process processes[], int num_processes) {
    printf("\nBest Fit Allocation:\n");
    for (int i = 0; i < num_processes; i++) {
        int min_diff = 10000;  
        int best_block = -1;
        for (int j = 0; j < num_blocks; j++) {
            if (blocks[j] >= processes[i].size && blocks[j] - processes[i].size < min_diff) {
                min_diff = blocks[j] - processes[i].size;
                best_block = j;
            }
        }
        if (best_block != -1) {
            processes[i].allocated = processes[i].size;
            blocks[best_block] -= processes[i].size;
            printf("Process P%d (Size: %dKB) allocated to Block B%d (Remaining Block Size: %dKB)\n", 
                i + 1, processes[i].size, best_block + 1, blocks[best_block]);
        } else {
            printf("Process P%d could not be allocated.\n", i + 1);
        }
    }
}

void worst_fit(int blocks[], int block_sizes[], int num_blocks, struct Process processes[], int num_processes) {
    printf("\nWorst Fit Allocation:\n");
    for (int i = 0; i < num_processes; i++) {
        int max_diff = -1;  
        int worst_block = -1;
        for (int j = 0; j < num_blocks; j++) {
            if (blocks[j] >= processes[i].size && blocks[j] - processes[i].size > max_diff) {
                max_diff = blocks[j] - processes[i].size;
                worst_block = j;
            }
        }
        if (worst_block != -1) {
            processes[i].allocated = processes[i].size;
            blocks[worst_block] -= processes[i].size;
            printf("Process P%d (Size: %dKB) allocated to Block B%d (Remaining Block Size: %dKB)\n", 
                i + 1, processes[i].size, worst_block + 1, blocks[worst_block]);
        } else {
            printf("Process P%d could not be allocated.\n", i + 1);
        }
    }
}

int main() {
    int num_blocks, num_processes;

    printf("Enter the number of memory blocks: ");
    scanf("%d", &num_blocks);

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    int block_sizes[num_blocks];
    int blocks[num_blocks];
    struct Process processes[num_processes];

    for (int i = 0; i < num_blocks; i++) {
        printf("Enter size for Block B%d: ", i + 1);
        scanf("%d", &block_sizes[i]);
        blocks[i] = block_sizes[i];  
    }

    for (int i = 0; i < num_processes; i++) {
        printf("Enter size for Process P%d: ", i + 1);
        scanf("%d", &processes[i].size);
        processes[i].allocated = 0;
    }

    int choice;
    do {
        printMemoryState(blocks, block_sizes, num_blocks, processes, num_processes);

        printf("\nChoose a Memory Allocation Strategy:\n");
        printf("1. First Fit\n");
        printf("2. Next Fit\n");
        printf("3. Best Fit\n");
        printf("4. Worst Fit\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                first_fit(blocks, block_sizes, num_blocks, processes, num_processes);
                break;
            case 2:
                next_fit(blocks, block_sizes, num_blocks, processes, num_processes);
                break;
            case 3:
                best_fit(blocks, block_sizes, num_blocks, processes, num_processes);
                break;
            case 4:
                worst_fit(blocks, block_sizes, num_blocks, processes, num_processes);
                break;
            case 5:
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

        printf("\nDo you want to try another allocation strategy? (1 for Yes, 0 for No): ");
        scanf("%d", &choice);
        if (choice) {
            for (int i = 0; i < num_blocks; i++) {
                blocks[i] = block_sizes[i];
            }
            for (int i = 0; i < num_processes; i++) {
                processes[i].allocated = 0;
            }
        }

    } while (choice != 0);

    return 0;
}

