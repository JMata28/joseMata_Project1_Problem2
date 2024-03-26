//This program is Jose Mata's submission for COMP590 - Computer Architecture Project 1, Problem 2
//This program uses and modifies some code from the "emulate590.c" program written in class and expands on it

#include <iostream>
using namespace std;

//storage units
unsigned char PC;
unsigned char memory[256];
unsigned char registers[4];

int main() {
    //Set PC to 0
    PC =0;

    // The following lines (25-29) should be implemented for the test program that multiplies 2*3. Else, they should be commented out.
    //Use a for loop to copy the test program into memory array and write memory[0xfe] = 2;
    unsigned char program[21] = {0xff,0xfe,0x23,0x15,0xc8,0xcb,0xcd,0xcd,0x49,0x5f,0x74,0xcc,0xcd,0x1b,0xcc,0xde,0x5f,0xff,0xff,0x37,0x01};
    for(int i = 0; i < 21; i++){
        memory[i] = program[i];
    }
    memory[254] = 2;

//  //The following lines (33-37) should be implemented for the fibonacci program. Else, they should be commented out.
//  //Use a for loop to copy the fibonacci program into memory array. The instruction to make memory[0xfe] = 6 is already included in the fibonacci program so there is no need to implement that in this C++ compiler;
//    unsigned char fibonacci_program[40] ={0xC4, 0xC4, 0xC8, 0xC9, 0xCC, 0xDE, 0xF3, 0xF2, 0x3C , 0xF3, 0xF2, 0x2C, 0xC2,0xD3, 0x4D, 0x58, 0x76, 0xFF, 0xFF, 0x37, 0xFF, 0xFD, 0x3B, 0x27, 0xFF, 0xFF, 0x2B, 0xF3, 0xF2, 0x2C, 0xC0, 0xC1, 0x1C, 0xF3, 0xF2, 0x3C, 0xCC, 0xED, 0x53, 0x01 };
//    for(int i = 0; i <40; i++){
//        memory[i]=fibonacci_program[i];
//    }
    int halt = 0;
    int x = -1; //This is the line counter. It starts at -1 so that the line number matches the index of the instruction in the memory array
    //Repeat steps 1-4 200 times.
    while (halt == 0) {
        x++;
        //Step 1: Fetch instruction from memory
        unsigned char instruction = memory[PC];
        //unsigned char instruction = 0b00100001;
        PC++;
        //get the high four bits
        unsigned char opcode = instruction >> 4;
        //to find the value of register rd, shift then mask the two right-most bits of the instruction by doing a logical AND with 0b11
        unsigned char rd = (instruction >> 2) & 0b11;
        //to find the value of register rs, mask the two right-most bits of the instruction by doing a logical AND with 0b11
        unsigned char rs = instruction & 0b11;

        //Step 2: Read the registers (and save the imm)
        //Find which register rd is
        unsigned char rdvalue = 0;
        if (rd == 0) rdvalue = registers[0];
        if (rd == 1) rdvalue = registers[1];
        if (rd == 2) rdvalue = registers[2];
        if (rd == 3) rdvalue = registers[3];
        //Find which register rs is
        unsigned char rsvalue = 0;
        if (rs == 0) rsvalue = registers[0];
        if (rs == 1) rsvalue = registers[1];
        if (rs == 2) rsvalue = registers[2];
        if (rs == 3) rsvalue = registers[3];

        //Find the opcode needed for SLI instruction
        unsigned char opcode3 =
                instruction >> 6; //Shift six bits to the right so that the only bits remaining are the two left-most bits

        string iname("none"); //This string will be used to store the name of the instruction (NEEDS TO BE UPDATED FOR EACH INSTRUCTION)
        /*
        Step 3: Execute the instruction. This requires if/else statements for each instruction.
        Each instruction will have their own version of Step 4.
        */
        //ADD
        if (opcode == 0b0111) {
            iname = "ADD"; //store in iname the name of the instruction
            unsigned char sum = rdvalue + rsvalue;
            //Step 4: save result in register rd
            if (rd == 0) registers[0] = sum;
            if (rd == 1) registers[1] = sum;
            if (rd == 2) registers[2] = sum;
            if (rd == 3) registers[3] = sum;
        }
            //SUB
        else if (opcode == 0b0001) {
            iname = "SUB";
            unsigned char subtraction = rdvalue - rsvalue;
            //Step 4: save result in register rd
            if (rd == 0) registers[0] = subtraction;
            if (rd == 1) registers[1] = subtraction;
            if (rd == 2) registers[2] = subtraction;
            if (rd == 3) registers[3] = subtraction;
        }
            //LOAD
        else if (opcode == 0b0010) {
            iname = "LOAD";
            rdvalue = memory[rsvalue]; //save in rdvalue the content of memory as address rsvalue
            //Step 4: save content in register rd
            if (rd == 0) registers[0] = rdvalue;
            if (rd == 1) registers[1] = rdvalue;
            if (rd == 2) registers[2] = rdvalue;
            if (rd == 3) registers[3] = rdvalue;
        }
            //STORE
        else if (opcode == 0b0011) {
            iname = "STORE";
            //Step4: Store rdvalue in the memory location indicated by the value saved in rsvalue
            memory[rsvalue] = rdvalue;
        }
            //SKIPNZ and SKIPZ (they have the same four left-most bits opcode but the two right-most defer)
        else if (opcode == 0b0100) {
            unsigned char opcode2 = instruction & 0b11; //mask to get only the two left-most bits of the instruction
            if (opcode2 == 0) { //SKIPZ
                iname = "SKIPZ";
                if (rdvalue == 0) {
                    PC++; //We increase PC by one so that the next instruction in the memory array is skipped
                }
            } else if (opcode2 == 1) {//SKIPNZ
                iname = "SKIPNZ";
                if (rdvalue != 0) {
                    PC++; //We increase PC by one so that the next instruction in the memory array is skipped
                }
            }
        }
            //JALR save PC+1 into RD, jump to RS
        else if (opcode == 0b0101) {
            iname = "JALR";
            unsigned char address = rsvalue; //Use a temporary variable to save the address that is stored in rsvalue
            rdvalue = PC;  //Rvalue is PC, because at the top of this while loop I've already increased 1 to PC
            if (rd == 0) registers[0] = rdvalue;
            if (rd == 1) registers[1] = rdvalue;
            if (rd == 2) registers[2] = rdvalue;
            if (rd == 3) registers[3] = rdvalue;
            PC = address;
        }
            //SLI
        else if (opcode3 == 3) {
            iname = "SLI";
            unsigned char topimm = (instruction >> 4) & 0b11; //Get IMM3 and IMM2
            unsigned char bottomimm = instruction & 0b11; //Get IMM1 and IMM0
            unsigned char imm = (topimm << 2) |
                                bottomimm; // Shifting the top two bits twice to the right and doing a logical OR results in Imm
            rd = (instruction >> 2) & 0b11; //Shift to the right two bits and mask to obtain RD1 and RD0
            //shift the bottom four bits of RD into the four upper bits, and doing a logical OR with imm is equivalent to loading the 4-bit Imm into the lower four bits of RD
            if (rd == 0) registers[0] = (rdvalue << 4) | imm;
            if (rd == 1) registers[1] = (rdvalue << 4) | imm;
            if (rd == 2) registers[2] = (rdvalue << 4) | imm;
            if (rd == 3) registers[3] = (rdvalue << 4) | imm;
        }
            //HALT
        else if (instruction == 1) {
            halt = 1;
            cout << "The program has been halted." << endl;
            break;
        }

        cout << "Instruction:" << iname << endl;
        cout << x << " "; //x is the line number
        printf("PC=%x, inst=%x, A=%x, B=%x, C=%x, D=%x\n\n", PC, instruction, registers[0], registers[1], registers[2],
               registers[3]);
    }
    printf("The final answer is: %d", memory[255]);
    return 0;
}
