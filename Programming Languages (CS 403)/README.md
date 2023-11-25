Jaylon Sanders: CS 403

This repository currently contains the workings of a Lox interpreter. I am currently using the book "Crafting Interpreters" and translating from Java to C#.
Notes on each chapter.

Chapter 8:
- Added the ability to evauate statements with scope.
- Handled the copious null errors. This involved the use of '?' with return types/variable types and also the null-coalescent operator '??'.
- Used the book's test case to test scope and variable assignment.
- Successfully build the project and an [XML file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Documentation.xml) where documentation for the classes and methods can be viewed. Formatting is needed to make it more readable. I may be able to generate a makefile with this executable.

Chapter 9:
- Added the ability to evaluate if-else statements and for/while loops.
- Moved GenerateAst.cs to it's own project to fix the Main comment problems, (it's documentation has been moved as well).
- Used the books test case to test for loop. In order to test the if-else statements, I made my own test-cases including an if-elseif-else statement and removed parts of syntax in order to cause errors.
- Considering adding remarks to documentation to have more in depth discussions.

Chapter 10:
- Added the ability to declare and call user-made functions and one of Lox's native functions (clock).
- Renamed the executables to "generate_ast" and "jlox" although I might change the latter to NETlox to reflect that it's completed in C#.
- Added a handle to catch an error when the user specifies a file that does not exist or is not in the path they specified.
- Added some additional comments within the source files for documentations' sake.
- Tested the books test cases along with my own test for the clock function.
- Also considering going through the previous chapter and writing a text file that contains the languages full grammar.

In order to run it, you should verify that you have C# installed on your computer. I would also suggest installing Visual Studio since this is the IDE that I created this project with. Additionally, you should clone this repository. I would build the project again to ensure it works on your operating system. Next, navigate to Jlox/Lox Interpreter/bin/net6.0 in a terminal and use one of the following commands to start the interpreter:
```
jlox
```
This starts an interactive program within the terminal where you can enter codes of Lox.
```
jlox <filename>.txt
```
This runs code from a file within the Jlox/Lox Interpreter/bin/net6.0 directory. You can specify a path and you should be able to run a file from any directory as long as special access is  not needed and the file exists. This interpreter currently allows variable assignments, printing, evaluation of expressions, if/else statements, for/while loops, and functions. It currently does not implement classes. These will come in future chapters.

If the source files [Expr.cs](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Lox/Expr.cs) and [Stmt.cs](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Lox/Stmt.cs) are modified or do not appear to be working properly, then you should naviagate to Jlox/Ast Generator/bin/Debug/net6.0 in your terminal and enter the following commands:
```
generate_ast ../../../../"Lox Interpreter"/Lox
```
This will re-generate both of these scripts as they allow the languge to be properly converted into a syntax tree. 

[This file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Lox/experiment1.txt) is the file that I have been using to test each chapters test cases as well as some of my own. I will be making unit tests in the near future when I am done with chapter 13.

Challenges currently implemented:
- Multi-line comments without nesting of more multi-line comments are allowed.
- Divide by 0 is not allowed and an error will be thrown.

Current Issues to Note:
- Needing to comment out different Main methods in order to execute the program properly. This is extremely tedious and leaves a bit of room for error. [UPDATE 2] I have moved the GenerateAst.cs class to be its own project, therefore, the two projects can be ran nidependently of each other with no issue.
- A makefile in order to run the program as intended. "dotnet run" should be "jlox" in order to make what's occuring more clear. [UPDATE] I have successfully built the project into an executable, from which I may be able to make a makefile. [FIXED] I found the project properties and was able to change the name of executable that is output.
- There are a couple of warnings warning that certain strings could possibly be null. However, for the purposes of the interpreter and fromm my testing thus far, these do not wreck the program. [FIXED] Through some intensive '?' usage for variable/return types and the null-coalescing operator.
- I should probably start making test cases with unit testing to ensure everything is running smoothly. This could probably be handled as soon as I figure out the multiple main problems. However, I do not think this is extremely urgent given the fact that I am refactoring code from a book, and there code is most likely well tested.
- There is no "throws IOException" equivalent in C# and this may affect how the program works. As of chapter 10, everything appears to be working as normal.
- Parse currently allows a ")" when there is no opening parenthesis. I'm not sure if this is intended or not but I will continue to monitor it as time goes on. For now, anything after it is simply not evaluated at all. [FIXED] As of chapter 8, this is now caught as error.
