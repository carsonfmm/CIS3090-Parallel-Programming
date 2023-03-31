Name: Carson Mifsud ( 1089936 )
Date: 2021-19-11
Description: This program uses a depth first search algorithm to create a path through a two-dimensional array, which represents a 2D maze. This program compiles into two executables. The serial executable is named maze and the parallel executable is called mazep which Open MP was used to parallelize the algorithm. This program can accept the -n flag followed by an integer to represent the grid size. This program can accept the -s flag followed by an integer to seed the random generator for checking the neighbours of a certain index. The serial executable will print the final maze, and the parallel executable will print the final maze as well and the number of times each thread claimed a neighbour.

Compile: ( This program compiles into two executables. The serial executable is named maze and the parallel version is called mazep. )
    make
Execute maze:
    ./maze -n <array size> (optional) -s <seed> (optional)
Execute mazep:
    ./mazep -n <array size> (optional) -s <seed> (optional)
Clean:
    make clean

Command Line Flags:
    - "-n": The program accepts -n to indicate the size of the array. For example: "./maze -n 21" would create an array that is 21 by 21 elements. If no -n argument is used, then the array is made 11 by 11 by default.
    - "-s": The -s argument is used to seed the random number generator for determining the order in which the neighbours are visited. If no -s argument is used, the random number generator is seeded with 0.

Limitations:
    - The grid size must not be less than 3.
    - The grid size must not be even.

Assumptions:
    - The command line argument for the grid size will never be less than 3.
    - The serial implementation of the program (./maze) will always begin at coordinates (1, 1).
    - The parallel implementation of the program (./mazep) will always have each thread should start in a different corner of the array. These coordinates are the following (1,1), (1, (size-2)), ((size-2), 1), and ((size-2), (size-2)).
    - Diagonals are not considered neighbours.
    - The parallel version of the program (./mazep) will always run 4 threads.
    - Only odd grid sizes will be used to test the program.
    - No penalty for memory leaks.
    - The values for a flag (-n and -s) will always be placed to the right of the flag separated by a space.
    - The thread counts are dependant on the number of times a neighbour is written and does not include the threads origin point.
    - If the -n flag is given, a grid size must be provided. If the -s flag is given, a seed must be provided.

Test Case:
    - The user will compile the maze.c program.
    - The user will execute the a2encrypt.c program using their desired command line arguments.
    - The program will run either the serial implementation or the parallel inmplementation depending on which program the user executes.
    - For the serial implementation (./maze), the generated maze will be printed to the terminal.
    - For the parallel implementation (./mazep), the generated maze as well as the number of times each thread claimed a neighbour will be printed to the terminal.
    - The program will end.

Citations:
    - Open MP code received from Professor Nikitenko via the Moodle course website from slides titled "CIS3090 Lecture 8 - Open MP" and "CIS3090 A3 overview".
    - Open MP code received from Professor Nikitenko via the Moodle course website from zip folder titled "ch5"
    - https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
    - https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
    - https://stackoverflow.com/questions/57848538/char-permutation-algorithm-in-c-that-stores-the-output-in-an-array