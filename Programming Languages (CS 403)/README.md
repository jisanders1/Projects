This repository currently contains the workings of a Lox interpreter. I am currently using the book "Crafting Interpreters" and translating from Java to C#.
Notes on each chapter:

Chapter 8:
- Added the ability to evauate statements with scope.
- Handled the copious null errors. This involved the use of '?' with return types/variable types and also the null-coalescent operator '??'.
- Used the book's test case to test scope and variable assignment.
- Successfully build the project and an [XML file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Documentation.xml) where documentation for the classes and methods can be viewed. Formatting is needed to make it more readable. I may be able to generate a makefile with this executable.

In order to run it, you should verify that you have C# installed on your computer. I would also suggest installing Visual Studio since this is the IDE that I created this project with. Then you should naviagate to Jlox/Lox Interpreter/Tool in your terminal and enter the following commands:
```
csc.exe GenerateAst.cs
GenerateAst.exe ../Lox
```
This will generate a script that allows the languge to be properly converted into a syntax tree. Afterwards, you should comment out the Main method [GenerateAst.cs](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Tool/GenerateAst.cs) in order to properly run the file. I'm aware that this solution isn't the most eloquent but I'm not having much luck with typical compiling and running. Next, navigate to Jlox/Lox Interpreter/bin/net6.0 and use the following commands to start the interpreter:
```
"Lox Interpreter".exe
```
This starts an interactive program within the terminal where you can enter codes of Lox.
```
"Lox Interpreter".exe <filename>.txt
```
This runs code from a file within the Jlox/Lox Interpreter/bin/net6.0 directory. This interpreter currently allows variable assignments, printing, and evaluation of expressions. It does not evaulate if/else statements not for/while loops, at the moment, nor implement classes. These will come in future chapters.

Challenges currently implemented:
- Multi-line comments without nesting of more multi-line comments are allowed.
- Divide by 0 is not allowed and an error will be thrown.

Current Issues to Note:
- Needing to comment out different Main methods in order to execute the program properly. This is extremely tedious and leaves a bit of room for error.
- A makefile in order to run the program as intended. "dotnet run" should be "jlox" in order to make what's occuring more clear. [UPDATE] I have successfully built the project into an executable, from which I may be able to make a makefile.
- There are a couple of warnings warning that certain strings could possibly be null. However, for the purposes of the interpreter and fromm my testing thus far, these do not wreck the program. [FIXED] Through some intensive '?' usage for variable/return types and the null-coalescing operator.
- I should probably start making test cases with unit testing to ensure everything is running smoothly. This could probably be handled as soon as I figure out the multiple main problems. However, I do not think this is extremely urgent given the fact that I am refactoring code from a book, and there code is most likely well tested.
- There is no "throws IOException" equivalent in C# and this may affect how the program works. As of chapter 8, everything appears to be working as normal.
- Parse currently allows a ")" when there is no opening parenthesis. I'm not sure if this is intended or not but I will continue to monitor it as time goes on. For now, anything after it is simply not evaluated at all. [FIXED] As of chapter 8, this is now caught as error.
