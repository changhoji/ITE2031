/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

struct branch {
	char name[7];
	int address;
	struct branch *next;
} *head;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int getBinaryOpcode(char *);
int isValidRegister(char *);
int getWordOffset(char *);
int getBranchOffset(char *, int);
int findBranchAddress(char *);
void freeBranchList(struct branch *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	
	head = (struct branch *)malloc(sizeof(struct branch));
	strcpy(head->name, "");
	head->address = -1;
	head->next = NULL;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	// /* here is an example for how to use readAndParse to read a line from
	// 	 inFilePtr */
	// if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
	// 	/* reached end of file */
	// }

	/* TODO: Phase-1 label calculation */
	int line = 0;
	struct branch *cur;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		// set labels
		if (strcmp(label, "")){ // add label to linked list
			cur = head;
			while (cur->next != NULL) { // find last node
				if (!strcmp(cur->name, label)) {
					printf("duplicant label\n");
					exit(1);
				}
				cur = cur->next;
			}
			if (!strcmp(cur->name, label)) {
				printf("duplicant label\n");
				exit(1);
			}
			
			// create new branch node
			struct branch *newNode = (struct branch*)malloc(sizeof(struct branch));
			if(newNode == 0) {
				printf("failed to malloc\n");
				exit(1);
			}
			
			strcpy(newNode->name, label);
			newNode->address = line;
			newNode->next = NULL;

			cur->next = newNode;
			cur = cur->next;
		}

		// increase line number
		line++;
	}

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */
	line = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		int res = 0;
		int regA, regB, destReg, offsetField;

		int bop = getBinaryOpcode(opcode);

		res |= (bop << 22);

		switch (bop) {
			case 0b000:  // add
			case 0b001:  // nor
				if (!isValidRegister(arg0) || !isValidRegister(arg1) || !isValidRegister(arg2)) {
					printf("invalid argument\n");
					exit(1);
				}

				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);
				sscanf(arg2, "%d", &destReg);

				res |= (regA << 19);
				res |= (regB << 16);
				res |= destReg;

				break;
				
			case 0b010:  // lw
			case 0b011:  // sw
			case 0b100:  // beq
				if (!isValidRegister(arg0) || !isValidRegister(arg1)) {
					printf("invalid argument\n");
					exit(1);
				}

				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);

				if (bop <= 0b011) { // lw & sw
					offsetField = getWordOffset(arg2);
				}
				else {
					offsetField = getBranchOffset(arg2, line);
				}

				res |= (regA << 19);
				res |= (regB << 16);
				res |= offsetField;

				break;

			case 0b101:  // jalr
				if (!isValidRegister(arg0) || !isValidRegister(arg1)) {
					printf("invalid argument\n");
					exit(1);
				}

				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);

				res |= (regA << 19);
				res |= (regB << 16);

				break;

			case 0b110:  // halt
			case 0b111:  // noop
				break;

			case 0b1000: // .fill
				if (isNumber(arg0)) { //
					sscanf(arg0, "%d", &res); 
				}
				else {
					res = findBranchAddress(arg0);
				}
				break;

			default:
				printf("unrecognized opcode\n");
				exit(1);
		}

		fprintf(outFilePtr, "%d\n", res);

		line++;
	}
	
	freeBranchList(head);
	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int getBinaryOpcode(char *opcode) {
	if (!strcmp(opcode, "add")) return 0b000;
	if (!strcmp(opcode, "nor")) return 0b001;
	if (!strcmp(opcode, "lw")) return 0b010;
	if (!strcmp(opcode, "sw")) return 0b011;
	if (!strcmp(opcode, "beq")) return 0b100;
	if (!strcmp(opcode, "jalr")) return 0b101;
	if (!strcmp(opcode, "halt")) return 0b110;
	if (!strcmp(opcode, "noop")) return 0b111;

	if (!strcmp(opcode, ".fill")) return 0b1000;

	printf("unrecognized opcode\n");
	exit(1);
}

int isValidRegister(char *arg) {
	if(!isNumber(arg)) {
		printf("non-integer register argument");
		return 0;
	}
	int regNum;
	sscanf(arg, "%d", &regNum);
	
	if (regNum < 0 || regNum >= 8) {
		printf("register outside the range");
		return 0;
	}

	return 1;
}

int getWordOffset(char *offsetField) {
	int res;

	if (isNumber(offsetField)) {
		// get numeric address value
		sscanf(offsetField, "%d", &res);
		if (res < -32768 || res > 32767) {
			printf("offsetField don't fit in 16bit\n");
			exit(1);
		}
	}
	else {
		res = findBranchAddress(offsetField);
	}

	res &= (0xffff);
	return res;
}

int getBranchOffset(char *offsetField, int line) {
	int res;

	if (isNumber(offsetField)) {
		// get numeric address value
		sscanf(offsetField, "%d", &res);
		if (res < -32768 || res > 32767) {
			printf("offsetField don't fit in 16bit\n");
			exit(1);
		}
	}
	else {
		res = findBranchAddress(offsetField) - (line + 1);
	}

	res &= (0xffff);
	return res;
}

int findBranchAddress(char *branchName) {
	struct branch *cur = head->next;

	while (1) {
		if (!strcmp(cur->name, branchName)) {
			return cur->address;
		}

		if (cur->next == NULL) break;
		cur = cur->next;
	}

	printf("undefined label\n");
	exit(1);
}

void freeBranchList(struct branch *cur) {
	struct branch *prev;
	while (cur->next != NULL) {
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	free(cur);
}