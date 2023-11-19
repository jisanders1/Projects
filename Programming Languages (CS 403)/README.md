This repository currently contains the workings of a Lox interpreter. I am currently using the book "Crafting Interpreters" and translating from Java to C#.

In order to run it, you should verify that you have C# installed on your computer. I would also suggest installing Visual Studio since this is the IDE that I created this project with. Then you should naviagate to Jlox/Lox Interpreter/Tool in your terminal and enter the following commands:
```
csc.exe GenerateAst.cs
GenerateAst.exe ../Lox
```
This will generate a script that allows the languge to be properly converted into a syntax tree. Afterwards, you should comment out the Main method [GenerateAst.cs](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Tool/GenerateAst.cs) in order to properly run the file. I'm aware that this solution isn't the most eloquent but I'm not having much luck with typical compiling and running. Next, navigate to Jlox/Lox Interpreter and use the following commands to start the interpreter:
```
dotnet run
```
This starts an interactive program within the terminal where you can enter codes of Lox.
```
dotnet run <filename>.txt
```
This runs code from a file within the Lox Interpreter/Lox directory. Currently, the tokens are just being spit back out into the terminal. But they will be parsed and interpreted later on.

Current Issues to Note:
- Needing to comment out different Main methods in order to execute the program properly. This is extremely tedious and leaves a bit of room for error.
- A makefile in order to run the program as intended. "dotnet run" should be "jlox" in order to make what's occuring more clear.
- There are a couple of warnings warning that certain strings could possibly be null. However, for the purposes of the interpreter and fromm my testing thus far, these do not wreck the program.
- I should probably start making test cases with unit testing to ensure everything is running smoothly. This could probably be handled as soon as I figure out the multiple main problems. However, I do not think this is extremely urgent given the fact that I am refactoring code from a book, and there code is most likely well tested.
- There is no "throws IOException" equivalent in C# and this may affect how the program works.
- Parse currently allows a ")" when there is no opening parenthesis. I'm not sure if this is intended or not but I will continue to monitor it as time goes on.