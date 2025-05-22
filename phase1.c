#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char M[100][4];
char R[4];
char IR[4];
int IC = 0, SI = 0;
char buffer[40];
int C = 0;

FILE *Input, *Output;

void clearBuffer() {
    for (int i = 0; i < 40; i++) {
        buffer[i] = '\0';
    }
}

int GETLOCATION() {
    int WordNumber = ((IR[2] - '0') * 10) + (IR[3] - '0');
    return WordNumber;
}

void INIT() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 4; j++) {
            M[i][j] = '\0';
        }
    }

    for (int i = 0; i < 4; i++) {
        R[i] = ' ';
        IR[i] = ' ';
    }

    C = 0;
    IC = 0;
}

void READ() {
    int loc = GETLOCATION();
    int k = 0;
    clearBuffer();

    fgets(buffer, 40, Input);

    while (k < 99 && buffer[k] != '\0') {
        for (int j = 0; j < 4; j++) {
            if (buffer[k] == '\n') {
                k++;
                continue;
            }
            M[loc][j] = buffer[k];
            k++;
        }
        loc++;
    }
}

void WRITE() {
    Output = fopen("output.txt", "a");
    if (Output == NULL) {
        printf("Could not open output file.\n");
        return;
    }

    int loc = GETLOCATION();
    for (int i = loc; i < loc + 10; i++) {
        for (int k = 0; k < 4; k++) {
            if (M[i][k] != '\0') {
                fprintf(Output, "%c", M[i][k]);
            }
        }
    }
    fprintf(Output, "\n");
    fclose(Output);
}

void TERMINATE() {
    Output = fopen("output.txt", "a");
    if (Output == NULL) {
        printf("Could not open output file.\n");
        return;
    }
    fprintf(Output, "\n");
    fclose(Output);
}

void MOS() {
    switch (SI) {
        case 1:
            READ();
            break;
        case 2:
            WRITE();
            break;
        case 3:
            TERMINATE();
            break;
        default:
            break;
    }
    SI = 0;
}

void EXECUTION() {
    while (1) {
        for (int i = 0; i < 4; i++) {
            IR[i] = M[IC][i];
        }
        IC++;

        if (IR[0] == 'G') {
            SI = 1;
            MOS();
        } else if (IR[0] == 'P') {
            SI = 2;
            MOS();
        } else if (IR[0] == 'H') {
            SI = 3;
            MOS();
            break;
        } else if (IR[0] == 'L') {
            int loc = GETLOCATION();
            for (int i = 0; i < 4; i++) {
                R[i] = M[loc][i];
            }
        } else if (IR[0] == 'S') {
            int loc = GETLOCATION();
            for (int i = 0; i < 4; i++) {
                M[loc][i] = R[i];
            }
        } else if (IR[0] == 'C') {
            int loc = GETLOCATION();
            C = 1; 
            for (int i = 0; i < 4; i++) {
                if (M[loc][i] != R[i]) {
                    C = 0;  
                    break;
                }
            }
        } else if (IR[0] == 'B') {
            if (C) {
                int loc = GETLOCATION();
                IC = loc;
            }
        }
    }
}

void STARTEXECUTION() {
    IC = 0;
    EXECUTION();
}

void LOAD() {
    if (Input == NULL) {
        printf("Could not open input file.\n");
        return;
    }

    int jobStarted = 0;

    while (fgets(buffer, 40, Input) != NULL) {
        if (strncmp(buffer, "$AMJ", 4) == 0) {
            if (jobStarted) {
                TERMINATE();
            }
            INIT();
            jobStarted = 1;
        } else if (strncmp(buffer, "$DTA", 4) == 0) {
            if (!jobStarted) {
                printf("Error: $DTA found before $AMJ.\n");
                return;
            }
            clearBuffer();
            STARTEXECUTION();
        } else if (strncmp(buffer, "$END", 4) == 0) {
            if (!jobStarted) {
                printf("Error: $END found before $AMJ.\n");
                return;
            }
            for (int i = 0; i < 100; i++) {
                printf("M[%d]:%c%c%c%c\n", i, M[i][0], M[i][1], M[i][2], M[i][3]);
            }
            TERMINATE();
            jobStarted = 0; 
        } else {
            int i = IC;
            int j = 0;
            while (j < 99 && buffer[j] != '\0' && buffer[j] != '\n') {
                for (int k = 0; k < 4; k++) {
                    if (buffer[j] == 'H' && buffer[j] != '\n') {
                        M[i][k] = 'H';
                        j++;
                        break;
                    }
                    M[i][k] = buffer[j++];
                }
                i++;
            }
            IC = i;
        }
        clearBuffer();
    }

    if (jobStarted) {
        TERMINATE();
    }
}

int main() {
    Input = fopen("input.txt", "r");
    Output = fopen("output.txt", "w");

    if (Input == NULL) {
        printf("Can't open input file\n");
        return 1;
    } else {
        printf("Input file opened successfully\n");
    }

    LOAD();

    fclose(Input);
    fclose(Output);

    return 0;
}
