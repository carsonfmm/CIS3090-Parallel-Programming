Name: Carson Mifsud ( 1089936 )
Date: 2021-13-10
Description gol_data: This program is a data parallelism version of The Game of Life and uses the closed topology. The Game of Life is a cellular life
                      and death simulation based on specific rules. Data parallelism is used in this program and the user will enter the amount of threads
                      to be used in a program. These threads will divide the work of searching through the squares in the grid equally. There are two
                      arrays in this program, one for the current grid and one for the future grid.

Description gol_task: This program is a task parallelism version of The Game of Life and uses the closed topology. The Game of Life is a cellular life
                      and death simulation based on specific rules. Task parallelism is used in this program and the number of threads is predetermined
                      according to the assignment outline via the course website. There will be three threads, one for counting the number of
                      neighbours for each square in the grid, a second for reading through the live queue and updating the next iteration game board,
                      and the third thread for reading through the dead queue and updating the next iteration game board. These queues are implemented
                      with message queue and condition variables which was received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on
                      the course website.

Compile:
    make
Execute gol_data:
    ./gol_data nThreads gridSize nIterations -d
Execute gol_task:
    ./gol_task gridSize nIterations -d
Clean:
    make clean

For program execution:
    - nThreads is the number of threads the program will create to run the game
    - gridSize is one number that represents both the height and width of the grid
    - nIterations is how many iterations of the game the program will run before exiting
    - -d indicates that the program should print out the grid after each iteration
        - If the -d is present then the output should be displayed
        - If the -d is not present then the output is not displayed

Assumptions:
    - For gol_data: If the total number of squares in the grid does not divide evenly into the number of threads, the remainder squares will be calculated
      by the last thread
    - If the number of threads is greater than the total number of squares in the grid than the program will not run and the user will be asked to re-enter
      command line arguments and execute the program again
    - The grid will only print to the terminal after each iteration if the -d flag is entered as the final command line argument
    - The command line arguments must be entered according to the template provided in the assignment description for the program to execute properly
    - The initial grid is randomized using srand() with a 50-50 ratio of alive and dead cells
    - If the -d flag is entered the initial grid will be printed out as well as every grid after for each iteration
    - The program will not execute properly if there are 0 threads for gol_data

Test Case:
    - The user will compile the program
    - The user will execute the program and the program will calculate the appropriate output based on the given Game of Life algorithm and calculate the
      amount of time the execution took based on the command line arguments
    - The total amount of time in seconds the program took to run will be printed to the terminal at the end of the program