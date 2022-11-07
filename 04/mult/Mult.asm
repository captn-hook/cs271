// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

@R2
M=0
//R2=0
@R0
D=M
@STEP
D;JGT
//if R0>0 goto STEP
@END
0;JMP
//steeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeep
(STEP)

    @R2
    D=M //D=R2

    @R1
    D=D+M //D=R2+R1

    @R2
    M=D //R2=R2+R1

    @R0
    D=M-1
    M=D //R0=R0-1

    @STEP
    D;JGT 

(END)
    @END
    0;JMP
