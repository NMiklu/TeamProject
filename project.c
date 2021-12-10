/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
void decoder3(BIT* I, BIT EN, BIT* O); // added for use in decoder5
void decoder5(BIT* I, BIT* O); // added for read_register and instruction_memory
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A){
	if (A) return FALSE;
	else return TRUE;
}

BIT or_gate(BIT A, BIT B){
	if (A || B) return TRUE;
	else return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C){
	return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B){
	if (A && B) return TRUE;
	else return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C){
	return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B){
	if (A ^ B) return TRUE;
	else return FALSE;
}

BIT nor_gate(BIT A, BIT B){
	return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B){
	return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3){
	// Note: The input -> output mapping is slightly modified from before
	*O0 = and_gate(not_gate(I1), not_gate(I0));
	*O1 = and_gate(not_gate(I1), I0);
	*O2 = and_gate(I1, not_gate(I0));
	*O3 = and_gate(I1, I0);
	return;
}

void decoder3(BIT* I, BIT EN, BIT* O) { // added to build decoder5
	O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
	O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
	O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
	O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
	O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
	O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
	O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
	O[7] = and_gate3(I[2], I[1], I[0]);
	
	O[0] = and_gate(EN, O[0]);
	O[1] = and_gate(EN, O[1]);
	O[2] = and_gate(EN, O[2]);
	O[3] = and_gate(EN, O[3]);
	O[4] = and_gate(EN, O[4]);
	O[5] = and_gate(EN, O[5]);
	O[6] = and_gate(EN, O[6]);
	O[7] = and_gate(EN, O[7]);
	
	return;
}

void decoder5(BIT* I, BIT* O) {  // added for read_register and instruction_memory
	BIT EN[4] = {FALSE};
	decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
	decoder3(I, EN[3], &O[24]);
	decoder3(I, EN[2], &O[16]);
	decoder3(I, EN[1], &O[8]);
	decoder3(I, EN[0], &O[0]);
}

BIT multiplexor2(BIT S, BIT I0, BIT I1){
	BIT x0 = and_gate(not_gate(S), I0);
	BIT x1 = and_gate(S, I1);
	return or_gate(x0, x1);  
}

void multiplexor2_5(BIT S, BIT* I0, BIT* I1, BIT* Output){
	for (int i = 0; i < 5; ++i) {
		BIT x0 = and_gate(not_gate(S), I0[i]);
		BIT x1 = and_gate(S, I1[i]);
		Output[i] = or_gate(x0, x1);
	} 
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output){
	for (int i = 0; i < 32; ++i) {
		BIT x0 = and_gate(not_gate(S), I0[i]);
		BIT x1 = and_gate(S, I1[i]);
		Output[i] = or_gate(x0, x1);
	}
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3){
	BIT x0, x1, x2, x3 = FALSE;
	decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
	BIT y0 = and_gate(x0, I0);
	BIT y1 = and_gate(x1, I1);
	BIT y2 = and_gate(x2, I2);
	BIT y3 = and_gate(x3, I3);
  
	BIT z0 = or_gate(y0, y1);
	BIT z1 = or_gate(y2, y3);
	return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B){
	for(int i = 0; i < 32; ++i) B[i] = A[i]; 
}

void print_binary(BIT* A){
	for (int i = 31; i >= 0; --i) printf("%d", A[i]); 
}

void convert_to_binary_char(int a, char* A, int length){
	if (a >= 0) {
		for (int i = 0; i < length; ++i) {
			A[i] = (a % 2 == 1 ? '1' : '0');
			a /= 2;
		}
	} else {
		a += 1;
		for (int i = 0; i < length; ++i) {
			A[i] = (a % 2 == 0 ? '1' : '0');
			a /= 2;
		}
	}
}

void convert_to_binary(int a, BIT* A, int length){
	convert_to_binary_char(a, A, 32);
}
  
int binary_to_integer(BIT* A){
	unsigned a = 0;
	unsigned mult = 1;
	for (int i = 0; i < 32; ++i) {
		a += A[i]*mult;
		mult *= 2;
  	}
	return (int)a;
}


/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

void set_register(char* input, char* output){
	if (strcmp(input, "t0") == 0) strncpy(output, "00010", 5);
	else if (strcmp(input, "s0") == 0) strncpy(output, "00001", 5);
  	else if (strcmp(input, "t1") == 0) strncpy(output, "10010", 5);
	else if (strcmp(input, "s1") == 0) strncpy(output, "10001", 5);
  	else if (strcmp(input, "zero") == 0) strncpy(output, "00000", 5);
  	else if (strcmp(input, "v0") == 0) strncpy(output, "01000", 5);
  	else if (strcmp(input, "a0") == 0) strncpy(output, "00100", 5);
  	else if (strcmp(input, "sp") == 0) strncpy(output, "10111", 5);
  	else if (strcmp(input, "ra") == 0) strncpy(output, "11111", 5);
}
void set_opcode(char* input, char* opcode ,char* funct) {
	// I-type
	if (strcmp(input, "lw") == 0) strncpy(opcode, "110001", 6);
  	else if (strcmp(input, "sw") == 0) strncpy(opcode, "110101", 6);
  	else if (strcmp(input, "beq") == 0) strncpy(opcode, "001000", 6);
  	else if (strcmp(input, "addi") == 0) strncpy(opcode, "000100", 6);
  	// R-type
	else if (strcmp(input, "and") == 0) {
		strncpy(opcode, "000000", 6);
		strncpy(funct, "001001", 6);
  	}
  	else if (strcmp(input, "or") == 0) {
    	strncpy(opcode, "000000", 6);
    	strncpy(funct, "101001", 6);
  	}
  	else if (strcmp(input, "add") == 0) {
    	strncpy(opcode, "000000", 6);
    	strncpy(funct, "000001", 6);
  	}
  	else if (strcmp(input, "sub") == 0) {
    	strncpy(opcode, "000000", 6);
    	strncpy(funct, "010001", 6);
  	}
  	else if (strcmp(input, "slt") == 0) {
    	strncpy(opcode, "000000", 6);
    	strncpy(funct, "010101", 6);
  	}
  	else if (strcmp(input, "jr") == 0) {
    	strncpy(opcode, "000000", 6);
    	strncpy(funct, "000100", 6);
  	}
  	// J-type
  	else if (strcmp(input, "j") == 0) strncpy(opcode, "010000", 6);
  	else if (strcmp(input, "jal") == 0) strncpy(opcode, "110000", 6);
}

int get_instructions(BIT Instructions[][32]){
	char line[256] = {0};
	int instruction_count = 0;
	while (fgets(line, 256, stdin) != NULL) {
  		// TODO: perform conversion of instructions to binary
    	// Input: coming from stdin via: ./a.out < input.txt
    	// Output: Convert instructions to binary in the instruction memory
    	// Return: Total number of instructions
    	// Note: you are free to use if-else and external libraries here
    	// Note: you don't need to implement circuits for saving to inst. mem.
    	// My approach:
    	// - Use sscanf on line to get strings for instruction and registers
    	// - Use instructions to determine op code, funct, and shamt fields
    	// - Convert immediate field and jump address field to binary
    	// - Use registers to get rt, rd, rs fields
    	// Note: I parse everything as strings, then convert to BIT array at end
    	char inst[256] = {0};
    	char op1[256] = {0};
    	char op2[256] = {0};
    	char op3[256] = {0};
    	sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
    
    	char temp_output[33] = {0};
    	char rs[6] = {0};
    	char rt[6] = {0};
    	char rd[6] = {0};
    	char imm[17] = {0};
    	char address[27] = {0};
    	char opcode[7] = {0};
    	char funct[7] = {0};
    	// I-type
    	if (strcmp(inst, "lw") == 0 || strcmp(inst, "sw") == 0 || strcmp(inst, "beq") == 0 || strcmp(inst, "addi") == 0) {
    		convert_to_binary_char(atoi(op3), imm, 16);
      		set_register(op1, rt);
      		set_register(op2, rs);
      		set_opcode(inst, opcode, funct);
      		strncpy(&temp_output[0], imm, 16);
      		strncpy(&temp_output[16], rt, 5);
      		strncpy(&temp_output[21], rs, 5);
      		strncpy(&temp_output[26], opcode, 6);     
    	// R-type 
    	} else if (strcmp(inst, "jr") == 0 || strcmp(inst, "add") == 0 || strcmp(inst, "and") == 0 || strcmp(inst, "or") == 0 || strcmp(inst, "sub") == 0 || strcmp(inst, "slt") == 0) {
      		set_register(op1, rd);
      		set_register(op2, rs);
      		set_register(op3, rt);
      		set_opcode(inst, opcode, funct);
      		strncpy(&temp_output[0], funct, 6);
      		strncpy(&temp_output[6], "00000", 5);
      		strncpy(&temp_output[11], rd, 5);
      		strncpy(&temp_output[16], rt, 5);
      		strncpy(&temp_output[21], rs, 5);
      		strncpy(&temp_output[26], opcode, 6);      
    	// J-type
    	} else if (strcmp(inst, "j") == 0 || strcmp(inst, "jal") == 0) {
      		convert_to_binary_char(atoi(op1), address, 26);
      		set_opcode(inst, opcode, funct);
      		strncpy(&temp_output[0], address, 26);
      		strncpy(&temp_output[26], opcode, 6);      
    	}
    
    	for (int i = 0; i < 32; ++i) Instructions[instruction_count][i] = (temp_output[i] == '1' ? TRUE : FALSE); 
    	instruction_count++;
  	}
  	return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction(){
	unsigned pc = binary_to_integer(PC);
	printf("PC: %d\n", pc);
	printf("Instruction: ");
	print_binary(MEM_Instruction[pc]);
	printf("\n");
}

void print_state(){
	printf("Data: ");
	for (int i = 0; i < 32; ++i) {
		printf("%d ", binary_to_integer(MEM_Data[i]));
	} 
	printf("\n");  
	printf("Register: ");
	for (int i = 0; i < 32; ++i) {
		printf("%d ", binary_to_integer(MEM_Register[i]));
	} 
	printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum){
	BIT x0 = xor_gate(A, B);
	*Sum = xor_gate(CarryIn, x0);

	BIT y0 = and_gate(x0, CarryIn);
	BIT y1 = and_gate(A, B);
	*CarryOut = or_gate(y0, y1);
}

void adder_32(BIT* a, BIT* b, BIT* sum) {
	BIT carry = FALSE;
	for (int i = 0; i < 32; i++) {
		adder1(a[i], b[i], carry, &carry, &(sum[i]));
	}
}

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction){ // isaac
	// TODO: Implement instruction memory
	// Input: 32-bit instruction address
	// Output: 32-bit binary instruction
	// Note: Useful to use a 5-to-32 decoder here

	BIT instruction_out[32] = {FALSE};
	decoder5(ReadAddress, instruction_out);
	for (int i = 0; i < 32; ++i) {
		multiplexor2_32(instruction_out[i], Instruction, MEM_Instruction[i], Instruction);
	}
}

void Control(BIT* OpCode, BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg, BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite){ // isaac
	// TODO: Set control bits for everything
	// Input: opcode field from the instruction
	// OUtput: all control lines get set 
	// Note: Can use SOP or similar approaches to determine bits

	// assigning all the bit values to each instruction
	BIT lw = and_gate(and_gate3((OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])), and_gate3(not_gate(OpCode[2]), (OpCode[1]), (OpCode[0])));
	BIT sw = and_gate(and_gate3((OpCode[5]), not_gate(OpCode[4]), (OpCode[3])), and_gate3(not_gate(OpCode[2]), (OpCode[1]), (OpCode[0])));
	BIT beq = and_gate(and_gate3((OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])), and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])));
	BIT addi = and_gate(and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), (OpCode[3])), and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])));
	BIT and = and_gate(and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])), and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])));
	BIT or = and_gate(and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])), and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])));
	BIT add = and_gate(and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])), and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])));
	BIT sub = and_gate(and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])), and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])));
	BIT j = and_gate(and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])), and_gate3(not_gate(OpCode[2]), (OpCode[1]), not_gate(OpCode[0])));
	BIT jal = and_gate(and_gate3(not_gate(OpCode[2]), (OpCode[1]), (OpCode[0])), and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])));
	BIT jr = and_gate(and_gate3(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3])), and_gate3(not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])));

	// assigning all the bit values to each instruction type
	BIT r_type = or_gate(or_gate(or_gate(add, sub), or_gate(and, or)), jr);
	BIT i_type = or_gate(or_gate(beq, addi), or_gate(lw, sw));
	BIT j_type = or_gate(j, jal);

	// assigning control bit values
	*RegDst = r_type;
	*Jump = j_type;
	*Branch = beq;
	*MemRead = lw;
	*MemToReg = lw;
	*MemWrite = sw;
	*ALUSrc = i_type;
	*RegWrite = and_gate(not_gate(sw), not_gate(beq));
	
	// assigning the operation bit values
	ALUOp[1] = r_type;
	ALUOp[0] = beq;
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2, BIT* ReadData1, BIT* ReadData2) { // isaac
	// TODO: Implement register read functionality
	// Input: two 5-bit register addresses
	// Output: the values of the specified registers in ReadData1 and ReadData2
	// Note: Implementation will be very similar to instruction memory circuit

	BIT bit1[32] = {FALSE};
	BIT bit2[32] = {FALSE};
	decoder5(ReadRegister1, bit1);
	decoder5(ReadRegister2, bit2);
	for(int i = 0; i < 32; ++i){
		multiplexor2_32(bit1[i], ReadData1, MEM_Register[i], ReadData1);
		multiplexor2_32(bit2[i], ReadData2, MEM_Register[i], ReadData2);
	}
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData){  // niko
	// TODO: Implement register write functionality
	// Input: one 5-bit register address, data to write, and control bit
	// Output: None, but will modify register file
	// Note: Implementation will again be similar to those above

	BIT t[32] = {FALSE};
	decoder5(WriteRegister, t);
	for(int j = 0; j < 32; j++){
		multiplexor2_32(and_gate(RegWrite,t[j]), MEM_Register[j],WriteData,MEM_Register[j]);
	}
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl){  // curtis
	// TODO: Implement ALU Control circuit
	// Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
	//        binary instruction
	BIT a_BIT = and_gate(and_gate3(not_gate(funct[0]), not_gate(funct[1]), not_gate(funct[2])), and_gate3(not_gate(funct[3]), not_gate(funct[4]), funct[5]));
	BIT s_BIT = and_gate(and_gate3(not_gate(funct[0]), funct[1], not_gate(funct[2])), and_gate3(not_gate(funct[3]), not_gate(funct[4]), funct[5]));
	BIT o_BIT = and_gate(and_gate3(funct[0], not_gate(funct[1]), funct[2]), and_gate3(not_gate(funct[3]), not_gate(funct[4]), funct[5]));
  	BIT sll_BIT = and_gate(and_gate3(not_gate(funct[0]), funct[1], not_gate(funct[2])), and_gate3(funct[3], not_gate(funct[4]), funct[5]));

	ALUControl[0] = or_gate(sll_BIT, o_BIT);
	ALUControl[1] = or_gate(or_gate3(a_BIT, s_BIT, sll_BIT), not_gate(ALUOp[1]));
	ALUControl[2] = or_gate3(sll_BIT, sll_BIT, ALUOp[0]);
	ALUControl[3] = 0;

	// Output:4-bit ALUControl for input into the ALU
	// Note: Can use SOP or similar approaches to determine bits
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set){
	BIT x0 = multiplexor2(Binvert, B, not_gate(B));

	BIT y0 = and_gate(A, x0);
	BIT y1 = or_gate(A, x0);
	BIT y2 = FALSE;
	adder1(A, x0, CarryIn, CarryOut, &y2);  
	*Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
	*Set = y2;
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result){  // curtis   
	// TODO: Implement 32-bit ALU
	// Input: 4-bit ALUControl, two 32-bit input
	*Zero = FALSE;
	BIT CarryIn = ALUControl[3];
	BIT Binvert = ALUControl[2];
	BIT Op1 = ALUControl[1];
	BIT Op0 = ALUControl[0];

	BIT Set = FALSE;
	BIT EMPTY = FALSE;

	BIT C = FALSE;
	BIT* CarryOut = &C;

	*CarryOut = CarryIn;
	/*print_binary(Input1);
	printf(" <- Input 1\n");
	print_binary(Input2);
	printf(" <- Input 2\n");*/
	for(int i = 0; i < 32; i++){
		ALU1(Input1[i], Input2[i], Binvert, *CarryOut, FALSE, Op0, Op1, &(Result[i]), CarryOut, &Set);
	}
	/*print_binary(Result);
	printf(" <- Result\n");*/
	/*
	ALU1(Input1[0], Input2[0], not_gate(Binvert), not_gate(CarryIn), Set, Op0, Op1, &EMPTY, CarryOut, &Set);

	Result[0] = multiplexor2(and_gate(Op0, Op1), Result[0], Set);
	*/
	// Output: 32-bit result, and zero flag big
	// Note: Can re-use prior implementations (but need new circuitry for zero)
}

void Data_Memory(BIT MemWrite, BIT MemRead, BIT* Address, BIT* WriteData, BIT* ReadData){  // niko
	// TODO: Implement data memory
	// Input: 32-bit address, control flags for read/write, and data to write
	// Output: data read if processing a lw instruction
	// Note: Implementation similar as above

	BIT in[5] = {Address[27],Address[28],Address[29],Address[30],Address[31]};
	BIT t[32] = {FALSE};
	decoder5(in, t);
	for(int i = 0; i < 32; i++){
		multiplexor2_32(and_gate(t[i], MemWrite), MEM_Data[i], WriteData, MEM_Data[i]);
		multiplexor2_32(and_gate(t[i], MemRead), ReadData, MEM_Data[i], ReadData);
	}
}

void Extend_Sign16(BIT* Input, BIT* Output){
	// TODO: Implement 16-bit to 32-bit sign extender
	// Copy Input to Output 
	for (int i = 0; i < 16; i++) {
		Output[i] = Input[i];
	}
	// extend 16th Input bit to 17-32 bits in Output
	for (int i = 16; i < 32; i++) {
		Output[i] = Input[15];
	}
}

void updateState(){
	// TODO: Implement the full datapath here
	// Essentially, you'll be figuring out the order in which to process each of 
	// the sub-circuits comprising the entire processor circuit. It makes it 
	// easier to consider the pipelined version of the process, and handle things
	// in order of the pipeline. The stages and operations are:

	// Fetch - load instruction from instruction memory
	BIT inst[32] = {FALSE};
	Instruction_Memory(PC, inst);

	// Decode - set control bits and read from the register file
	Control(&(inst[26]), &RegDst, &Jump, &Branch, &MemRead, &MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite);
	BIT ReadData1[32] = {FALSE};
	BIT ReadData2[32] = {FALSE};
	Read_Register(&(inst[21]), &(inst[16]), ReadData1, ReadData2);
	BIT imm[32] = {FALSE};
	Extend_Sign16(inst, imm);

	// Execute - process ALU
	ALU_Control(ALUOp, inst, ALUControl);
	BIT ALUResult[32] = {FALSE};
	BIT ALUIn2[32] = {FALSE}; 
	multiplexor2_32(ALUSrc, ReadData2, imm, ALUIn2);
	ALU(ALUControl, ReadData1, ALUIn2, &Zero, ALUResult);

	// Memory - read/write data memory
	BIT ReadData[32] = {FALSE};
	Data_Memory(MemWrite, MemRead, ALUResult, ReadData2, ReadData);

	// Write Back - write to the register file
	BIT WriteReg[5] = {FALSE};
	multiplexor2_5(RegDst, &(inst[16]), &(inst[11]), WriteReg);
	BIT WriteData[32] = {FALSE};
	multiplexor2_32(MemToReg, ALUResult, ReadData, WriteData);
	Write_Register(RegWrite, WriteReg, WriteData);

	// Update PC - determine the final PC value for the next instruction
	BIT PCAdd1[32] = {FALSE};
	adder_32(PC, ONE, PCAdd1);
	BIT PCAddImm[32] = {FALSE};
	adder_32(PCAdd1, imm, PCAddImm);
	BIT PCChoice1[32] = {FALSE};
	multiplexor2_32(and_gate(Branch, Zero), PCAdd1, PCAddImm, PCChoice1);
	BIT JumpAddress[32] = {FALSE};
	for (int i = 0; i < 26; i++) {
		JumpAddress[i] = inst[i];
	}
	// sign extend our jump address
	for (int i = 26; i < 32; i++) {
		JumpAddress[i] = JumpAddress[25];
	}
	// update our PC
	multiplexor2_32(Jump, PCChoice1, JumpAddress, PC);
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main(){
	setbuf(stdout, NULL);
	// parse instructions into binary format
	int counter = get_instructions(MEM_Instruction);
	// load program and run
	copy_bits(ZERO, PC);
	copy_bits(THIRTY_TWO, MEM_Register[29]);

	while (binary_to_integer(PC) < counter) {
		print_instruction();
		updateState();
		print_state();
	}
	return 0;
}

