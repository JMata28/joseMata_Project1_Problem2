//This program is Jose Mata's submission for COMP590 - Computer Architecture Project 1, Problem 2
//This program uses and modifies some code from the "emulate590.c" program written in class and expands on it

#include <iostream>
using namespace std;

//storage units
unsigned char PC;
unsigned char memory[256];
unsigned char registers[4];

int main() {
    //Set registers A(0), B(1), C(2), and D(3) to zero.
    registers[0] = 0;
    registers[1] = 0;
    registers[2] = 0;
    registers[3] = 0;

    //Set PC to 0
    PC =0;

    //Use a for loop to copy the test program into memory array
    unsigned char program[21] = {0xff,0xfe,0x23,0x15,0xc8,0xcb,0xcd,0xcd,0x49,0x5f,0x74,0xcc,0xcd,0x1b,0xcc,0xde,0x5f,0xff,0xff,0x37,0x01};
    for(int i = 0; i < 21; i++){
        memory[i] = program[i];
    }
    //write memory[0xfe] = 2;
    memory[254] = 2;

    //Repeat steps 1-4 44 times.
    for(int x = 0; x <50; x++) {
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
            //save result in memory 0xff
            memory[255] = sum;
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
            //save result in memory 0xff
            memory[255] = subtraction;
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
            //save result in memory 0xff
            memory[255] = rdvalue;
        }
            //STORE
        else if (opcode == 0b0011) {
            iname = "STORE";
            //Step4: Store rdvalue in the memory location indicated by the value saved in rsvalue
            memory[rsvalue] = rdvalue;
            //save result in memory 0xff. NOTE: this line below is probably unnecessary since the value of rd was not changed
            memory[255] = rdvalue;
        }
            //SKIPNZ and SKIPZ (they have the same four left-most bits opcode but the two right-most defer)
        else if (opcode == 0b0100) {
            unsigned char opcode2 = instruction & 0b11; //mask to get only the two left-most bits of the instruction
            if (opcode2 == 0) { //SKIPZ
                iname = "SKIPZ";
                if (rdvalue == 0) {
                    PC++; //We increase PC by one so that the next instruction in the memory array is skipped
                    //save result in memory 0xff. NOTE: this line below is probably unnecessary since the value of rd was not changed
                    memory[255] = rdvalue;
                }
            } else if (opcode2 == 1) {//SKIPNZ
                iname = "SKIPNZ";
                if (rdvalue != 0) {
                    PC++; //We increase PC by one so that the next instruction in the memory array is skipped
                    //save result in memory 0xff. NOTE: this line below is probably unnecessary since the value of rd was not changed
                    memory[255] = rdvalue;
                }
            }
        }
            //JALR save PC+1 into RD, jump to RS
        else if (opcode == 0b0101) {
            iname = "JALR";
            unsigned char address = rsvalue; //Use a temporary variable to save the address that is stored in rsvalue
            rdvalue = PC;  //QUESTION: I'm confused as to why rdvalue is not PC + 1, but this was the only way I got yhe desired output
            if (rd == 0) registers[0] = rdvalue;
            if (rd == 1) registers[1] = rdvalue;
            if (rd == 2) registers[2] = rdvalue;
            if (rd == 3) registers[3] = rdvalue;
            PC = address;
            //save result in memory 0xff.
            memory[255] = rdvalue;
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
            //save result in memory 0xff.
            memory[255] = (rdvalue << 4) | imm;
        }
            //HALT
        else if (instruction == 1) {
            cout << "The program has been halted." << endl;
        }

        cout << "Instruction:" << iname << endl;
        cout << x << " "; //x is the line number
        printf("PC=%x, inst=%x, A=%x, B=%x, C=%x, D=%x\n\n", PC, instruction, registers[0], registers[1], registers[2],
               registers[3]);
    }
    printf("The final answer is: %d", memory[255]);
    return 0;
}
