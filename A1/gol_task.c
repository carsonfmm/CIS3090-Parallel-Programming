// Name: Carson Mifsud (  )
// Date: 2021-13-10
/* Description: This program is a task parallelism version of The Game of Life and uses the closed topology. The Game of Life is a cellular life
 *              and death simulation based on specific rules. Task parallelism is used in this program and the number of threads is
 *              predetermined according to the assignment outline via the course website. There will be three threads, one for counting
 *              the number of neighbours for each square in the grid, a second for reading through the live queue and updating the next
 *              iteration game board, and the third thread for reading through the dead queue and updating the next iteration game board.
 *              These queues are implemented with message queue and condition variables which was received from Professor Nikitenko in
 *              msgqueueCondition.c via Threads.zip on the course website. The total amount of time in seconds the program took to run will be
 *              printed to the terminal at the end of the program. To execute the program, use the following template:
 * 
 *              ./gol_task gridSize nIterations -d
 */

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */

// Message struct for x (rowVal) and y (colVal) values of grid as well as if the message belongs to the dead or alive queue
// ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
typedef struct {
    int colVal;
    int rowVal;
    int deadOrAlive;
} Message;

// Message node ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
typedef struct message_node {
    Message msg;
    struct message_node* next;
} MessageNode;

// Message queue - a singly linked list
// Remove from head, add to tail
// ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
typedef struct {
    MessageNode* head;
    MessageNode* tail;
    pthread_mutex_t mutex;
    
    // Add a condition variable
    pthread_cond_t cond;
} MessageQueue;

// Each thread needs multiple arguments, so we create a dedicated struct
// ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
typedef struct {
    int workerId;
    MessageQueue* q;
} ThreadArgs;

// Global variable used to store the grid size inputted by the user via the command line
int gridSize = 0;

// Global variable used to store the number of iterations inputted by the user via the command line
int nIterations = 0;

// Global variable used to determine if the user entered the -d flag to print the current grid after every iteration
int isPrint = 0;

// Global variables to track the amount of alive and dead cells there are on the grid
int aliveCount = 0;
int deadCount = 0;

// Create a queue and initialize its mutex and condition variable
// ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
MessageQueue* createMessageQueue() {

    MessageQueue* q = (MessageQueue*)malloc(sizeof(MessageQueue));
    q->head = q->tail = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    
    // Initialize the condition variable
    pthread_cond_init(&q->cond, NULL);
    return q;
}

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

// "Send" a message - append it onto the queue ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
void sendMessage(MessageQueue* q, int colVal, int rowVal, int deadOrAlive) {

    MessageNode* node = (MessageNode*)malloc(sizeof(MessageNode));
    node->msg.colVal = colVal;
    node->msg.rowVal = rowVal;
    node->msg.deadOrAlive = deadOrAlive;
    node->next = NULL;

    // critical section
    pthread_mutex_lock(&q->mutex);
    if (q->tail != NULL) {
        q->tail->next = node;       // append after tail
        q->tail = node;
    } else {
        q->tail = q->head = node;   // first node
    }

    // Signal the consumer thread waiting on this condition variable
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);

}

// "Receive" a message - remove it from the queue ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
Message * getMessage(MessageQueue* q, Message* msg_out) {

    int success = 0;
    
    // critical section
    pthread_mutex_lock(&q->mutex);
    
    // Wait for a signal telling us that there's something on the queue
    while(q->head == NULL) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    
    // By the time we get here, we know q->head is not null, so it's all good
    MessageNode* oldHead = q->head;
    *msg_out = oldHead->msg;    // copy out the message
    q->head = oldHead->next;
    if (q->head == NULL) {
        q->tail = NULL;         // last node removed
    }
    free(oldHead);
    success = 1;
    
    // Release lock
    pthread_mutex_unlock(&q->mutex);

    return msg_out;
}

// Global double integer arrays used to store the current and future grids throughout The Game of Life simulation
int **oldGrid;
int **newGrid;

// Global variable to track if every square in the grid has been searched through by the first thread
int doneGridCalculations = 0;

// Message queues for the alive and dead queues
MessageQueue* aliveQ;
MessageQueue* deadQ;

/*
 * This function is used to check the neighbours of a square in the current gird
 */
void * cellCheck ( void * arg ) {

    // Initialize doneGridCalculations back to 0
    doneGridCalculations = 0;

    // Integer variables used throughout the function
    int i = 0;
    int j = 0;
    int n = 0;
    int m = 0;
    int k = 0;
    int totalAlive = 0;

    // Initialize the totalAlive variable to be 0 alive neighbours
    totalAlive = 0;

    // Initialize n and m back to 0
    n = 0;
    m = 0;

    // Begin counting on the grid starting from the 0, to the gridSize on the y axis
    for ( i = 0; i < gridSize; i++ ) {

        // Begin counting on the grid starting from the 0, to the gridSize on the y axis
        for ( j = 0; j < gridSize; j++ ) {

            // Checking neighbours for the top left square on the grid ( edge case )
            if ( i == 0 && j == 0 ) {

                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the top right cell on the grid ( edge case )
            else if ( i == 0 && j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the bottom left cell on the grid ( edge case )
            else if ( i == gridSize-1 && j == 0 ) {

                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the bottom right cell on the grid ( edge case )
            else if ( i == gridSize-1 && j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the cells on the upper edge of the grid ( edge case )
            else if ( i == 0 ) {

                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the cells on the left most edge of the grid ( edge case )
            else if ( j == 0 ) {

                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j+1];
                totalAlive = totalAlive + oldGrid[i+1][j];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the cells on the bottom edge of the grid ( edge case )
            else if ( i == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i-1][j+1];
                totalAlive = totalAlive + oldGrid[i][j+1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }
            
            // Checking neighbours for the cells on the right most edge of the grid ( edge case )
            else if ( j == gridSize-1 ) {

                totalAlive = totalAlive + oldGrid[i-1][j-1];
                totalAlive = totalAlive + oldGrid[i-1][j];
                totalAlive = totalAlive + oldGrid[i+1][j];
                totalAlive = totalAlive + oldGrid[i+1][j-1];
                totalAlive = totalAlive + oldGrid[i][j-1];

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
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

                // Determine if the cell should be placed in the alive or dead queue based on its surrounding neighbours on the current grid
                if ( oldGrid[i][j] == 0 && totalAlive == 3 ) {
                    sendMessage ( aliveQ, i, j, 1 );
                    aliveCount++;
                } else if ( oldGrid[i][j] == 1 && ( totalAlive < 2 || totalAlive > 3 ) ) {
                    sendMessage ( deadQ, i, j, 0 );
                    deadCount++;
                } else {
                    if ( oldGrid[i][j] == 1 ) {
                        sendMessage ( aliveQ, i, j, 1 );
                        aliveCount++;
                    } else {
                        sendMessage ( deadQ, i, j, 0 );
                        deadCount++;
                    }
                }

            }

            // Reset the totalAlive variable back to 0
            totalAlive = 0;

        }

    }

    // Initialize doneGridCalculations to 1 as each square on the grid has been searched
    doneGridCalculations = 1;

    // return NULL
    return NULL;

}

/*
 * This function is used to get a message from the alive queue and place it on the grid
 * ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
 */
void* aliveCheck(void* arg) {

    int test = 1;

    while ( test ) {

        // while the alive queue is not empty
        while ( aliveQ->head != NULL ) {
            Message msg;
            Message* retMsg = getMessage(aliveQ, &msg);
            newGrid[retMsg->colVal][retMsg->rowVal] = 1;
        }

        // Check if the entire grid has been searched
        if ( doneGridCalculations == 1 ) {
            test = 0;
        }
    }

    while ( aliveQ->head != NULL ) {
        Message msg;
        Message* retMsg = getMessage(aliveQ, &msg);
        newGrid[retMsg->colVal][retMsg->rowVal] = 1;
    }

    // Return NULL
    return NULL;
}

/*
 * This function is used to get a message from the dead queue and place it on the grid
 * ( Code received from Professor Nikitenko in msgqueueCondition.c via Threads.zip on the course website )
 */
void* deadCheck(void* arg) {

    int test = 1;

    while ( test ) {

        // while the alive queue is not empty
        while ( deadQ->head != NULL ) {
            Message msg;
            Message* retMsg = getMessage(deadQ, &msg);
            newGrid[retMsg->colVal][retMsg->rowVal] = 0;
        }

        // Check if the entire grid has been searched
        if ( doneGridCalculations == 1 ) {
            test = 0;
        }
    }

    while ( deadQ->head != NULL ) {
        Message msg;
        Message* retMsg = getMessage(deadQ, &msg);
        newGrid[retMsg->colVal][retMsg->rowVal] = 0;
    }

    // Return NULL
    return NULL;

}

// Total number of working threads is predefined to be 3 in the gol_task.c program according the the Assignment 1 description via the course website
#define NUM_WORKERS 3

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
    if ( argc != 3 && argc != 4 ) {
        printf ( "Error: Input format: \"./gol_task <number that represents the height and width of the grid> <number of iterations the game will run before exiting> <-d> (optional)\"\n" );
        exit ( -1 );
    }

    // Integer variables used throughout the program
    int i = 0;
    int j = 0;
    int k = 0;
    int a = 0;
    int n = 0;
    int m = 0;
    int lineTotal = 0;
    int totalCells = 0;

    // Store all command line arguments in variables
    gridSize = atoi ( argv[1] );
    nIterations = atoi ( argv[2] );

    // If argc is 4
    if ( argc == 4 ) {

        // check if the -d flag was entered to print the grid after each iteration
        if ( strcmp ( argv[3], "-d" ) == 0 ) {
            isPrint = 1;
        } else {
            isPrint = 0;
        }

    }

    // Initialize totalCells with the total number of cells in the grid
    totalCells = gridSize * gridSize;

    // malloc for the oldGrid and newGrid
    oldGrid = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( oldGrid ));
    newGrid = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( newGrid ));

    // malloc for the oldGrid and newGrid for each row and column in the grid
    for ( i = 0; i < gridSize; i++ ) {

        oldGrid[i] = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( oldGrid[i] ));
        newGrid[i] = malloc ( sizeof ( int ) * ( gridSize ) * sizeof ( newGrid[i] ));

    }

    // Used to draw the dashes ( - ) of grid to the terminal
    lineTotal = gridSize * 4;

    // srand function using time in order to randomize the cells on the grid after each execution of the program
    srand ( time ( NULL ) );

    // Randomize all the cells in the grid as either dead or alive ( using 1 for alive and 0 for dead ) and initialize the newGrid to be blank ( all dead cells )
    for ( i = 0; i < gridSize; i++ ) {
        for ( j = 0; j < gridSize; j++ ) {
            int randomNum = ( rand() % 2 );
            oldGrid[i][j] = randomNum;
            newGrid[i][j] = 0;
        }
    }

    // Create the alive and dead queues
    aliveQ = createMessageQueue();
    deadQ = createMessageQueue();

    // pthread array for worker threads
    pthread_t tid[NUM_WORKERS];

    // Iterate through The Game of Life for the total number of iterations inputted by the user via the command line arguments
    for ( k = 0; k <= nIterations; k++ ) {

        // Initialize doneGridCalculations back to 0
        doneGridCalculations = 0;

        // Create a worker thread for counting the number of neighbours for each square on the grid
        pthread_create ( &tid[0], NULL, cellCheck, NULL );

        // Create a worker thread for getting messages from the alive queue and placing them onto the grid
        pthread_create ( &tid[1], NULL, aliveCheck, NULL );

        // Create a worker thread for getting messages from the dead queue and placing them onto the grid
        pthread_create ( &tid[2], NULL, deadCheck, NULL );

        // Wait for worker threads to terminate
        for ( a = 0; a < NUM_WORKERS; a++ ) {
            pthread_join ( tid[a], NULL );
        }

        // Initialize doneGridCalculations back to 0
        doneGridCalculations = 0;

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

    //Remember to delete the message queues
    free ( aliveQ );
    free ( deadQ );

    /*
     * Get and display elapsed wall time time
     */
    // ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
    elapsed = calcTime(start);
	printf("Program took %.4f seconds to execute\n", elapsed);

    return ( 0 );

}
