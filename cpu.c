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

uint16_t address; // can address 65535 bytes (65k of ram) this unsigned 16-bit int makes us... 16 bit!

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

    SBA = 1, // Select Operation Register B: A
    SBB = 2, // Select Operation Register B: B
    SBC = 3,
    SBD = 4,
    SBE = 5, 
    SBF = 6, 
    SBG = 7, 
    SBH = 8, 

    // Jumping, Loading, Storing, Printing, IO

    // Jumps
    JMA = 9, // jump to whatever Register A is equal to.
    JMB = 10, //  jump to whatever Register B is equal to.
    JM2 = 11, // jumps to whatever the next 2 addresses define 
    J2C = 12, // Jump If Register A + B overflow, reads next 2 addresses
    
    // Loads
    LNA = 13, // Load Next A, loads next 2 bytes immediately from the next 2 addresses into selected register A.
    LNB = 14, // Load Next B, loads next 2 bytes immediately from the next 2 addresses into selected register B.
    LXP = 15, // Load from expansion port into the expansion register. (not finished)

    // writes/polls
    PRB = 16, // print to terminal (both registers)
    WRA = 17, // Write register A to next 2 addresses
    WRB = 18, // Write register B to next 2 addresses
    WSR = 19, // Write Sum Register to next two addresses
    PRA = 20, // Print register A's contents
    PRB = 21, // Print register B's contents
    PLA = 22, // Poll two chars of input into register A
    PLB = 23, // Poll two chars of input into register B
    PL2 = 24, // Poll four chars of input into register A AND B
    

    // SUM register ops
    LSA = 25, // Load Sum A, loads sum register into selected register A.
    LSB = 26, // Load Sum B, loads sum register into selected register B.

    // Maths

    ADD = 27, // add Selection A and B together, setting the Sum Register
    SUB = 28, // subtract Selection A and B, setting the Sum Register as the result.
};

uint16_t expansionRegister;

uint16_t sumRegister;

uint16_t registers[8]; // 8 16-bit gp registers for whatever, goes from A to H.

int main(){

    Program prog1;

    // first instruction, load data into register
    prog1.rom[0] = 0b00000101;  // |00| |000| |101| (LON) into r0

    prog1.rom[2] = 0b01000101;  // |01| |000| |101| (LON) into r1

    prog1.rom[4] = 0b10000101;  // |10| |000| |101| (LON) into r2

    prog1.rom[6] = 0b11000101;  // |11| |000| |101| (LON) into r3

    //print stuff

    prog1.rom[8] = 0b00000100; // |00| |000| |100| (PRT) print out reg 0
    prog1.rom[9] = 0b01000100; // |01| |000| |100| (PRT) print out reg 1
    prog1.rom[10] = 0b10000100; // |10| |000| |100| (PRT) print out reg 2
    prog1.rom[11] = 0b11000100; // |10| |000| |100| (PRT) print out reg 3


    prog1.rom[12] = 0b00000110; // |00| |000| |110| (POL) poll input for register 0

    prog1.rom[11] = 0b00000100; // |10| |000| |100| (PRT) print out reg 0

    prog1.rom[13] = 0b00111000; // kill process

    //addresses for text
    prog1.rom[1] = 0b01010111; // W
    prog1.rom[3] = 0b01001000; // H
    prog1.rom[5] = 0b01011001; // Y... did i make this?
    prog1.rom[7] = 0x0A; // newline

    //load ROM into RAM
    memcpy(memory,prog1.rom,sizeof(prog1.rom));

    // end of ROM block


    // should jump to position and execute
    int shouldJump = FALSE;

    // should load the next address and move on
    int shouldLoad = FALSE;

    // the register we are loading data into
    int loadRegister = 0; 

    //ncurses being ncurses part 1
    initscr();
    cbreak();
    noecho();
    timeout(-1); //makes getch block

    char t = getch();

    for (address = 0x0; address < sizeof(memory)/sizeof(memory[0]); address++){

   
        refresh();


        if(memory[address] == 0b00111000){
            printw("00111000 detected! KILLING!");
            refresh();
            return 0;
        }

        if(shouldJump == TRUE){
            shouldJump = FALSE;
            address = memory[address];
        } // keep going

        if(shouldLoad == TRUE){
            shouldLoad = FALSE;
            registers[loadRegister] = memory[address];
            continue; // STOP! This could be dangerous to run!
        }

            // x & 1 == Least Significant Bit (LSB)
            // >> 1 moves byte right by 1 (0b00000010 becomes 0b00000001)

        // add first 3 bits up to get instruction number, multiply so they are in the right spot
        int instructionBinary = (memory[address] & 1) + (((memory[address] >> 1) & 1) * 10) + (((memory[address] >> 2) & 1) * 100);

        int instruction = 0;

        int base = 1;

        // convert the binary number to a real number #1
        while (instructionBinary > 0)
        {
                
                int digit = instructionBinary % 10; 
                instruction += digit * base; // convert a binary digit to a decimal value
                instructionBinary /= 10;
                base *= 2; 
    
        }
        

        // 4th shift puts us in the dummy space
        int dummyBinary =  ((memory[address] >> 3) & 1) + (((memory[address] >> 4) & 1) * 10) + (((memory[address] >> 5) & 1) * 100);
        
        int dummy = 0;

        // convert the binary number to a real number #2
        while (dummyBinary > 0)
        {
                
                int digit = dummyBinary % 10; 
                dummy += digit * base;
                dummyBinary /= 10;
                base *= 2; 
    
        }

        
        // and the 7th shift leaves only the last number in the byte
        int registerBinary = ((memory[address] >> 6) & 1) + (((memory[address] >> 7) & 1) * 10);

        base = 1;

        int register1 = 0;

        // convert the binary number to a real number #3
        while (registerBinary > 0)
        {
                
                int digit = registerBinary % 10; 
                register1 += digit * base;
                registerBinary /= 10;
                base *= 2; 
    
        }

        // load next
        if(instruction == LON){
            shouldLoad = TRUE;
            loadRegister = register1;
        }

        //jump
        if(instruction == JMP){
            shouldJump = TRUE;
        }

        // write register (can write to 0 if it overflows!)
        // using dummy writes mthRegister two spaces ahead. Why? Why not?
        if(instruction == WRT){
            if(dummy == 0){
            memory[address++] = registers[register1];
            }else{
                memory[address + 2] = mthRegister;
            }
        }

        // sets math register
        if(instruction == ADD){
            mthRegister = registers[register1] + registers[dummy];
        }

        if(instruction == SUB){
            mthRegister = registers[register1] - registers[dummy];
        }

        if(instruction == POL){

            // ncurses being ncurses part 2
            char data = getch();
            if(data != ERR){
                registers[register1] = data;
            }else{
                return ERR;
            }
            refresh();
  
        }


        // print impl
        if(instruction == PRT){
            printw("%c", registers[register1]);   
            refresh();
        }

    }
    endwin();
    

}
