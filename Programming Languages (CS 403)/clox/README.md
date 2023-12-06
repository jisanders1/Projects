Jaylon Sanders: CS 403

This folder containts the workings of a Lox interpreter via a virtual machine in C. I used the book "Crafting Interpreters" and modified the code slightly.

In order to run the program, you should first make sure that you have the ability to compile and run C code. If you are running a Linux based system, then there is a high chance that it is already installed on your computer. If you are using a Windows, you should install the MinGW compiler (for instructions to compile go (here)[https://www.geeksforgeeks.org/installing-mingw-tools-for-c-c-and-changing-environment-variable/]). Next, navigate to the ```Sources``` folder and enter the following line of code to compile the source files:
```
gcc main.c debug.c chunk.c memory.c value.c -o clox
```
After you enter this command, the program sholud be compiled to an executable. In order to run the program, you should type invoke the executable file, which may end in ".exe" or ".out" for most. For Windows users, you should be able to type the following.:
```
clox
```
Currently, this will output: 
```
== test chunk ==
0000  123 CONSTANT_OP         0 '1.2'
0002    | RETURN_OP
```

You can find a copy of my code after each chapter in the ```Progression``` folder.

Chapter 14 - Chunks of Bytecode:
- Added basic functionality for encoding constant and return operations, and for encoding constants and values.
- Separated source and header files into two separate directories.

Current Issues:
- The compilation process is a bit long. Perhaps making a makefile will reduce difficulty. 