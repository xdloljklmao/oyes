#include <stdio.h>

#define DISK_SIZE 200

void sort(int arr[], int n) {
    int i, j, temp;
    for(i = 0; i < n-1; i++) {
        for(j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int n, head, i, j;
    int requests[100];
    char direction;

    printf("Enter number of track requests: ");
    scanf("%d", &n);

    printf("Enter the track numbers (0 - 199):\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter initial head position (0 - 199): ");
    scanf("%d", &head);

    printf("Enter direction (C for Clockwise, A for Anticlockwise): ");
    scanf(" %c", &direction);

    requests[n] = head;
    n++;

    sort(requests, n);

    int head_index = 0;
    for(i = 0; i < n; i++) {
        if(requests[i] == head) {
            head_index = i;
            break;
        }
    }

    int total_movement = 0;

    printf("\n< SCAN Disk Scheduling Algorithm >\n\n");

    if(direction == 'C' || direction == 'c') {

        for(i = head_index; i < n; i++) {
            int distance;
            if(i == head_index) {
                distance = 0;
            } else {
                distance = requests[i] - requests[i-1];
            }
            total_movement += distance;
            printf("Current track: %d (No. of tracks traversed: %d)\n", requests[i], distance);
        }

        printf("\nReached the top end (199). Changing direction towards 0.\n\n");

        for(i = head_index-1; i >= 0; i--) {
            int distance = requests[i+1] - requests[i];
            total_movement += distance;
            printf("Current track: %d (No. of tracks traversed: %d)\n", requests[i], distance);
        }
    }
    else if(direction == 'A' || direction == 'a') {

        for(i = head_index; i >= 0; i--) {
            int distance;
            if(i == head_index) {
                distance = 0;
            } else {
                distance = requests[i+1] - requests[i];
            }
            total_movement += distance;
            printf("Current track: %d (No. of tracks traversed: %d)\n", requests[i], distance);
        }

        printf("\nReached the bottom end (0). Changing direction towards 199.\n\n");

        for(i = head_index+1; i < n; i++) {
            int distance = requests[i] - requests[i-1];
            total_movement += distance;
            printf("Current track: %d (No. of tracks traversed: %d)\n", requests[i], distance);
        }
    }
    else {
        printf("Invalid direction input! Please enter 'C' or 'A'.\n");
        return 1;
    }

    printf("\nSeek Time = %d tracks\n", total_movement);

    return 0;
}
