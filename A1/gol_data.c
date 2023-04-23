// Name: Carson Mifsud ( )
// Date: 2021-13-10
/* Description: This program is a data parallelism version of The Game of Life and uses the closed topology. The Game of Life is a cellular
 *              life and death simulation based on specific rules. Data parallelism is used in this program and the user will enter the
 *              amount of threads to be used in a program. These threads will divide the work of searching through the squares in the
 *              grid equally. There are two arrays in this program, one for the current grid and one for the future grid. The total
 *              amount of time in seconds the program took to run will be printed to the terminal at the end of the program. To execute
 *              the program, use the following template:
 * 
 *              ./gol_data nThreads gridSize nIterations -d
 */

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */

// Global variable used to store the grid size inputted by the user via the command line
int gridSize = 0;

// Global variable used to store the number of threads to be used in the program inputted by the user via the command line arguments
int nThreads = 0;

// Global double integer arrays used to store the current and future grids throughout The Game of Life simulation
int **oldGrid;
int **newGrid;

/*
 * This function is used to calculate the total execution time of the program in seconds
 */
double calcTime(struct timeval start){
    
    long long		startusec, endusec;
    struct timeval	end;
    
    gettimeofday(&end, NULL);
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    return (double)(endusec - startusec) / 1000000.0;
}

/*
 * This function is used to check the neighbours of a cell in the current gird
 */
void * cellCheck ( void * arg ) {

    // Integer variables used throughout the function
    int i = 0;
    int j = 0;
    int n = 0;
    int m = 0;
    int totalAlive = 0;
    int allCells = 0;
    int portions = 0;

    // Calculate the total number of cells in the grid
    allCells = gridSize * gridSize;

    // Calculate the amount of cells each thread will be assigned
    portions = allCells / nThreads;

    // Rank of thread
    long threadNumber = ( long ) arg;

    // Calculate a position on the grid where a thread should begin making calculations for cells
    int begin = threadNumber * portions;

    // Calculate a position on the grid where a thread should stop making calculations for cells
    int end = begin + portions - 1;

    // Initialize the totalAlive variable to be 0 alive neighbours
    totalAlive = 0;

    // Calculate the starting x position on the grid for the thread
    int startColumn = begin % gridSize;

    // Calculate the starting y position on the grid for the thread
    int startRow = begin / gridSize;

    // Calculate the ending x position on the grid for the thread
    int endColumn = end % gridSize;

    // Calculate the ending y position on the grid for the thread
    int endRow = end / gridSize;

    // Initialize tempEnd with the gridSize
    int tempEnd = gridSize;

    // If the last thread is working through the function, initialize the ending positions for x and y to be the gridSize
    if ( threadNumber == nThreads - 1 ) {
        endRow = gridSize-1;
        tempEnd = gridSize;
    }

    // Begin counting on the grid starting from the calculated y position, to the calculated ending y position
    for ( i = startRow; i < endRow + 1; i++ ) {

        // Begin counting on the grid starting from the calculated x position, to the calculated ending x position
        for ( j = startColumn; j < tempEnd; j++ ) {

            // Checking neighbours for the top left cell on the grid ( edge case )
            if ( i == 0 && j == 0 ) {

                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the top right cell on the grid ( edge case )
            else if ( i == 0 && j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the bottom left cell on the grid ( edge case )
            else if ( i == gridSize-1 && j == 0 ) {

                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the bottom right cell on the grid ( edge case )
            else if ( i == gridSize-1 && j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the cells on the upper edge of the grid ( edge case )
            else if ( i == 0 ) {

                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the cells on the left most edge of the grid ( edge case )
            else if ( j == 0 ) {

                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the cells on the bottom edge of the grid ( edge case )
            else if ( i == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the cells on the right most edge of the grid ( edge case )
            else if ( j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }
            
            // Checking neighbours for the cells that are not considered an edge case ( middle cells )
            else {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be alive or dead on the future grid based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    newGrid[i][j] = 1;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    newGrid[i][j] = 0;
                } else {
                    newGrid[i][j] = oldGrid[i][j];
                }

            }

            // Reset the totalAlive variable back to 0
            totalAlive = 0;

        }

        // If the last thread is not working through the function
        if ( threadNumber != nThreads - 1 ) {

            // If i equals the last row to be worked through
            if ( i == endRow ) {

                // Set the tempEnd to the ending x position to be searched by that specific thread on the grid
                tempEnd = endColumn + 1;
            }
        }

        // Initialize the startColumn back to 0
        startColumn = 0;

    }

    // return NULL
    return NULL;

}

int main ( int argc, char **argv ) {

    // Variables used for tracking program execution time ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
    struct timeval	start;
	double elapsed;

    /*
     * Get timing data
     */
    // ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
	gettimeofday(&start, NULL);

    // Checking if the user has entered the correct amount of command line arguments
    if ( argc != 4 && argc != 5 ) {
        printf ( "Error: Input format: \"./gol_data <number of threads> <number that represents the height and width of the grid> <number of iterations the game will run before exiting> <-d> (optional)\"\n" );
        exit ( -1 );
    }

    // Integer variables used throughout the program
    int i = 0;
    int j = 0;
    int k = 0;
    int n = 0;
    int m = 0;
    int totalCells = 0;
    int isPrint = 0;
    int lineTotal = 0;
    int nIterations = 0;

    // Long variable used for the last arguement of pthread_create (arg) for the rank of the thread
    long a = 0;

    // Store all command line arguments in variables
    nThreads = atoi ( argv[1] );
    gridSize = atoi ( argv[2] );
    nIterations = atoi ( argv[3] );

    // If argc is 5
    if ( argc == 5 ) {

        // check if the -d flag was entered to print the grid after each iteration
        if ( strcmp ( argv[4], "-d" ) == 0 ) {
            isPrint = 1;
        } else {
            isPrint = 0;
        }

    }

    // Initialize totalCells with the total number of cells in the grid
    totalCells = gridSize * gridSize;

    // Check if the number of threads is greater than the number of total squares in the grid
    if ( nThreads > totalCells ) {
        printf ( "Error: Input format: \"The number of threads may not be greater than the total number of squares in the grid.\nPlease re-enter the command line arguments and execute the program again.\"\n" );
        exit ( -1 );
    }

    // malloc for the oldGrid and newGrid
    oldGrid = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( oldGrid ) );
    newGrid = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( newGrid ));

    // malloc for the oldGrid and newGrid for each row and column in the grid
    for ( i = 0; i < gridSize; i++ ) {

        oldGrid[i] = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( oldGrid[i] ) );
        newGrid[i] = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( newGrid[i] ) );

    }

    // srand function using time in order to randomize the cells on the grid after each execution of the program
    srand ( time ( NULL ) );

    // Used to draw the dashes ( - ) of grid to the terminal
    lineTotal = gridSize * 4;

    // Randomize all the cells in the grid as either dead or alive ( using 1 for alive and 0 for dead ) and initialize the newGrid to be blank ( all dead cells )
    for ( i = 0; i < gridSize; i++ ) {
        for ( j = 0; j < gridSize; j++ ) {
            int randomNum = ( rand() % 2 );
            oldGrid[i][j] = randomNum;
            newGrid[i][j] = 0;
        }
    }

    // pthread array for worker threads
    pthread_t tid[nThreads];

    // Iterate through The Game of Life for the total number of iterations inputted by the user via the command line arguments
    for ( k = 0; k <= nIterations; k++ ) {

        // Create the total number of threads for checking neighbours of cells inputted by the user via the command line arguments
        for ( a = 0; a < nThreads; a++ ) {
            pthread_create ( &tid[a], NULL, cellCheck, ( void * ) a );
        }

        // Wait for threads to join to avoid memory leaks
        for ( a = 0; a < nThreads; a++ ) {
            pthread_join ( tid[a], NULL );
        }

        // Initialize n and m back to 0
        n = 0;
        m = 0;

        // If the user entered the -d flag command line argument, print out the grid after each iteration
        if ( isPrint == 1 ) {

            for ( n = 0; n < gridSize; n++ ) {

                for ( m = 0; m < lineTotal; m++ ) {
                    printf ( "-" );
                }

                printf ( "-\n" );

                for ( m = 0; m < gridSize; m++ ) {
                    if ( oldGrid[n][m] == 1 ) {
                        printf ( "| X " );
                    } else {
                        printf( "|   " );
                    }
                }

                printf ( "|\n" );

            }

            for ( m = 0; m < lineTotal; m++ ) {
                printf ( "-" );
            }

            printf ( "-\n" );

        }

        // Copy the future grid to the current grid
        for ( i = 0; i < gridSize; i++ ) {
            for ( j = 0; j < gridSize; j++ ) {
                oldGrid[i][j] = newGrid[i][j];
            }
        }

        // Clear the future grid by setting all cell values to dead ( 0 )
        for ( i = 0; i < gridSize; i++ ) {
            for ( j = 0; j < gridSize; j++ ) {
                newGrid[i][j] = 0;
            }
        }

    }

    // Free the oldGrid and newGrid for each row and column in the grid
    for ( i = 0; i < gridSize; i++ ) {

        free ( oldGrid[i] );
        free ( newGrid[i] );

    }

    // free the oldGrid and newGrid
    free ( oldGrid );
    free ( newGrid );

    /*
     * Get and display elapsed wall time time
     */
    // ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
    elapsed = calcTime(start);
	printf("Program took %.4f seconds to execute\n", elapsed);

    return ( 0 );

}
