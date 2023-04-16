# Projects
This repository contains several of my projects from my college classes. 

The Data Algorithms and Structures folder contains my class projects for implementing my own data structures, including the circular dynamic array, the red-black tree, the binary heap, and the fibonacci heap data structures. These projects are in C++ and consist of classes that can be utilized inside of another file. It can be compiled using:
```
g++ <filename>.cpp -std=c++11
```
Although these data structures are already well-implemented, implementing these data structures on my own helped to enhance my understanding of how these data structures function as well as what common sources of error may be. Additinally, it has served to give me a great deal of respect for those who design data structures such as these.

The Software Engineering folder contains the code for the final project as well as additional information about the project. A manual can be found [here](https://github.com/jisanders1/Projects/blob/main/Software%20Engineering%20(CS%20200)/Project4/manual/CS%20Project%204%20Manual.pdf).

This project was a group project based on creating a system for the ficticious company "Chocolate Anonymous", or "ChocAn" for short. We were given a requirements document and were given the task of designing, developing, and implementing a system to work with their providers, managers, and operators in order to track services provided to members, the records of members and providers, staff payyment, and various reports on the services the center provided. Some of the requirements included:
- Allowing providers to scan in members for their services as well as payment for those services
- Allowing managers to generate reports to realize the amount of money needed for staff payment and view the different types of services provided over the course of a week
- Allowing operators to be able to add, delete, and modify the records of both members and providers.

My main contributions to this project include the controllers (from the MVC model) for modifying the records of [members](https://github.com/jisanders1/Projects/blob/main/Software%20Engineering%20(CS%20200)/Project4/src/projectclasses/ModifyMemberRecords.java) and [providers](https://github.com/jisanders1/Projects/blob/main/Software%20Engineering%20(CS%20200)/Project4/src/projectclasses/ModifyProviderRecords.java) as well as the controller behind the [reports](https://github.com/jisanders1/Projects/blob/main/Software%20Engineering%20(CS%20200)/Project4/src/projectclasses/ReportController.java). Additionally, I helped to provide my other team members with moral support, helping with and reviewing their code, suggesting ideas for improvement to the team, such as figuring out how to pass information between the classes, and dividing tasks up between the team members. 

Although persistence and a GUI were not requirements of the project, I am looking forward to implementing these features during the summer of 2023, so keep an eye out for it! I would also like to thank my team members for helping to complete the project with a grade of 123 out of 120, inclusive of 6 bonus points!

This project can be run by compiling and running the MainMenu.java file. Additonally, some of the operations require entering a identification number and these numbers are currently hardcoded in as follows:
- Operator Identification Numbers: 300000, 300001, 300002
- Manager Identification Numbers: 400000, 400001, 400002, 400003, 400004

With these numbers, you should be able to add or delete providers as necessary.

Additonally, I am also open to constructive criticism and suggestions for improvement! I enjoy learning new things and taking the time to work and better myself as a programmer! Feel free to email me at: jisanders132@gmail.com or jisanders1@crimson.ua.edu.
