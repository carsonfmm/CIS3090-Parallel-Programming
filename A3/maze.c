// Name: Carson Mifsud (  )
// Date: 2021-19-11
/* Description: This program uses a depth first search algorithm to create a path through a two-dimensional array, which represents a
 *              2D maze. This program compiles into two executables. The serial executable is named maze and the parallel executable
 *              is called mazep which Open MP was used to parallelize the algorithm. This program can accept the -n flag followed by
 *              an integer to represent the grid size. This program can accept the -s flag followed by an integer to seed the random
 *              generator for checking the neighbours of a certain index. The serial executable will print the final maze, and the parallel
 *              executable will print the final maze as well and the number of times each thread claimed a neighbour.
 */

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */
#include <omp.h>        /* Open MP */

// A structure to represent a stack
struct coordinateStack {
    int lastInFirstOut;
    unsigned maxSize;
    int* xValue;
    int* yValue;
};

/*
 * Function to swap to given char pointers
 */
// Code partially received from : https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
void swap ( char* x, char* y ) {

    char temp;
    temp = *x;
    *x = *y;
    *y = temp;

}

// Double char array for storing permutations for neighbour checking combinations
char **permutationArray;

/*
 * Function to find and store all permutations of a given char pointer
 */
// Code partially received from : https://stackoverflow.com/questions/57848538/char-permutation-algorithm-in-c-that-stores-the-output-in-an-array
void permutation ( char* inputString, int start, int end ) {

    int i;
    static int count = 0;
    int length = strlen ( inputString );

    // Store permutation
    if ( start == end ) {
        permutationArray[count] = malloc ( sizeof ( char * ) * length + 1 );
        strcpy ( permutationArray[count], inputString );
        count++;
    }

    // Calculate permuation
    else {
        for ( i = start; i <= end; i++ ) {

            swap ( ( inputString + start ), ( inputString + i ) );
            permutation ( inputString, start + 1, end );
            swap ( ( inputString + start ), ( inputString + i ) );

        }
    }
}

/*
 * Function to create a stack of given maxSize.
 */
// Code partially received from : https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
struct coordinateStack* createStack ( unsigned maxSize ) {

    struct coordinateStack* LIFOstack = ( struct coordinateStack * ) malloc ( sizeof ( struct coordinateStack ) * 4 );

    LIFOstack -> maxSize = maxSize;
    LIFOstack -> lastInFirstOut = -1;
    LIFOstack -> xValue = ( int * ) malloc ( LIFOstack -> maxSize * sizeof ( int ) );
    LIFOstack -> yValue = ( int * ) malloc ( LIFOstack -> maxSize * sizeof ( int ) );

    return LIFOstack;

}

/*
 * Function to check if the stack is full.
 */
// Code partially received from : https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
int isLIFOstackFull ( struct coordinateStack* LIFOstack ) {

    return LIFOstack -> lastInFirstOut == LIFOstack -> maxSize - 1;

}

/*
 * Function to check if the stack is empty.
 */
// Code partially received from : https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
int isLIFOstackEmpty ( struct coordinateStack* LIFOstack ) {

    return LIFOstack -> lastInFirstOut == -1;

}

/*
 * Function to push a set of coordinates to the stack.
 */
// Code partially received from : https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
void push ( struct coordinateStack* LIFOstack, int currentXValue, int currentYValue ) {

    if ( isLIFOstackFull ( LIFOstack ) ) {
        return;
    }

    LIFOstack -> xValue[++LIFOstack -> lastInFirstOut] = currentXValue;
    LIFOstack -> yValue[LIFOstack -> lastInFirstOut] = currentYValue;

}

/*
 * Function to pop a set of coordinates from the stack.
 */
// Code partially received from : https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
int pop ( struct coordinateStack* LIFOstack ) {

    if ( isLIFOstackEmpty ( LIFOstack ) ) {
        return -1;
    }

}

int main ( int argc, char **argv ) {

    int i = 0;
    int j = 0;
    int k = 0;
    int arraySize = 0;
    int arraySizeCheck = 0;
    int threadCount = 4;
    int isSeed = 0;
    int currentPos = -1;
    int emptyCheck = 0;
    int currentX = -1;
    int currentY = -1;
    int randomNum = 0;
    int currentRank = -1;
    int totalWrites = 0;
    int writes0 = 0;
    int writes1 = 0;
    int writes2 = 0;
    int writes3 = 0;
    int totalIterations = 0;
    int seedingNumber = 0;

    // Checking for max input of command line arguments
    if ( argc > 5 ) {
        printf ( "Error: Input format: \"./(maze/mazep) -n <array size> (optional) -s <seed> (optional)\n" );
        exit ( -1 );
    }

    // Checking if the -n flag was inputted in the command line arguments
    for ( i = 0; i < argc; i++ ) {

        if ( strcmp ( argv[i], "-n" ) == 0 ) {
            arraySize = atoi ( argv[i+1] );
            arraySizeCheck = 1;

            // Error checking for minimal grid size
            if ( arraySize < 3 ) {
                printf ( "Error: Grid Size: Please input a valid grid size. (NOTE: The grid size must not be smaller than 3.)\n" );
                exit ( -1 );
            }

            // Error checking for even grid size
            if ( (arraySize % 2) == 0 ) {
                printf ( "Error: Grid Size: Please input a valid grid size. (NOTE: The grid size must not an even number.)\n" );
                exit ( -1 );
            }

        }

    }

    // If no -n flag was inputted, set the grid size to a default of 11x11
    if ( arraySizeCheck == 0 ) {
        arraySize = 11;
    }

    // Checking if the -s flag was inputted in the command line arguments
    for ( i = 0; i < argc; i++ ) {

        if ( strcmp ( argv[i], "-s" ) == 0 ) {
            isSeed = 1;
            seedingNumber = atoi ( argv[i+1] );
        }

    }

    // Declare the array used for the maze generation
    char **mazeArray = malloc ( sizeof ( char * ) * ( arraySize ) * ( arraySize ) * sizeof ( mazeArray ));

    for ( i = 0; i < arraySize; i++ ) {
        mazeArray[i] = malloc ( sizeof ( char ) * ( arraySize ) * sizeof ( mazeArray[i] ));
    }

    // Initialize each index in the maze array as a '.'
    for ( i = 0; i < arraySize; i++ ) {
        for ( j = 0; j < arraySize; j++ ) {
            mazeArray[i][j] = '.';
        }
    }

    if ( strcmp(argv[0], "./mazep") == 0 ) {

        // Create a unique stack for each thread
        struct coordinateStack* stack0 = createStack(arraySize*arraySize);
        struct coordinateStack* stack1 = createStack(arraySize*arraySize);
        struct coordinateStack* stack2 = createStack(arraySize*arraySize);
        struct coordinateStack* stack3 = createStack(arraySize*arraySize);

        // Permutation string for every combination of checking neighbours
        char *processPermutaiton = ( char * ) malloc ( sizeof ( char * ) * 5 );
        strcpy ( processPermutaiton, "1234" );

        permutationArray = ( char ** ) malloc ( sizeof ( char * ) * 24 * 5 );

        permutation ( processPermutaiton, 0, strlen ( processPermutaiton )-1 );

        // If no -s flag is used seed the random number generator with 0
        if ( isSeed == 0 ) {
            srand ( 0 );
            randomNum = ( rand() % 24 );
        }

        // Initialize the starting position to (1, 1)
        currentX = 1;
        currentY = 1;

        // Push the starting position to each stack using the four corners of the maze array
        push ( stack0, currentX, currentY );
        push ( stack1, arraySize-2, currentY );
        push ( stack2, currentX, arraySize-2 );
        push ( stack3, arraySize-2, arraySize-2 );

        // Top left stack 0
        mazeArray[currentY][currentX] = '0';
        // Top right stack 1
        mazeArray[currentY][arraySize-2] = '1';
        // Bottom left stack 2
        mazeArray[arraySize-2][currentX] = '2';
        // Bottom right stack 3
        mazeArray[arraySize-2][arraySize-2] = '3';

        // If the -s flag is used it is used to seed the random number generator for determining the order in which the neighbours are visited
        if ( isSeed == 1 ) {
            srand ( seedingNumber );
            randomNum = ( rand() % 24 );
        }

        // Used to determine the maximum number of iterations possible for the inputted grid size
        totalIterations = arraySize / 2;
        totalIterations = totalIterations * totalIterations;

        // The parallel for directive
        #pragma omp parallel for num_threads ( threadCount )
        for ( k = 0; k < (totalIterations*4); k++ ) {

            // Only allow one thread to access a block of code at a time, critical section identified using the critical directive
            #pragma omp critical
            {

                // Get the current thread rank
                currentRank = omp_get_thread_num();

                // Checking the top of the stack for rank 0
                if ( currentRank == 0 ) {

                    // Check if the stack is empty
                    emptyCheck = pop ( stack0 );
                    if ( emptyCheck == -1 ) {
                    } else {
                        currentPos = stack0 -> lastInFirstOut--;
                    }

                    // Initialize the current X and Y values of the current index
                    currentY = stack0 -> yValue[currentPos];
                    currentX = stack0 -> xValue[currentPos];
                }
                
                // Checking the top of the stack for rank 1
                else if ( currentRank == 1 ) {

                    // Check if the stack is empty
                    emptyCheck = pop ( stack1 );
                    if ( emptyCheck == -1 ) {
                    } else {
                        currentPos = stack1 -> lastInFirstOut--;
                    }

                    // Initialize the current X and Y values of the current index
                    currentY = stack1 -> yValue[currentPos];
                    currentX = stack1 -> xValue[currentPos];
                }
                
                // Checking the top of the stack for rank 2
                else if ( currentRank == 2 ) {
                    
                    // Check if the stack is empty
                    emptyCheck = pop ( stack2 );
                    if ( emptyCheck == -1 ) {
                    } else {
                        currentPos = stack2 -> lastInFirstOut--;
                    }

                    // Initialize the current X and Y values of the current index
                    currentY = stack2 -> yValue[currentPos];
                    currentX = stack2 -> xValue[currentPos];
                }
                
                // Checking the top of the stack for rank 3
                else {

                    // Check if the stack is empty
                    emptyCheck = pop ( stack3 );
                    if ( emptyCheck == -1 ) {
                    } else {
                        currentPos = stack3 -> lastInFirstOut--;
                    }

                    // Initialize the current X and Y values of the current index
                    currentY = stack3 -> yValue[currentPos];
                    currentX = stack3 -> xValue[currentPos];
                }

                // If the -s flag is used it is used to seed the random number generator for determining the order in which the neighbours are visited
                if ( isSeed == 1 ) {
                    randomNum = ( rand() % 24 );
                }

                // If the stack is not empty, check neighbours of the current index according to the randomly generated checking order
                if ( emptyCheck != -1 ) {
                    if ( permutationArray[randomNum][0] == '1' ) {

                        // Checking the neighbour 2 indices above the current position
                        if ( !((currentY - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY-2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY-2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY-1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY-2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY-2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY-2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY-2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][0] == '2' ) {
                        
                        // Checking the neighbour 2 indices to the right of the current position
                        if ( !((currentX + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX+2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX+2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX+1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX+2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX+2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX+2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX+2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][0] == '3' ) {

                        // Checking the neighbour 2 indices to the left of the current position
                        if ( !((currentX - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX-2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX-2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX-1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX-2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX-2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX-2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX-2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else {

                        // Checking the neighbour 2 indices below the current position
                        if ( !((currentY + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY+2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY+2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY+1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY+2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY+2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY+2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY+2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    }

                    if ( permutationArray[randomNum][1] == '1' ) {

                        // Checking the neighbour 2 indices above the current position
                        if ( !((currentY - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY-2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY-2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY-1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY-2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY-2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY-2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY-2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][1] == '2' ) {

                        // Checking the neighbour 2 indices to the right of the current position
                        if ( !((currentX + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX+2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX+2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX+1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX+2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX+2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX+2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX+2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][1] == '3' ) {

                        // Checking the neighbour 2 indices to the left of the current position
                        if ( !((currentX - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX-2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX-2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX-1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX-2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX-2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX-2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX-2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else {

                        // Checking the neighbour 2 indices below the current position
                        if ( !((currentY + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY+2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY+2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY+1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY+2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY+2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY+2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY+2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    }

                    if ( permutationArray[randomNum][2] == '1' ) {

                        // Checking the neighbour 2 indices above the current position
                        if ( !((currentY - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY-2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY-2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY-1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY-2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY-2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY-2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY-2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][2] == '2' ) {

                        // Checking the neighbour 2 indices to the right of the current position
                        if ( !((currentX + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX+2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX+2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX+1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX+2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX+2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX+2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX+2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][2] == '3' ) {

                        // Checking the neighbour 2 indices to the left of the current position
                        if ( !((currentX - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX-2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX-2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX-1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX-2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX-2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX-2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX-2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else {

                        // Checking the neighbour 2 indices below the current position
                        if ( !((currentY + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY+2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY+2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY+1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY+2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY+2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY+2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY+2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    }

                    if ( permutationArray[randomNum][3] == '1' ) {
                        
                        // Checking the neighbour 2 indices above the current position
                        if ( !((currentY - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY-2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY-2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY-1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY-2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY-2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY-2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY-2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][3] == '2' ) {

                        // Checking the neighbour 2 indices to the right of the current position
                        if ( !((currentX + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX+2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX+2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX+1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX+2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX+2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX+2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX+2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else if ( permutationArray[randomNum][3] == '3' ) {

                        // Checking the neighbour 2 indices to the left of the current position
                        if ( !((currentX - 2) <= 0) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY][currentX-2] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY][currentX-2] = omp_get_thread_num() + '0';
                                mazeArray[currentY][currentX-1] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX-2, currentY );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX-2, currentY );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX-2, currentY );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX-2, currentY );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }

                    } else {

                        // Checking the neighbour 2 indices below the current position
                        if ( !((currentY + 2) >= arraySize) ) {

                            // Check if the neighbour is a '.'
                            if ( mazeArray[currentY+2][currentX] == '.' ) {
                                totalWrites += 1;

                                // Change the neighbours index to the thread rank number
                                mazeArray[currentY+2][currentX] = omp_get_thread_num() + '0';
                                mazeArray[currentY+1][currentX] = omp_get_thread_num() + '0';

                                // Push the neighbour to the corresponding stack according to the thread rank number and increment the threads total writes
                                if ( omp_get_thread_num() == 0 ) {
                                    push ( stack0, currentX, currentY+2 );
                                    writes0 = writes0 + 1;
                                } else if ( omp_get_thread_num() == 1 ) {
                                    push ( stack1, currentX, currentY+2 );
                                    writes1 = writes1 + 1;
                                } else if ( omp_get_thread_num() == 2 ) {
                                    push ( stack2, currentX, currentY+2 );
                                    writes2 = writes2 + 1;
                                } else {
                                    push ( stack3, currentX, currentY+2 );
                                    writes3 = writes3 + 1;
                                }
                            }
                        }
                    }
                }
            }

        }

        printf ( "\n" );

        // Print the generated maze
        for ( i = 0; i < arraySize; i++ ) {
            
            for ( j = 0; j < arraySize; j++ ) {
                if ( j == arraySize-1 ) {
                    printf ( "%c", mazeArray[i][j] );
                } else {
                    printf ( "%c ", mazeArray[i][j] );
                }
            }
            printf ( "\n" );
        }

        // Print the number of neighbours claimed by each thread
        printf ( "\n" );
        printf ( "thread 0 count: %d\n", writes0 );
        printf ( "thread 1 count: %d\n", writes1 );
        printf ( "thread 2 count: %d\n", writes2 );
        printf ( "thread 3 count: %d\n", writes3 );
        printf ( "\n" );

    } else if ( strcmp(argv[0], "./maze") == 0 ) {

        // Create a stack
        struct coordinateStack* stack0 = createStack(arraySize*arraySize);

        // Permutation string for every combination of checking neighbours
        char *processPermutaiton = ( char * ) malloc ( sizeof ( char * ) * 5 );
        strcpy ( processPermutaiton, "1234" );

        permutationArray = ( char ** ) malloc ( sizeof ( char * ) * 24 * 5 );

        permutation ( processPermutaiton, 0, strlen ( processPermutaiton )-1 );

        // If no -s flag is used seed the random number generator with 0
        if ( isSeed == 0 ) {
            srand ( 0 );
            randomNum = ( rand() % 24 );
        }

        // Initialize the starting position to (1, 1)
        currentX = 1;
        currentY = 1;

        // Push the starting position to the stack using the top left corner of the maze array
        push ( stack0, currentX, currentY );

        // Top left stack 0
        mazeArray[currentY][currentX] = '0';

        // If the -s flag is used it is used to seed the random number generator for determining the order in which the neighbours are visited
        if ( isSeed == 1 ) {
            srand ( seedingNumber );
            randomNum = ( rand() % 24 );
        }

        // Continuously loop through the stack until it is empty
        for ( k = 0; ( pop ( stack0 ) ) != -1; k++ ) {

            // Check if the stack is empty
            emptyCheck = pop ( stack0 );
            if ( emptyCheck == -1 ) {
            } else {
                currentPos = stack0 -> lastInFirstOut--;
            }

            // Initialize the current X and Y values of the current index
            currentY = stack0 -> yValue[currentPos];
            currentX = stack0 -> xValue[currentPos];

            // If the -s flag is used it is used to seed the random number generator for determining the order in which the neighbours are visited
            if ( isSeed == 1 ) {
                randomNum = ( rand() % 24 );
            }

            // Check neighbours of the current index according to the randomly generated checking order
            if ( permutationArray[randomNum][0] == '1' ) {

                // Checking the neighbour 2 indices above the current position
                if ( !((currentY - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY-2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY-2][currentX] = '0';
                        mazeArray[currentY-1][currentX] = '0';
                        
                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY-2 );
                    }
                }

            } else if ( permutationArray[randomNum][0] == '2' ) {

                // Checking the neighbour 2 indices to the right of the current position
                if ( !((currentX + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX+2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX+2] = '0';
                        mazeArray[currentY][currentX+1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX+2, currentY );
                    }
                }

            } else if ( permutationArray[randomNum][0] == '3' ) {

                // Checking the neighbour 2 indices to the left of the current position
                if ( !((currentX - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX-2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX-2] = '0';
                        mazeArray[currentY][currentX-1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX-2, currentY );
                    }
                }

            } else {

                // Checking the neighbour 2 indices below the current position
                if ( !((currentY + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY+2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY+2][currentX] = '0';
                        mazeArray[currentY+1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY+2 );
                    }
                }

            }

            if ( permutationArray[randomNum][1] == '1' ) {

                // Checking the neighbour 2 indices above the current position
                if ( !((currentY - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY-2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY-2][currentX] = '0';
                        mazeArray[currentY-1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY-2 );
                    }
                }

            } else if ( permutationArray[randomNum][1] == '2' ) {

                // Checking the neighbour 2 indices to the right of the current position
                if ( !((currentX + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX+2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX+2] = '0';
                        mazeArray[currentY][currentX+1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX+2, currentY );
                    }
                }

            } else if ( permutationArray[randomNum][1] == '3' ) {

                // Checking the neighbour 2 indices to the left of the current position
                if ( !((currentX - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX-2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX-2] = '0';
                        mazeArray[currentY][currentX-1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX-2, currentY );
                    }
                }

            } else {

                // Checking the neighbour 2 indices below the current position
                if ( !((currentY + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY+2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY+2][currentX] = '0';
                        mazeArray[currentY+1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY+2 );
                    }
                }

            }

            if ( permutationArray[randomNum][2] == '1' ) {

                // Checking the neighbour 2 indices above the current position
                if ( !((currentY - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY-2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY-2][currentX] = '0';
                        mazeArray[currentY-1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY-2 );
                    }
                }

            } else if ( permutationArray[randomNum][2] == '2' ) {

                // Checking the neighbour 2 indices to the right of the current position
                if ( !((currentX + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX+2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX+2] = '0';
                        mazeArray[currentY][currentX+1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX+2, currentY );
                    }
                }

            } else if ( permutationArray[randomNum][2] == '3' ) {

                // Checking the neighbour 2 indices to the left of the current position
                if ( !((currentX - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX-2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX-2] = '0';
                        mazeArray[currentY][currentX-1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX-2, currentY );
                    }
                }

            } else {

                // Checking the neighbour 2 indices below the current position
                if ( !((currentY + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY+2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY+2][currentX] = '0';
                        mazeArray[currentY+1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY+2 );
                    }
                }

            }

            if ( permutationArray[randomNum][3] == '1' ) {

                // Checking the neighbour 2 indices above the current position
                if ( !((currentY - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY-2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY-2][currentX] = '0';
                        mazeArray[currentY-1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY-2 );
                    }
                }

            } else if ( permutationArray[randomNum][3] == '2' ) {

                // Checking the neighbour 2 indices to the right of the current position
                if ( !((currentX + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX+2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX+2] = '0';
                        mazeArray[currentY][currentX+1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX+2, currentY );
                    }
                }

            } else if ( permutationArray[randomNum][3] == '3' ) {

                // Checking the neighbour 2 indices to the left of the current position
                if ( !((currentX - 2) <= 0) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY][currentX-2] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY][currentX-2] = '0';
                        mazeArray[currentY][currentX-1] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX-2, currentY );
                    }
                }

            } else {

                // Checking the neighbour 2 indices below the current position
                if ( !((currentY + 2) >= arraySize) ) {

                    // Check if the neighbour is a '.'
                    if ( mazeArray[currentY+2][currentX] == '.' ) {

                        // Change the neighbours index to '0'
                        mazeArray[currentY+2][currentX] = '0';
                        mazeArray[currentY+1][currentX] = '0';

                        // Push the neighbour to the stack
                        push ( stack0, currentX, currentY+2 );
                    }
                }
            }

        }

        printf ( "\n" );

        for ( i = 0; i < arraySize; i++ ) {
            
            for ( j = 0; j < arraySize; j++ ) {
                if ( j == arraySize-1 ) {
                    printf ( "%c", mazeArray[i][j] );
                } else {
                    printf ( "%c ", mazeArray[i][j] );
                }
            }
            printf ( "\n" );
        }

        printf ( "\n" );

    } else {
        printf ( "Executable error - Invalid executable: %s\n", argv[0] );
    }

    for ( i = 0; i < arraySize; i++ ) {
        free ( mazeArray[i] );
    }

    free ( mazeArray );

    return ( 0 );

}
