/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);
void parseField(int, int *, int *, int *, int *);

int main(int argc, char *argv[])
{
    int i;
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

		/* TODO: */

    // initialize registers and pc
    for (i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }
    state.pc = 0;
    
    // fetch and execute instructions
    int instruction, opcode, regA, regB, destReg, offsetField;
    int isHalted = 0, executeCnt = 0;

    while (1) {
        printState(&state);
        // fetch and increase pc
        instruction = state.mem[state.pc++]; // now pc has next instruction's address
                                             
        opcode = instruction >> 22;

        parseField(instruction, &regA, &regB, &destReg, &offsetField);

        switch (opcode) {
            case 0b000: // add
                state.reg[destReg] = state.reg[regA] + state.reg[regB];
                break;
            case 0b001: // nor
                state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
                break;
            case 0b010: // lw
                state.reg[regB] = state.mem[state.reg[regA] + offsetField];
                break;
            case 0b011: // sw
                state.mem[state.reg[regA] + offsetField] = state.reg[regB];
                break;
            case 0b100: // beq
                if (state.reg[regA] == state.reg[regB]) { // branching
                    state.pc += offsetField;
                }
                break;
            case 0b101: // jalr
                state.reg[regB] = state.pc;
                state.pc = state.reg[regA];
                break;
            case 0b110: // halt
                isHalted = 1;
                break;
            case 0b111: // noop
                break;
            default:
                printf("invalid opcode\n");
                exit(1);
        }

        executeCnt++;

        // terminate program
        if (isHalted) break;
    }

    printf("machine halted\n");
    printf("total of %d instructions executed\n", executeCnt);
    printf("final state of machine:\n");
    printState(&state);

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}

void parseField(int instruction, int *regA, int *regB, int *destReg, int* offsetField) {
    int opcode = instruction >> 22;

    switch (opcode) {
        case 0b000:
        case 0b001:
            *regA = (instruction >> 19) & 0b111;
            *regB = (instruction >> 16) & 0b111;
            *destReg = (instruction) & 0b111;
            break;
        case 0b010:
        case 0b011:
        case 0b100:
            *regA = (instruction >> 19) & 0b111;
            *regB = (instruction >> 16) & 0b111;
            *offsetField = convertNum(instruction & 0xffff);
            break;
        case 0b101:
            *regA = (instruction >> 19);
            *regB = (instruction >> 16) & 0b111;
            break;
        case 0b110:
        case 0b111:
            break;
        default:
            printf("invalid opcode\n");
            exit(1);
    }
}