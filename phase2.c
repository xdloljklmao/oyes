#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct PCB
{
    int JID;
    int TTL;
    int TLL;
    int LLC;
    int TTC;
};

struct OS
{
    char M[300][4];
    char IR[4];
    int IC;
    char GR[4];
    int SI;
    int TI;
    int PI;
    int C;
    char buffer[41];
    struct PCB pcb;
    int PTR;
    int RA;
    int EM;
    char *Error[7];
    FILE *infile;
    FILE *outfile;
};

// Function declarations
void init(struct OS *os);
void initPageTable(struct OS *os);
int isBlockFree(struct OS *os, int blockIndex);
int allocate(struct OS *os);
int getAddress(struct OS *os);
void Read(struct OS *os);
void Write(struct OS *os);
void Load_register(struct OS *os);
void Store_register(struct OS *os);
void Compare_register(struct OS *os);
void Branch_on_true(struct OS *os);
void Terminate(struct OS *os, int a, int b);
void MOS(struct OS *os);
void addressMap(struct OS *os, int VA);
void Start_User_Pg(struct OS *os);
void StartExe(struct OS *os);
void LOAD(struct OS *os);
void PRINTMEMORY(struct OS *os);

// Function definitions
void initPageTable(struct OS *os)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            os->M[os->PTR + i][j] = '*';
        }
    }
}

int isBlockFree(struct OS *os, int blockIndex)
{
    int startLine = blockIndex * 10;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (os->M[startLine + i][j] != ' ')
            {
                return 0;
            }
        }
    }
    return 1;
}

// Allocate a random free block
int allocate(struct OS *os)
{
    int freeBlocks[30];
    int count = 0;
    for (int i = 0; i < 30; ++i)
    {
        if (isBlockFree(os, i))
        {
            freeBlocks[count++] = i;
        }
    }
    if (count == 0)
    {
        return -1;
    }
    int randIndex = rand() % count;
    return freeBlocks[randIndex];
}

int getAddress(struct OS *os)
{
    int address = (os->IR[2] - '0') * 10 + (os->IR[3] - '0');
    return address;
}

void init(struct OS *os)
{
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            os->M[i][j] = ' ';
        }
    }
    for (int j = 0; j < 4; j++)
    {
        os->IR[j] = ' ';
        os->GR[j] = ' ';
    }
    os->pcb.JID = 0;
    os->pcb.TTL = 0;
    os->pcb.TLL = 0;
    os->pcb.LLC = 0;
    os->pcb.TTC = 0;
    os->PTR = 0;
    os->RA = 0;
    os->IC = 0;
    os->SI = 0;
    os->TI = 0;
    os->PI = 0;
    os->C = 0;
    os->Error[0] = "No Error";
    os->Error[1] = "Out of Data";
    os->Error[2] = "Line Limit Exceeded";
    os->Error[3] = "Time Limit Exceeded";
    os->Error[4] = "Operation Code Error";
    os->Error[5] = "Operand Error";
    os->Error[6] = "Invalid Page Fault";
}


void Read(struct OS *os)
{
    os->SI = 0;
    os->pcb.TTC += 2;
    if (feof(os->infile))
    {
        return;
    }
    for (int i = 0; i < 41; i++)
        os->buffer[i] = '\0';
    fgets(os->buffer, 41, os->infile);
    if (strncmp(os->buffer, "$END", 4) == 0)
    {
        Terminate(os, 1, -1);
        return;
    }
    if (os->buffer[0] == '\0')
    {
        return;
    }
    if (os->RA < 0 || os->RA >= 300)
    {
        return;
    }
    int k = 0;
    for (int i = 0; i < 10 && k < 40; i++)
    {
        for (int j = 0; j < 4 && k < 40; j++)
        {
            os->M[os->RA][j] = os->buffer[k++];
        }
        os->RA++;
    }
}

void Write(struct OS *os)
{
    os->SI = 0;
    int address = os->RA;
    if (address < 0 || address >= 300)
    {
        return;
    }
    os->pcb.TTC++;
    os->pcb.LLC++;
    if (os->pcb.LLC > os->pcb.TLL)
    {
        Terminate(os, 2, -1);
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (os->M[address][j] == '\0')
                break;
            fputc(os->M[address][j], os->outfile);
        }
        address++;
    }
}

void Load_register(struct OS *os)
{
    int address = getAddress(os);
    if (address < 0 || address >= 300)
    {
        printf("Error: Invalid memory address in Load_register.\n");
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        os->GR[i] = os->M[address][i];
    }
}

void Store_register(struct OS *os)
{
    int address = getAddress(os);
    if (address < 0 || address >= 300)
    {
        printf("Error: Invalid memory address in Store_register.\n");
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        os->M[address][i] = os->GR[i];
    }
}

void Compare_register(struct OS *os)
{
    int address = getAddress(os);
    if (address < 0 || address >= 300)
    {
        printf("Error: Invalid memory address in Compare_register.\n");
        return;
    }
    os->C = 1;
    for (int i = 0; i < 4; i++)
    {
        if (os->GR[i] != os->M[address][i])
        {
            os->C = 0;
            break;
        }
    }
}

void Branch_on_true(struct OS *os)
{
    if (os->C == 1)
    {
        os->IC = getAddress(os);
    }
}

void Terminate(struct OS *os, int a, int b)
{
    fprintf(os->outfile, "-----------------------------------------------------------------------------------\n");
    if (a == -1 && b == -1)
    {
        fprintf(os->outfile, "Program terminated normally\n\n");
    }
    else
    {
        fprintf(os->outfile, "Program terminated abnormally\n\n");
    }
    fprintf(os->outfile, "Job ID: %d\n", os->pcb.JID);
    if (a != -1)
    {
        fprintf(os->outfile, "Error: %s\n", os->Error[a]);
    }
    if (b != -1)
    {
        fprintf(os->outfile, "Error: %s\n", os->Error[b]);
    }
    fprintf(os->outfile, "IC: %d\n", os->IC);
    fprintf(os->outfile, "TTC: %d\n", os->pcb.TTC);
    fprintf(os->outfile, "LLC: %d\n", os->pcb.LLC);
    fprintf(os->outfile, "SI: %d\n", os->SI);
    fprintf(os->outfile, "TI: %d\n", os->TI);
    fprintf(os->outfile, "PI: %d\n", os->PI);
}

void MOS(struct OS *os)
{
    if (os->SI == 1)
    {
        if (os->TI == 0 && os->PI == 0)
        {
            Read(os);
            os->SI = 0; // Reset Service Interrupt
            return;
        }
        else if (os->TI == 2 && os->PI == 0)
        {
            Terminate(os, 3, -1);
            os->SI = 0; // Reset Service Interrupt
            return;
        }
    }
    else if (os->SI == 2)
    {
        if (os->TI == 0 && os->PI == 0)
        {
            Write(os);
            os->SI = 0; // Reset Service Interrupt
            return;
        }
        else if (os->TI == 2 && os->PI == 0)
        {
            Write(os);
            os->SI = 0; // Reset Service Interrupt
            Terminate(os, 3, -1);
            return;
        }
    }
    else if (os->SI == 3)
    {
        if ((os->TI == 0 || os->TI == 2) && os->PI == 0)
        {
            Terminate(os, -1, -1);
            os->SI = 0; // Reset Service Interrupt
            return;
        }
    }

    if (os->PI == 1)
    {
        if (os->TI == 0)
        {
            Terminate(os, 4, -1);
            return;
        }
        else if (os->TI == 2)
        {
            Terminate(os, 3, 4);
            return;
        }
    }
    else if (os->PI == 2)
    {
        if (os->TI == 0)
        {
            Terminate(os, 5, -1);
            return;
        }
        else if (os->TI == 2)
        {
            Terminate(os, 3, 5);
            return;
        }
    }
    else if (os->PI == 3)
    {
        if (os->TI == 0)
        {
            os->RA = getAddress(os);

            if (((os->IR[0] == 'G' && os->IR[1] == 'D') ||
                (os->IR[0] == 'S' && os->IR[1] == 'R')) &&
                os->M[os->PTR + (os->RA / 10)][0] == '*')
            {
                int newBlock = allocate(os); // Allocate a new block for DATA

                os->M[os->PTR + (os->RA / 10)][0] = '0';                   // Mark valid
                os->M[os->PTR + (os->RA / 10)][1] = '0';
                os->M[os->PTR + (os->RA / 10)][2] = (newBlock / 10) + '0'; // Store block #
                os->M[os->PTR + (os->RA / 10)][3] = (newBlock % 10) + '0';

                addressMap(os, getAddress(os));
                os->PI = 0; // Reset Page Fault Interrupt
                return;
            }
            else
            {
                Terminate(os, 6, -1);
                return;
            }
        }
        else if (os->TI == 2)
        {
            Terminate(os, 3, -1);
            return;
        }
    }

    if (os->PI == 0 && os->SI == 0 && os->TI == 2)
    {
        Terminate(os, 3, -1);
        return;
    }
}


void addressMap(struct OS *os, int VA)
{
    int pageNumber = VA / 10;  // Calculate the page number
    int offset     = VA % 10;

    // Check if the page is present in the page table
    if (os->M[os->PTR + pageNumber][0] == '*')
    {
        os->PI = 3;                       // Set Page Fault Interrupt
        MOS(os);                          // Call the MOS function to handle the page fault/terminate
    }

    // Return if page fault is invalid
    if (os->PI == 3)
        return;

    // PTE refers to the block number stored in that page frame
    int PTE = (os->M[os->PTR + pageNumber][2] - '0') * 10 +
            (os->M[os->PTR + pageNumber][3] - '0');
    os->RA = PTE * 10 + offset;         // Combine with offset
}


void Start_User_Pg(struct OS *os)
{
    while (os->IC < 100)
    {
        addressMap(os, os->IC);  // Map the IC to the real address

        // Check if memory location is blank
        if (os->M[os->RA][0] == ' ' && os->M[os->RA][1] == ' ' &&
            os->M[os->RA][2] == ' ' && os->M[os->RA][3] == ' ')
        {
            os->IC++;
            continue;
        }

        memcpy(os->IR, os->M[os->RA], 4);

        // Check for valid instruction
        if (!((os->IR[0] == 'G' && os->IR[1] == 'D') ||
            (os->IR[0] == 'P' && os->IR[1] == 'D') ||
            (os->IR[0] == 'L' && os->IR[1] == 'R') ||
            (os->IR[0] == 'S' && os->IR[1] == 'R') ||
            (os->IR[0] == 'C' && os->IR[1] == 'R') ||
            (os->IR[0] == 'B' && os->IR[1] == 'T') ||
            os->IR[0] == 'H'))
        {
            os->PI = 1;            // Operation Code Error
            MOS(os);               // handle error/terminate
            return;
        }

        // Check for operand error
        os->IC++;
        addressMap(os, os->IC);
        memcpy(os->IR, os->M[os->RA], 4);

        if (os->IR[0] >= '0' && os->IR[0] <= '9')
        {
            os->PI = 2;  // Operand Error
            MOS(os);
            return;
        }

        os->IC--;
        addressMap(os, os->IC);
        memcpy(os->IR, os->M[os->RA], 4);

        if (os->IR[0] != 'H')  // H has no operand
        {
            int oprand = getAddress(os);
            addressMap(os, oprand);
        }

        // Check for any PI interrupt
        if (os->PI != 0)
            return;

        // Increment Instruction Counter
        os->IC++;

        // Execute the instruction
        if (os->IR[0] == 'G' && os->IR[1] == 'D')
        {
            os->SI = 1;
            MOS(os);
        }
        else if (os->IR[0] == 'P' && os->IR[1] == 'D')
        {
            os->SI = 2;
            MOS(os);
        }
        else if (os->IR[0] == 'L' && os->IR[1] == 'R')
        {
            Load_register(os);
        }
        else if (os->IR[0] == 'S' && os->IR[1] == 'R')
        {
            Store_register(os);
        }
        else if (os->IR[0] == 'C' && os->IR[1] == 'R')
        {
            Compare_register(os);
        }
        else if (os->IR[0] == 'B' && os->IR[1] == 'T')
        {
            Branch_on_true(os);
        }
        else if (os->IR[0] == 'H')
        {
            os->SI = 3;
            MOS(os);
            return;
        }

        // Check Time/Line limits
        if (os->pcb.TTC > os->pcb.TTL)
        {
            os->TI = 2;  // Time Limit Exceeded
            MOS(os);
            return;
        }
        if (os->pcb.LLC > os->pcb.TLL)
        {
            return;
        }

        // After Read, if buffer indicates end
        if (strncmp(os->buffer, "$END", 4) == 0)
            return;
    }
}


void StartExe(struct OS *os)
{
    os->IC = 0;
    Start_User_Pg(os);
}


void LOAD(struct OS *os)
{
    int memPtr = 0;           // Persistent memory index
    bool loadMode = false;    // Only true between $AMJ and $DTA
    int cardNo = 0;           // Card Number
    char buffer[41];

    while (1)
    {
        memset(buffer, '\0', sizeof(buffer));

        if (fgets(buffer, sizeof(buffer), os->infile) == NULL)
            break;

        if (strncmp(buffer, "$AMJ", 4) == 0)
        {
            init(os);

            // Extract values from buffer
            char jidStr[5] = {0}, ttlStr[5] = {0}, tllStr[5] = {0};
            strncpy(jidStr, buffer + 4, 4);
            strncpy(ttlStr, buffer + 8, 4);
            strncpy(tllStr, buffer + 12, 4);

            // Convert to integers
            os->pcb.JID = atoi(jidStr);
            os->pcb.TTL = atoi(ttlStr);
            os->pcb.TLL = atoi(tllStr);

            os->pcb.LLC = 0;
            os->pcb.TTC = 0;

            os->PTR = 10 * allocate(os);
            initPageTable(os);

            cardNo = os->PTR;
            memPtr = 0;
            loadMode = true;
        }
        else if (strncmp(buffer, "$DTA", 4) == 0)
        {
            loadMode = false;
            StartExe(os);
        }
        else if (strncmp(buffer, "$END", 4) == 0)
        {
            continue;
        }
        else if (loadMode)
        {
            memPtr = allocate(os);

            os->M[cardNo][0] = '0';
            os->M[cardNo][1] = '0';
            os->M[cardNo][2] = (memPtr / 10) + '0';
            os->M[cardNo][3] = (memPtr % 10) + '0';

            cardNo++;
            if (cardNo == 10 + os->PTR)
                cardNo = os->PTR;

            int k = 0;
            int startAddr = memPtr * 10;
            while (buffer[k] != '\0' && startAddr < 300 && k < 40)
            {
                for (int j = 0; j < 4 && k < 40; j++)
                {
                    if (buffer[k] == '\0')
                        break;
                    os->M[startAddr][j] = buffer[k];
                    if (buffer[k] == 'H')
                        j = 3;
                    k++;
                }
                startAddr++;
            }
        }
    }
}


void PRINTMEMORY(struct OS *os)
{
    printf("\nMemory Dump:\n");
    for (int i = 0; i < 300; i++)
    {
        printf("[%d] ", i);
        for (int j = 0; j < 4; j++)
        {
            printf("%c", os->M[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    struct OS os;
    srand(time(NULL)); // Seed once at program start (for random number generation)

    // Open files for reading and writing
    os.infile = fopen("input.txt", "r");
    os.outfile = fopen("output.txt", "w");

    if (os.infile == NULL)
    {
        printf("Failure: File not found!\n");
        return 1;
    }

    // Call the LOAD and PRINTMEMORY functions
    LOAD(&os);
    PRINTMEMORY(&os);

    // Close the files
    fclose(os.infile);
    fclose(os.outfile);

    return 0;
}
