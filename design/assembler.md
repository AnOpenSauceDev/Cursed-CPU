# SSOC Assembler Concept

Because writing an OS in pure binary is probably going to make programming incredibly slow, I'm going to write a "pretty low" level assembler, which is a slightly faster way to write programs. This also my foray into really low-level programming for my own chip.

What I'm going for in terms of an SSOC program.

```SSOC

// double slashes are comments.
// a comma appends two things together
// all DEFINE's must be made before MAIN.

DEFINE HelloWorld AS "Hello, World!", 0xA

// MAIN is your entrypoint
FUNCTION MAIN:
    PRINT HelloWorld // print contents of data at HelloWorld
    JUMP MAIN // jump back to MAIN
    END; // end of a function. Stop execution here.

```
