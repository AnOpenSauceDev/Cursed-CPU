#include <stdio.h>
#include <curses.h>
#include "processor_defs.h"
#include <string.h>

#define FALSE  0
#define TRUE  1

//Quirkix V1, first edition, the whole DUMMY thing is... dummy, so i'm going to write V2 later and make it suck less.
// i hope to write a basic semi unix-like OS in the far, far future.

// the WHOLE byte is now just an instruction. My old cpu concept was novel, but also way too overcomplicated. 255 instructions? Heck yeah!

// mistake i made. 1024 means that there are 1024 elements including 0, not 1024 plus 0
typedef struct Program{
    unsigned char rom[1024]; // 1024b of program memory
} Program;

uint16_t address = 0; // can address 65535 bytes (65k of ram) this unsigned 16-bit int makes us... 16 bit!

unsigned char memory[4096]; // 4K of base ram

// instead of having "Add Register A and B!", let's just... select two registers to mess with

enum INSTRUCTIONS{ // 255 instructions (more that you'll ever need :3 )
    NOP = 0, // do nothing

    //  Selection for A

    SAA = 1, // Select Operation Register A: A
    SAB = 2, // Select Operation Register A: B
    SAC = 3,
    SAD = 4, // :(
    SAE = 5, 
    SAF = 6, 
    SAG = 7, 
    SAH = 8, 

    // Selection for B

    SBA = 9, // Select Operation Register B: A
    SBB = 10, // Select Operation Register B: B
    SBC = 11,
    SBD = 12,
    SBE = 13, 
    SBF = 14, 
    SBG = 15, 
    SBH = 16, 

    // Jumping, Loading, Storing, Printing, IO

    // Jumps
    JMA = 17, // jump to whatever Register A is equal to.
    JMB = 18, //  jump to whatever Register B is equal to.
    JM2 = 19, // jumps to whatever the next 2 addresses create an address out of.
    J2C = 20, // Jump If Register A + B overflow, reads next 2 addresses.
    
    // Loads
    LNA = 21, // Load Next A, loads next 2 bytes immediately from the next 2 addresses into selected register A.
    LNB = 22, // Load Next B, loads next 2 bytes immediately from the next 2 addresses into selected register B.
    LXP = 23, // Load from expansion port into the expansion register. (not finished)

    // writes/polls
    PAB = 24, // print to terminal (both registers)
    WRA = 25, // Write register A to next 2 addresses
    WRB = 26, // Write register B to next 2 addresses
    WSR = 27, // Write Sum Register to next two addresses
    PRA = 28, // Print register A's contents
    PRB = 29, // Print register B's contents
    PLA = 30, // Poll two chars of input into register A
    PLB = 31, // Poll two chars of input into register B
    PL2 = 32, // Poll four chars of input into register A AND B
    

    // SUM register ops
    LSA = 33, // Load Sum A, loads sum register into selected register A.
    LSB = 34, // Load Sum B, loads sum register into selected register B.

    // Maths

    ADD = 35, // add Selection A and B together, setting the Sum Register
    SUB = 36, // subtract Selection A and B, setting the Sum Register as the result.
    MUL = 37, // multiply Selection A and B, setting the Sum Register as the result.
    DIV = 38, // Divide Selection A and B, setting the Sum Register as the result.

    //other
    LJA = 39, // load next 2 addresses to register selection A, then jump past. 
    LJB = 40, // load next 2 addresses to register selection B, them jump past.
    PR2 = 41, // print next 2 addresses, while skipping them.
};

uint16_t expansionRegister;
uint16_t selectedReigsterA = 0;
uint16_t selectedReigsterB = 0;
uint16_t sumRegister;

uint16_t registersA[8]; // 8 16-bit gp registers for whatever, goes from A to H. might split this into. applies for selections to A
uint16_t registersB[8]; // 8 16-bit gp registers for whatever, goes from A to H. might split this into. applies for selections to B



int main(){

    Program prog1;

    prog1.rom[0] = SAA; // set register A to A in A bank
    prog1.rom[1] = SBA; // set register B to A in B bank

    // just realized i could've used actual strings for this, oh well.

    prog1.rom[2] = PR2;
    prog1.rom[3] = 0x48; // H
    prog1.rom[4] = 0x65; // E
    prog1.rom[5] = PR2;
    prog1.rom[6] = 0x6C; // L
    prog1.rom[7] = 0x6C; // L
    prog1.rom[8] = PR2;
    prog1.rom[9] = 0x6F; // O
    prog1.rom[10] = 0x2C; // ,

    prog1.rom[11] = PR2;
    prog1.rom[12] = 0x20; // 
    prog1.rom[13] = 0x57; // W

    prog1.rom[14] = PR2;
    prog1.rom[15] = 0x6F; // O
    prog1.rom[16] = 0x72; // R
    prog1.rom[17] = PR2;
    prog1.rom[18] = 0x6C; // L
    prog1.rom[19] = 0x64; // D
    prog1.rom[20] = PR2;
    prog1.rom[21] = 0x21; // !
    prog1.rom[22] = 0x0A; // newline 


    //load ROM into RAM
    memcpy(memory,prog1.rom,sizeof(prog1.rom));

    // unlike V1, this will now stop at the end of memory.

    while (address <= sizeof(memory) / sizeof(memory[0]))
    {

        
        int instruction = memory[address]; // we dont need to convert from binary.
        
        switch (instruction)
        {
        case SAA:
            selectedReigsterA = 0;
            break;
        case SBA:
            selectedReigsterB = 0;
            break;
        case PR2:
            printf("%c",memory[address + 1]);
            printf("%c",memory[address + 2]);
            address += 2;
            break;
        default:
            
        }

        address++;

    }
    

}
