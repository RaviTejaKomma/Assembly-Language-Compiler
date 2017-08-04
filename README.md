# Assembly-Language-Compiler

This is a windows console application built using C language. The main aim of this project is to implement an assembly language compiler which parses and executes the given assembly code in an .asm file.

A compiler is a program that converts instructions into a machine-code or lower-level form so that they can be read and executed by a computer. 

**The instruction set of the language is predefined and the datasheet corresponding to the instructions is as follows :**

1. There are 8 registers namely : AX, BX, CX, DX, EX, FX, GX, HX 
2. Any arithmetic operation can be done only using registers.
3. There are two input/output instruction.
4. Supported Arithmetic operators are ADD, SUB, MUL ,DIV.
5. Logical operations IF THEN ELSE are supported.
6. JUMP instruction is used to jump to the corresponding label in the program.
7. Program execution starts with the keyword START and ends with the keyword END.

**SOFTWARE REQUIREMENTS :**
1. Operating system : WINDOWS
2. Language         :  C
3. IDE              : Visual Studio 


The Power Point Presentation provided above consists clear expalanation of the generation of  Intermediate language,Symbol table, Memory table etc and also the step wise execution of the sample assembly code. Sample assembly code is provided in the sample.asm file.

The code was written in Visual Studio IDE, so it is supposed to be compiled using a VS compiler. Specifically, this program was tested in Visual Studio 2013 on a 64-bit computer.

Note: Do not have high expectations on this compiler. If you try to compile a program which vioaltes the rules there's a good chance to see compile errors or miscompilations. This is basically a one-man project, and I have spent only a few days of my spare time so far.
