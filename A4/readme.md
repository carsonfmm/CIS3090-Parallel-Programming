Name: Carson Mifsud (  )
Date: 2021-05-12
Description: This is an OpenCL program written in C that implements the 1-D Game of Life cellular automation described by J. Millen. This program creates an image made up of ASCII characters by calculating one row at a time based upon the previous row. This program can accept the -n flag followed by an integer to represent the number of kernels. This program can accept the -s flag followed by an integer to represent the height and width of the array. This program can accept the -i flag followed by an integer which indicates which initial configuration will be used in the first row of the array. The program will print the final output of the 1-D Game of Life to the terminal once completed.

Group Members: (This assignment was completed individually)
    - Carson Mifsud (  )

Compile:
    make
Execute a4:
    ./a4 <-n #> (optional) <-s #> (optional) <-i #> (optional)
Clean:
    make clean

Command Line Flags:
    - "-n #": The number represents the number of kernels. NOTE: if n > 10 (i.e. we have more than 10 kernels) display the X character. NOTE: if n <= 10 (i.e. we have less than or equal to 10 kernels) display the id of the kernel. The default number of kernels is 1.
    - "-s #": The number represents both the height and width of the array. The default size of the array is 20.
    - "-i #": The number indicates which initial configuration will be used in the first row of the array. The initial configuration is 0 (random).

Problem Description:
    When inputting an array size (<-s #>) greater than or equal to 65, my program will print the following to the terminal:
        CL_OUT_OF_RESOURCES
        Couldn't enqueue the kernel: Success
    However, this error may be OpenCL related and not an issue with my program as the error message "CL_OUT_OF_RESOURCES" indicates insufficient resources. Please take this into consideration when grading my assignment. Thank you.

Limitations:
    - Error checking implemented to not allow for a number of kernels less than 1.
    - Error checking implemented to make sure the array size is at least 1.
    - Error checking implemented to make sure the array size is greater than or equal to the number of spaces the configuration will use.
    - Error checking implemented to make sure the number of kernels is not greater than the array size.

Assumptions:
    - The 1-D Game of Life was implemented using the closed topology.
    - When using any command line argument flag, it will always have an integer placed to the right of it separated by a space.
    - The makefile in this submission was received from Professor Nikitenko in the file titled "OpenCL-add" by changing the project name to "a4".
    - If the division of the number of kernels amoung the columns of the 1-D Game of Life board is uneven, the last kernels will always have the least number of columns.
    - The configuration for the first row of the 1-D Game of Life goes in the middle of the first row on the board. If this is uneven the configuration will be shifted over by one index.
    - No marks will be deducted for compiler warnings for this assignment.
    - No penalty for memory leaks.

Test Case:
    - The user will compile the a4.c program.
    - The user will execute the a4.c program using their desired command line arguments.
    - The program will initialize the 1-D Game of Life Board with the specified configuration and pass information to the kernel(s).
    - The program will read the completed array back into the C program when all of the calculations for all rows are complete.
    - The generated board will be printed to the terminal. If the number of kernels is greater than 10, the board will display the X character. If the number of kernels is less than or equal to 10 kernels, the board will display the id of the kernel.
    - The program will end.

Citations:
    - Open MP code received from Professor Nikitenko via the Moodle course website from slides titled "CIS3090 Lecture 11a - OpenCLFile", "CIS3090 Lecture 11b - OpenCL and memoryFile", and "CIS3090 A4 overviewFile".
    - Open MP code received from Professor Nikitenko via the Moodle course website from zip folder titled "OpenCL-add"
    - https://www.drdobbs.com/parallel/a-gentle-introduction-to-opencl/231002854?%20pgno=1
    - https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/
    - https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/OpenCL-1.2-refcard.pdf
