/*Phase1 Updated - Arithmetic Operation Instruction Included*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class VM
{
private:
    char buffer[40];
    char Memory[100][4];
    char IR[4];  // Instruction Register
    char R[4];   // General purpose Register 
    bool C;      // Toggle Register
    int IC;      // Instruction counter
    int SI;      // System Interrupt
    fstream infile;
    ofstream outfile;

    void resetAll() {
        fill(buffer, buffer + sizeof(buffer), '\0');
        fill(&Memory[0][0], &Memory[0][0] + sizeof(Memory), '\0');
        fill(IR, IR + sizeof(IR), '\0');
        fill(R, R + sizeof(R), '\0');
        IC = 0;
        C = true;
        SI = 0;
    }

    void resetBuffer() {
        fill(buffer, buffer + sizeof(buffer), '\0');
    }

    void masterMode() {
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
        }
        SI = 0;
    }

    void READ() {
        cout << "Read function called\n";
        string data;
        getline(infile, data);
        int len = data.size();
        for (int i = 0; i < len; i++) {
            buffer[i] = data[i];
        }
        int buff = 0, mem_ptr = (IR[2] - '0') * 10;
        while (buff < 40 && buffer[buff] != '\0') {
            for (int i = 0; i < 4; i++) {
                Memory[mem_ptr][i] = buffer[buff];
                buff++;
            }
            mem_ptr++;
        }
        resetBuffer();
    }

    void WRITE() {
        cout << "Write function called\n";
        outfile.open("output.txt", ios::app);
        for (int i = (IR[2] - '0') * 10; i < (IR[2] - '0' + 1) * 10; i++) {
            for (int j = 0; j < 4; j++) {
                if (Memory[i][j] != '\0') {
                    outfile << Memory[i][j];
                }
            }
        }
        outfile << "\n";
        outfile.close();
    }

    void TERMINATE() {
        outfile.open("output.txt", ios::app);
        cout << "Terminate called\n\n";
        outfile << "\n\n";
        outfile.close();
    }

    void LOAD() {
        if (infile.is_open()) {
            string s;
            while (getline(infile, s)) {
                if (s.substr(0, 4) == "$AMJ") {
                    resetAll();
                    cout << "New Job started\n";
                } else if (s.substr(0, 4) == "$DTA") {
                    cout << "Data card loading\n";
                    resetBuffer();
                    MOSstartexe();
                } else if (s.substr(0, 4) == "$END") {
                    cout << "END of Job\n";
                } else {
                    cout << "Program Card loading\n";
                    int length = s.size();
                    resetBuffer();
                    for (int i = 0; i < length; i++) {
                        buffer[i] = s[i];
                    }
                    int buff = 0;
                    int ref = 0;
                    while (buff < 40 && buffer[buff] != '\0') {
                        for (int j = 0; j < 4; j++) {
                            if (buffer[buff] == 'H') {
                                Memory[IC][j] = 'H';
                                buff++;
                                break;
                            }
                            Memory[IC][j] = buffer[buff];
                            buff++;
                        }
                        IC++;
                    }
                }
            }
            infile.close(); // Input File closed
        }
    }

    void MOSstartexe() {
        IC = 0;
        executeUserProgram();
    }

    void executeUserProgram() {
        while (IC < 99 && Memory[IC][0] != '\0') {
            for (int i = 0; i < 4; i++) {
                IR[i] = Memory[IC][i];
            }
            IC++;

            if (IR[0] == 'G' && IR[1] == 'D') {
                SI = 1;
                masterMode();
            } else if (IR[0] == 'P' && IR[1] == 'D') {
                SI = 2;
                masterMode();
            } else if (IR[0] == 'H') {
                SI = 3;
                masterMode();
                return;
            } else if (IR[0] == 'L' && IR[1] == 'R') {
                for (int i = 0; i < 4; i++) {
                    R[i] = Memory[(IR[2] - '0') * 10 + (IR[3] - '0')][i];
                }
            } else if (IR[0] == 'S' && IR[1] == 'R') {
                for (int i = 0; i < 4; i++) {
                    Memory[(IR[2] - '0') * 10 + (IR[3] - '0')][i] = R[i];
                }
            } else if (IR[0] == 'C' && IR[1] == 'R') {
                int cnt = 0;
                for (int i = 0; i < 4; i++) {
                    if (Memory[(IR[2] - '0') * 10 + (IR[3] - '0')][i] == R[i]) {
                        cnt++;
                    }
                }
                C = (cnt == 4);
            } else if (IR[0] == 'B' && IR[1] == 'T') {
                if (C) {
                    IC = (IR[2] - '0') * 10 + (IR[3] - '0');
                }
            }
            // New AD - ADD instruction
            else if (IR[0] == 'A' && IR[1] == 'D') {
                int regVal = stoi(string(R, 4));
                int memVal = stoi(string(Memory[(IR[2] - '0') * 10 + (IR[3] - '0')], 4));
                int result = regVal + memVal;
                string resStr = to_string(result);
                fill(R, R + 4, ' ');
                for (int i = 0; i < resStr.size() && i < 4; i++) {
                    R[i] = resStr[i];
                }
            }
        }
    }

public:
    VM() {
        infile.open("input.txt", ios::in);
        resetAll();
        LOAD();
    }
};

int main() {
    VM v;
    return 0;
}
