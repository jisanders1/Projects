# Lox: Tree-Walk Implementation in C#
Jaylon Sanders: CS 403, Dr. Yessick, University of Alabama

This folder contains the workings of a Lox interpreter implemented using the Tree-Walk implementation, which builds a syntax tree and traverses it in order to execute the code. I used the book [_Crafting Interpreters_](https://www.craftinginterpreters.com/) and translated the code from Java to C#. In addition to using some of my own test cases with the books' in-text test cases (found in the file [experiment1.txt](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/NETlox/Lox%20Interpreter/experiment1.txt)), I will also be utilizing most of the test-cases found in the [book's repository](https://github.com/munificent/craftinginterpreters/tree/master/test). Given how similar Java and C# are, these test cases should pass. Given time constraints, I have not ran every single test case, but if you would like to run them, complete the [Building](https://github.com/jisanders1/Projects/tree/main/Programming%20Languages%20(CS%20403)/NETlox#building) instructions and run the program with a test case file from the `Tests` directory. They are grouped by the functionality they check for and each case contains comments on what the expected output should be.

## Building
In order to run it, you should verify that you have .NET6.0 Software Develpoment Kit installed on your computer. Information on how to install it can be found [here](https://dotnet.microsoft.com/en-us/download/dotnet/6.0). Additionally, you should clone this repository.

After completing the above, you should navigate to the folder that contains ```NETlox.sln``` and enter the following to build the project:
```
dotnet build
```
The executable for the program can be found in the ```Lox Interpreter/bin/Debug/net6.0``` directory. You should navigate to this directory in a terminal and use one of the following commands to start the interpreter:
```
netlox
```
This starts an interactive program called the REPL (short for **_R_**eading a line of input, **_E_**valuating it, **_P_**rinting the result, then **_L_**ooping and repeating all four steps again) within the terminal where you can enter lines of code of in the Lox language.
```
netlox <file path.
```
This runs code from a file within a specified file path, given that it exists. You should be able to run a file from any directory as long as special access is  not needed and the file exists. This interpreter currently allows variable assignments, printing, evaluation of expressions, if/else statements, for/while loops, and functions. The program also implements classes with constructors, methods, and simple inheritence (ie, you can only inherit from one class). Personally, I also found that this implementation of a Lox interpreter was much easier to implement and understand when compared to bytecode. If there is an issue finding or opening the file, the program will report it(for example, if the directory does not exist, the program will output ```Directory not found```).

For example, in order to execute the test case ```class/inherited_method.lox``` you enter the following:
```
netlox ../../../../Tests/class/inherited_method.lox
```
and it outputs:
```
in foo
in bar
in baz
```

Running a file with the following contents:
```
fun sum(a, b, c) {
    return a + b + c;
}

print 4 + sum(5, 6, 7);
```
Outputs ```22```.


If the source files ```Expr.cs``` and ```Stmt.cs``` are modified or do not appear to be working properly, then you should naviagate to Jlox/Ast Generator/bin/Debug/net6.0 in your terminal and enter the following commands:
```
generate_ast ../../../../"Lox Interpreter"
```
This will re-generate both of these scripts as they allow the languge to be properly converted into a syntax tree. 

[This file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Lox/experiment1.txt) is the file that I have been using to test each chapters test cases as well as some of my own. I will be making unit tests in the near future when I am done with chapter 13.

## Progression Notes
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

Chapter 11:
- Changed the project executable to "netlox" to reflect the fact that my version of the interpreter uses C# to implement it and not Java.
- Add a resolver class to the project to resolve the what and where of each variable of each program before interpreting it and to catch some additional errors (such as a top-level return statement declaring the same twice in the same scope, etc.).
- Tested the book's test cases to ensure proper errors were thrown.
- May update the catch clause in the Lox class to be more specific with which error it caught since the File class method used throws many different types.
- Added a [Lox grammar file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Full%20Lox%20Grammar.txt) to keep track of what the grammar looks like in one place.

Chapter 12:
- Added classes to my implmentation of lox with constructors, methods, the "this" keyword, and fields/properties. Inheritance will come in the next chapter.
- Updated the catch clause in the Lox class to print more specific information in regards to which error was caught, such as "Directory not found" for the DirectoryNotFound exception or "File is unable to be accessed due to permission requirements." for the SecurityException.
- Changed the for loop in the ResolveLocal method of the [Resolver.cs](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Jlox/Lox%20Interpreter/Lox/Resolver.cs) class to start at the front and increment instead of starting at the back to decrement. The decrement version caused a weird error to occur where the program could not find the "this" keyword althogh it had already been declared and was in the dictionary being searched.
- Updated the [Lox grammar file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Full%20Lox%20Grammar.txt) to reflect the addition of classes.
- Considering moving all projects out of the Lox folder into the Lox Interpreter folder to simplify path calls for both the interpreter itself and the generate_ast project.

Chapter 13:
- Added inheritance to the classes along with the super keyword and
- Updated the [Lox grammar file](https://github.com/jisanders1/Projects/blob/main/Programming%20Languages%20(CS%20403)/Full%20Lox%20Grammar.txt) to reflect the addition of the super keyword and inheritance.
- Test cases downloaded and copied into the project.

Challenges currently implemented:
- Multi-line comments without nesting of more multi-line comments are allowed.
- Divide by 0 is not allowed and an error will be thrown.

## Issues (and Solutions to Issues)
- Needing to comment out different Main methods in order to execute the program properly. This is extremely tedious and leaves a bit of room for error. [UPDATE 2] I have moved the GenerateAst.cs class to be its own project, therefore, the two projects can be ran independently of each other with no issue.
- A makefile in order to run the program as intended. "dotnet run" should be "jlox" in order to make what's occuring more clear. [UPDATE] I have successfully built the project into an executable, from which I may be able to make a makefile. [FIXED] I found the project properties and was able to change the name of executable that is output.
- There are a couple of warnings warning that certain strings could possibly be null. However, for the purposes of the interpreter and fromm my testing thus far, these do not wreck the program. [FIXED] Through some intensive '?' usage for variable/return types and the null-coalescing operator.
- I should probably start making test cases with unit testing to ensure everything is running smoothly. This could probably be handled as soon as I figure out the multiple main problems. However, I do not think this is extremely urgent given the fact that I am refactoring code from a book, and there code is most likely well tested. [UPDATE] Will be making some unit tests along with utilizing some from the book after I am finished with chapter 13.
- There is no "throws IOException" equivalent in C# and this may affect how the program works. As of chapter 11, everything appears to be working as normal.
- Parse currently allows a ")" when there is no opening parenthesis. I'm not sure if this is intended or not but I will continue to monitor it as time goes on. For now, anything after it is simply not evaluated at all. [FIXED] As of chapter 8, this is now caught as error.
- This iteration of the interpreter works well for smaller files will little recursion or small loops, but it is slow when you run much larger files. This issue is allevieted when using a different implementation of a Lox interpreter. For exmple, executing ```benchmark/string_equality.lox``` took about 10 seconds on my computer and running ```benchmark/equality.lox``` to nearly 30 seconds! However, this faster implementation is implemented in a different folder of this repository named [```clox```](https://github.com/jisanders1/Projects/tree/main/Programming%20Languages%20(CS%20403)/clox).