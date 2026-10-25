# CS 300: Data Structures and Algorithms

This portfolio contains my analysis and implementation of an ABCU course advising program.

## Portfolio Artifacts
- [Project One: Pseudocode, Runtime, and Memory Analysis](ProjectOne_Analysis.md)
- [Project Two: C++ Course Planner](ProjectTwo.cpp)

Project One recommends a vector for the small supplied dataset. Project Two implements a hash table using `unordered_map` and sorts a separate vector of course numbers for display. These artifacts represent different design choices rather than an identical implementation of the original recommendation.

## Journal Reflection

### What was the problem you were solving in the projects for this course?
The projects focused on organizing course information for an academic advising program. The program needed to load course information from a file, print courses in alphanumeric order, and display a selected course with its prerequisites. Project One compared possible data structures, while Project Two provided the C++ implementation.

### How did you approach the problem? Consider why data structures are important to understand.
I compared vectors, hash tables, and binary search trees by looking at loading, searching, sorting, and memory requirements. My Project One recommendation favored a vector because the dataset was small and the structure was simple. My final implementation uses a hash table for course lookup and a sorted vector of keys for ordered output. This work helped me understand that a data structure should be selected based on the operations a program needs, not just whether it can store the data.

### How did you overcome any roadblocks you encountered while going through the activities or project?
One challenge addressed in the implementation was producing sorted output from an unordered hash table. I handled that by collecting the course numbers in a vector and sorting them before printing. Another challenge was validating prerequisites that might appear later in the input file. The program checks those references after loading all courses and builds a temporary table so invalid input does not replace previously loaded data.

### How has your work on this project expanded your approach to designing software and developing programs?
The projects expanded my approach by connecting pseudocode and runtime analysis to implementation decisions. I considered the cost of validation and lookup rather than evaluating insertion alone. I also separated storage from presentation: the hash table supports lookup, while the sorted vector controls how the course list is displayed. This showed me that combining structures can address different requirements within the same program.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?
I used separate functions for loading data, parsing lines, normalizing input, printing the course list, and displaying course details. Descriptive names and comments make each function's purpose easier to follow. Input validation and clear error messages also make the program easier to troubleshoot. Keeping these responsibilities separate makes it easier to modify file handling or output without rewriting the entire program.

## Building and Running
With a C++11-compatible compiler or newer:

```sh
g++ -std=c++11 -Wall -Wextra ProjectTwo.cpp -o course_planner
./course_planner
```

On Windows, run the generated `course_planner.exe`.

Use menu option 1 to load a course data file, option 2 to print the sorted list, option 3 to display a course and its prerequisites, and option 9 to exit.

## Input Format and Scope
Provide a headerless text file with one course per line:

```text
courseNumber,title,prerequisite1,prerequisite2
```

Prerequisite fields are optional. Every referenced prerequisite must also have a course record in the file. The parser handles simple comma-separated fields, not quoted fields containing commas. The original course dataset is not included in this repository.

The implementation rejects duplicate course records, self-prerequisites, and missing prerequisites. It does not detect longer prerequisite cycles or enforce a letters-then-digits course-number pattern. The Project One analysis describes the planned algorithms; it is not a claim that every detail of that plan is implemented identically in Project Two.
