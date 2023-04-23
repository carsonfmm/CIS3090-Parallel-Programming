// Name: Carson Mifsud (  )
// Date: 2021-02-11
/* Description:  A parallel program using MPI that will use a brute-force decryption algorithm on the encrypted string in "ciphertext.txt". The 
 * program will decrypt the string read from "ciphertext.txt" and display all the valid decrypted values as well as its process rank. The SoCS system 
 * dictionary is used to check the decrypted results are valid English words. This program requires one process per unique decryption dictionary and 
 * uses permutation to brute-force calculate all possible decryptions.
 */

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */
#include <mpi.h>        /* MPI functions */ 

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

char **permutationArray;

/*
 * Function to find and store all permutations of a given char pointer
 */
// Code partially received from : // https://stackoverflow.com/questions/57848538/char-permutation-algorithm-in-c-that-stores-the-output-in-an-array
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
 * This function is used to calculate the total execution time of the program in seconds
 */
// double calcTime(struct timeval start){
    
//     long long		startusec, endusec;
//     struct timeval	end;
    
//     gettimeofday(&end, NULL);
//     startusec = start.tv_sec * 1000000 + start.tv_usec;
//     endusec = end.tv_sec * 1000000 + end.tv_usec;
//     return (double)(endusec - startusec) / 1000000.0;
// }

int main ( int argc, char **argv ) {

    // Variables used for tracking program execution time ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
    // struct timeval	start;
	// double elapsed;

    /*
     * Get timing data
     */
    // ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
	// gettimeofday(&start, NULL);

    // MPI code received from Professor Nikitenko via the Moodle course website from slides titled "CIS3090 Lecture 7 - parallel programming with MPI"
    // MPI code received from Professor Nikitenko via the Moodle course website from zip folder titled "ch3"

    /* Start up MPI */
    MPI_Init ( NULL, NULL ); 

    // Number of processes
    int comm_sz;

    // Process rank
    int my_rank;

    int test = 0;
   
    char *encryption = malloc ( sizeof ( char * ) * 1000 );
    char *removeLetters = malloc ( sizeof ( char * ) * 1000 );
    char **processStrings = malloc ( sizeof ( char * ) * 65534 );

    for ( test = 0; test < 65534; test++ ) {
        processStrings[test] = malloc ( sizeof ( char * ) * 1000 );
    }

   // Get the number of processes
   MPI_Comm_size ( MPI_COMM_WORLD, &comm_sz ); 

   // Get rank among all processes
   MPI_Comm_rank ( MPI_COMM_WORLD, &my_rank ); 

    // Process 0
    if ( my_rank == 0 ) {

        // Open file for reading
        FILE *fp = fopen ( "ciphertext.txt", "r" );

        // If file cannot be opened for reading
        if ( fp == NULL ) {

            printf ( "Error: Cannot read from ciphertext.txt\n" );
            exit(1);

        }

        // Read in file and store contents in encryption
        while ( fgets ( encryption, 1000, fp ) ) {}

        fclose ( fp );

        int i = 0;
        int j = 0;
        int aCheck = 0;
        int bCheck = 0;
        int cCheck = 0;
        int dCheck = 0;
        int eCheck = 0;
        int fCheck = 0;
        int gCheck = 0;
        int hCheck = 0;
        int iCheck = 0;
        int jCheck = 0;
        int kCheck = 0;
        int lCheck = 0;
        int mCheck = 0;
        int nCheck = 0;
        int oCheck = 0;
        int pCheck = 0;
        int qCheck = 0;
        int rCheck = 0;
        int sCheck = 0;
        int tCheck = 0;
        int uCheck = 0;
        int vCheck = 0;
        int wCheck = 0;
        int xCheck = 0;
        int yCheck = 0;
        int zCheck = 0;

        // Initialize removeLetters with an empty string
        strcpy ( removeLetters, "" );

        // Create the input dictionary
        for ( i = 0; encryption[i] != '\0'; i++ ) {

            if ( encryption[i] == 'a' ) {
                if ( aCheck == 0 ) {
                    strcat ( removeLetters, "a" );
                    aCheck = 1;
                }
            } else if ( encryption[i] == 'b' ) {
                if ( bCheck == 0 ) {
                    strcat ( removeLetters, "b" );
                    bCheck = 1;
                }
            } else if ( encryption[i] == 'c' ) {
                if ( cCheck == 0 ) {
                    strcat ( removeLetters, "c" );
                    cCheck = 1;
                }
            } else if ( encryption[i] == 'd' ) {
                if ( dCheck == 0 ) {
                    strcat ( removeLetters, "d" );
                    dCheck = 1;
                }
            } else if ( encryption[i] == 'e' ) {
                if ( eCheck == 0 ) {
                    strcat ( removeLetters, "e" );
                    eCheck = 1;
                }
            } else if ( encryption[i] == 'f' ) {
                if ( fCheck == 0 ) {
                    strcat ( removeLetters, "f" );
                    fCheck = 1;
                }
            } else if ( encryption[i] == 'g' ) {
                if ( gCheck == 0 ) {
                    strcat ( removeLetters, "g" );
                    gCheck = 1;
                }
            } else if ( encryption[i] == 'h' ) {
                if ( hCheck == 0 ) {
                    strcat ( removeLetters, "h" );
                    hCheck = 1;
                }
            } else if ( encryption[i] == 'i' ) {
                if ( iCheck == 0 ) {
                    strcat ( removeLetters, "i" );
                    iCheck = 1;
                }
            } else if ( encryption[i] == 'j' ) {
                if ( jCheck == 0 ) {
                    strcat ( removeLetters, "j" );
                    jCheck = 1;
                }
            } else if ( encryption[i] == 'k' ) {
                if ( kCheck == 0 ) {
                    strcat ( removeLetters, "k" );
                    kCheck = 1;
                }
            } else if ( encryption[i] == 'l' ) {
                if ( lCheck == 0 ) {
                    strcat ( removeLetters, "l" );
                    lCheck = 1;
                }
            } else if ( encryption[i] == 'm' ) {
                if ( mCheck == 0 ) {
                    strcat ( removeLetters, "m" );
                    mCheck = 1;
                }
            } else if ( encryption[i] == 'n' ) {
                if ( nCheck == 0 ) {
                    strcat ( removeLetters, "n" );
                    nCheck = 1;
                }
            } else if ( encryption[i] == 'o' ) {
                if ( oCheck == 0 ) {
                    strcat ( removeLetters, "o" );
                    oCheck = 1;
                }
            } else if ( encryption[i] == 'p' ) {
                if ( pCheck == 0 ) {
                    strcat ( removeLetters, "p" );
                    pCheck = 1;
                }
            } else if ( encryption[i] == 'q' ) {
                if ( qCheck == 0 ) {
                    strcat ( removeLetters, "q" );
                    qCheck = 1;
                }
            } else if ( encryption[i] == 'r' ) {
                if ( rCheck == 0 ) {
                    strcat ( removeLetters, "r" );
                    rCheck = 1;
                }
            } else if ( encryption[i] == 's' ) {
                if ( sCheck == 0 ) {
                    strcat ( removeLetters, "s" );
                    sCheck = 1;
                }
            } else if ( encryption[i] == 't' ) {
                if ( tCheck == 0 ) {
                    strcat ( removeLetters, "t" );
                    tCheck = 1;
                }
            } else if ( encryption[i] == 'u' ) {
                if ( uCheck == 0 ) {
                    strcat ( removeLetters, "u" );
                    uCheck = 1;
                }
            } else if ( encryption[i] == 'v' ) {
                if ( vCheck == 0 ) {
                    strcat ( removeLetters, "v" );
                    vCheck = 1;
                }
            } else if ( encryption[i] == 'w' ) {
                if ( wCheck == 0 ) {
                    strcat ( removeLetters, "w" );
                    wCheck = 1;
                }
            } else if ( encryption[i] == 'x' ) {
                if ( xCheck == 0 ) {
                    strcat ( removeLetters, "x" );
                    xCheck = 1;
                }
            } else if ( encryption[i] == 'y' ) {
                if ( yCheck == 0 ) {
                    strcat ( removeLetters, "y" );
                    yCheck = 1;
                }
            } else if ( encryption[i] == 'z' ) {
                if ( zCheck == 0 ) {
                    strcat ( removeLetters, "z" );
                    zCheck = 1;
                }
            }
        }

        int adder = 1;
        int total = strlen ( removeLetters );

        // Creating all unique decryption dictionaries
        for ( i = 0; i < total; i++ ) {

            processStrings[i][0] = '\0';

            processStrings[i][0] = removeLetters[i];

            for ( j = 0; j < total; j++ ) {

                if ( processStrings[i][0] != removeLetters[j] ) {

                    processStrings[i][adder] = removeLetters[j];
                    adder++;

                }

            }

            processStrings[i][j] = '\0';
            adder = 1;

        }

        int k = 0;

        // Send out encryption and removeLetters to all other processes
        for ( k = 1; k < comm_sz; k++ ) {
            MPI_Send ( encryption, 1000, MPI_CHAR, k, 1, MPI_COMM_WORLD );
            MPI_Send ( removeLetters, 1000, MPI_CHAR, k, 2, MPI_COMM_WORLD );
        }

        int tagSender = 3;
        
        // Send unique decryption dictionaries
        for ( k = 1; k < comm_sz; k++ ) {
            MPI_Send ( processStrings[k], 1000, MPI_CHAR, k, tagSender, MPI_COMM_WORLD ); 
            tagSender++;
        }

    }

    // Synchronize all processes
    MPI_Barrier ( MPI_COMM_WORLD );

    if ( my_rank != 0 ) {

        // Receive encryption from process 0
        MPI_Recv ( encryption, 1000, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

        // Receive removeLetters from process 0
        MPI_Recv ( removeLetters, 1000, MPI_CHAR, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

        int k = 0;
        int tagSender = 3;

        // Receive unique decryption dictionaries to a unique process
        for ( k = 1; k < comm_sz; k++ ) {
            if ( my_rank == k ) {
                MPI_Recv ( processStrings[k], 1000, MPI_CHAR, 0, tagSender, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            }
            tagSender++;
        }

    }

    // Synchronize all processes
    MPI_Barrier ( MPI_COMM_WORLD );

    int k = 0;

    for ( k = 0; k < comm_sz; k++ ) {

        if ( my_rank == k ) {

            char *newDecrypted = malloc ( sizeof ( char * ) * 65534 );
            char **newDecryptedArray = malloc ( sizeof ( char * ) * 1000 );

            int factorial = 1;
            int fact = 0;

            // Get factorial of the length of characters on the processString -  1
            for ( fact = strlen ( processStrings[k] ) - 1; fact != 1; fact-- ) {
                factorial = factorial * fact;
            }

            int length = strlen ( processStrings[k] );
            int itr = 0;

            char *tempString = NULL;

            permutationArray = ( char ** ) malloc ( sizeof ( char * ) * factorial * length );

            // Code partially received from : // https://stackoverflow.com/questions/57848538/char-permutation-algorithm-in-c-that-stores-the-output-in-an-array
            tempString = ( char * ) malloc ( sizeof ( char * ) * length + 1 );

            // Permutation on strings
            permutation ( processStrings[k], 0, length - 1 ); 

            int a = 0;
            int b = 0;
            int c = 0;

            // Create each new encrypted message from the permutated decryption dictionaries
            for ( itr = 0 ; itr < factorial ; itr++ ) {

                newDecrypted[0] = '\0';
                newDecryptedArray[itr] = malloc ( sizeof ( char * ) * 65534 );

                strcpy ( tempString, permutationArray[itr] );

                for ( a = 0; encryption[a] != '\0'; a++ ) {

                    char ch;
                    ch = encryption[a];

                    // Ignore spaces as they are not part of the decryption dictionaries
                    if ( ch == ' ' ) {
                        for ( c = 0; newDecrypted[c] != '\0'; c++ ) {}
                        newDecrypted[c] = ' ';
                        newDecrypted[c+1] = '\0';

                    } else {

                        for ( b = 0; removeLetters[b] != '\0'; b++ ) {
                            if ( ch == removeLetters[b] ) {
                                for ( c = 0; newDecrypted[c] != '\0'; c++ ) {}
                                newDecrypted[c] = tempString[b];
                                newDecrypted[c+1] = '\0';
                            }

                        }

                    }

                }

                strcpy ( newDecryptedArray[itr], newDecrypted );
            }

            // Synchronize all processes
            MPI_Barrier ( MPI_COMM_WORLD );

            char **tokens = malloc ( sizeof ( char * ) * 65534 );

            for ( a = 0 ; a < factorial ; a++ ) {

                // Code partially taken from https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
                int d = 0;
                const char s[2] = " ";
                char *token;

                // Spit strings into tokens using a " " (space) as the delimiter
                token = strtok ( newDecryptedArray[a], s );
                        
                while( token != NULL ) {
                    tokens[d] = malloc ( sizeof ( char * ) * 65534 );
                    strcpy ( tokens[d], token );
                    d++;
                    token = strtok ( NULL, s );
                }

                int overallCheck = 1;
                char* finalPrinter = malloc ( sizeof ( char * ) * 65534 );

                // Using grep check the English dictionary located on the SoCS Servers for each token for all strings
                for ( b = 0; tokens[b] != NULL; b++ ) {

                    char* systemString = malloc ( sizeof ( char * ) * 65534 );
                    int returnCheck = 0;

                    // x flag used to select only those matches that exactly match the whole line and q flag to not write to standard output
                    // Code partially taken from: https://linux.die.net/man/1/grep
                    strcat( systemString, "grep -xq '" );
                    strcat( systemString, tokens[b] );
                    strcat( systemString, "' /usr/share/dict/words" );

                    // System call using grep
                    // Code partially taken from: //https://man7.org/linux/man-pages/man3/system.3.html
                    returnCheck = system ( systemString );

                    // Check if each token in a string is a word in the given dictionary from the SoCS Servers
                    if ( returnCheck != 0 ) {
                        overallCheck = 0;
                    } else {
                        strcat( finalPrinter, tokens[b] );
                        strcat( finalPrinter, " " );
                    }

                }

                // Successful decryption was found
                if ( overallCheck == 1 ) {

                    int p = 0;

                    for ( p = 0; finalPrinter[p] != '\0'; p++ ) {}
                    finalPrinter[p - 1] = '\0';

                    printf ( "Rank %d: %s\n", my_rank, finalPrinter );

                }
            }

            for ( itr = 0 ; itr < factorial ; itr++ ) {
                free ( newDecryptedArray[itr] );
            }

            free ( newDecryptedArray );
            free ( newDecrypted );
        }
    }

    // Synchronize all processes
    MPI_Barrier ( MPI_COMM_WORLD );

    // Shut down MPI
    MPI_Finalize();

    free ( encryption );
    free ( removeLetters );

    for ( test = 0; test < 65534; test++ ) {
        free ( processStrings[test] );
    }

    free ( processStrings );

    /*
     * Get and display elapsed wall time
     */
    // ( Code received from Professor Nikitenko in serialSort.c via Threads.zip on the course website )
    // if ( my_rank == 0 ) {
    //     elapsed = calcTime(start);
	//     printf ( "Program took %.4f seconds to execute\n", elapsed );
    // }

    return ( 0 );

}
