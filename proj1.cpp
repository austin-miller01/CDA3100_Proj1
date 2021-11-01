/* instruction-level simulator for LC3100 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

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
std::string checkOpCode(std::string);
int checkReg(std::string);
//convert 2's complement to decimal
int twosComplement(std::string);

//convert decimal to binary
std::string decToBin(int dec) {
	std::string bin = "";
	while (dec > 0) {
		bin = (dec % 2 == 0 ? "0" : "1") + bin;
		dec /= 2;
	}
	if(bin.length() < 32){
		while(bin.length() < 32){
			bin = "0" + bin;
		}

	}
	return bin;
}

int
main(int argc, char *argv[])
{
	std::vector<std::string> binary_nums;


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
		printf("memory[%d]=%d\n", state.numMemory, 
		state.mem[state.numMemory]);
	}
	//Below this line is all the stuff about opcodes and registers, adding the lines to a vector
	for(int i = 0; i < state.numMemory; i++){
		binary_nums.push_back(decToBin(state.mem[i]));
		//std::cout << binary_nums[i] << std::endl;
	}
	for(int i = 0; i < NUMREGS; i++){
		state.reg[i] = 0;
	}
	int reg1 = 0, reg2 = 0, rdest = 0, offset = 0;
	std::string currOpCode = "";
	int i = 0;
	for(int i = 0; i < state.numMemory; i++){
		label:
		currOpCode = checkOpCode(binary_nums[i]);
		//std::cout << "Current Opcode: " << currOpCode << std::endl;
		printState(&state);
		if(currOpCode == "NOOP"){
			state.pc++;
			continue;
		}
		else if(currOpCode == "ADD" || currOpCode == "NAND" ){
			reg1 = checkReg(binary_nums[i].substr(31-21,3));
			reg2 = checkReg(binary_nums[i].substr(31-18,3));
			rdest = checkReg(binary_nums[i].substr(31-2,3));
			if(currOpCode == "ADD"){
				state.reg[rdest] = state.reg[reg1] + state.reg[reg2];
			}
			else if(currOpCode == "NAND"){
				state.reg[rdest] = ~(state.reg[reg1] & state.reg[reg2]);
			}
			state.pc++;
		}
		else if(currOpCode == "LW" || currOpCode == "SW" || currOpCode == "BEQ"){
			offset = twosComplement(binary_nums[i].substr(31-15,16));
			reg1 = checkReg(binary_nums[i].substr(31-21,3));
			reg2 = checkReg(binary_nums[i].substr(31-18,3));
			if(currOpCode == "BEQ"){
				if(state.reg[reg1] == state.reg[reg2]){
					i = state.pc + 1 + offset;
					//std::cout << "offset == " << offset << std::endl;
					state.pc = i;
					//std::cout << "I == "<<  i << std::endl;
					goto label;
				}
			}
			else if(currOpCode == "SW"){

				state.mem[state.reg[reg1] + offset] = state.reg[reg2];
				state.pc++;
			}
			else if(currOpCode == "LW"){
				
				state.reg[reg2] = state.mem[state.reg[reg1] + offset];
				state.pc++;
			}
			
		}
		else if(currOpCode == "HALT")break;
		

	}
	
	return(0);
}
void
printState(stateType *statePtr)
{
	int i;

	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");

	for (i=0; i<statePtr->numMemory; i++) {
    		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
	printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
    	printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
	printf("end state\n");
}

std::string checkOpCode(std::string binary){
	std::string opcode = binary.substr(31-24,3);
	if (opcode == "000")return "ADD";
	else if(opcode == "001")return "NAND";
	else if(opcode == "010")return "LW";
	else if(opcode == "011")return "SW";
	else if(opcode == "100")return "BEQ";
	else if(opcode == "110")return "HALT";
	else if(opcode == "111")return "NOOP";
	return "INVALID";

}

int checkReg(std::string input){
	//convert binary to decimal
	int dec = 0;
	for(int i = 0; i < input.length(); i++){
		dec += (input[i] - '0') * pow(2, input.length() - i - 1);
	}
	return dec;
}

int twosComplement(std::string input){
	int dec = 0;
	for(int i = 0; i < input.length(); i++){
		dec += (input[i] - '0') * pow(2, input.length() - i - 1);
	}
	if(dec > 32767){
		dec = dec - 65536;
	}
	return dec;
}
