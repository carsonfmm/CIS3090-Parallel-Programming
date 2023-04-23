// Name: Carson Mifsud (  )
// Date: 2021-02-11
/* Description: A serial program that will encrypt plain text entered by the user. This is done using a simple substitution cipher and will place 
 * the encrypted string in the file called "ciphertext.txt". The encryption dictionary for this program was implemented via the use of arrays.
 */

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */

int main ( int argc, char **argv ) {

    int i = 0;
    int j = 0;
    int ran = 0;
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

    // Used for string shuffle randomization
    srand ( time ( NULL ) );

    // The only command line argument is the file name. The program will prompt the user to enter a string after the program is compiled and executed
    if ( argc != 1 ) {
        printf ( "Error: To execute the a2encrypt.c program please use \"./a2encrypt\". The program will then prompt the user to input a string.\n" );
        exit ( -1 );
    }

    // File pointer
    FILE *fp = NULL;

    // Check if file can be opened for writting in binary mode
    if ( ( fp = fopen ( "ciphertext.txt", "wb" ) ) == NULL ) {

       printf ( "Error: Cannot write to ciphertext.txt\n" );

       exit ( 1 );
    }

    char *userInput = malloc ( sizeof ( char * ) * 1000 );
    char *removeLetters = malloc ( sizeof ( char * ) * 1000 );
    char *randomizedCharacters = malloc ( sizeof ( char * ) * 1000 );
    char *finalEncryption = malloc ( sizeof ( char * ) * 1000 );

    // Get user input
    printf ( "Enter a string: " );
    fgets ( userInput, 1000, stdin );

    // Remove the '\n' character from the users input
    for ( i = 0; userInput[i] != '\0'; i++ ) {}
    userInput[i-1] = '\0';

    // Initialize removeLetters with an empty string
    strcpy ( removeLetters, "" );

    // Create the input dictionary
    for ( i = 0; userInput[i] != '\0'; i++ ) {

        if ( userInput[i] == 'a' ) {
            if ( aCheck == 0 ) {
                strcat ( removeLetters, "a" );
                aCheck = 1;
            }
        } else if ( userInput[i] == 'b' ) {
            if ( bCheck == 0 ) {
                strcat ( removeLetters, "b" );
                bCheck = 1;
            }
        } else if ( userInput[i] == 'c' ) {
            if ( cCheck == 0 ) {
                strcat ( removeLetters, "c" );
                cCheck = 1;
            }
        } else if ( userInput[i] == 'd' ) {
            if ( dCheck == 0 ) {
                strcat ( removeLetters, "d" );
                dCheck = 1;
            }
        } else if ( userInput[i] == 'e' ) {
            if ( eCheck == 0 ) {
                strcat ( removeLetters, "e" );
                eCheck = 1;
            }
        } else if ( userInput[i] == 'f' ) {
            if ( fCheck == 0 ) {
                strcat ( removeLetters, "f" );
                fCheck = 1;
            }
        } else if ( userInput[i] == 'g' ) {
            if ( gCheck == 0 ) {
                strcat ( removeLetters, "g" );
                gCheck = 1;
            }
        } else if ( userInput[i] == 'h' ) {
            if ( hCheck == 0 ) {
                strcat ( removeLetters, "h" );
                hCheck = 1;
            }
        } else if ( userInput[i] == 'i' ) {
            if ( iCheck == 0 ) {
                strcat ( removeLetters, "i" );
                iCheck = 1;
            }
        } else if ( userInput[i] == 'j' ) {
            if ( jCheck == 0 ) {
                strcat ( removeLetters, "j" );
                jCheck = 1;
            }
        } else if ( userInput[i] == 'k' ) {
            if ( kCheck == 0 ) {
                strcat ( removeLetters, "k" );
                kCheck = 1;
            }
        } else if ( userInput[i] == 'l' ) {
            if ( lCheck == 0 ) {
                strcat ( removeLetters, "l" );
                lCheck = 1;
            }
        } else if ( userInput[i] == 'm' ) {
            if ( mCheck == 0 ) {
                strcat ( removeLetters, "m" );
                mCheck = 1;
            }
        } else if ( userInput[i] == 'n' ) {
            if ( nCheck == 0 ) {
                strcat ( removeLetters, "n" );
                nCheck = 1;
            }
        } else if ( userInput[i] == 'o' ) {
            if ( oCheck == 0 ) {
                strcat ( removeLetters, "o" );
                oCheck = 1;
            }
        } else if ( userInput[i] == 'p' ) {
            if ( pCheck == 0 ) {
                strcat ( removeLetters, "p" );
                pCheck = 1;
            }
        } else if ( userInput[i] == 'q' ) {
            if ( qCheck == 0 ) {
                strcat ( removeLetters, "q" );
                qCheck = 1;
            }
        } else if ( userInput[i] == 'r' ) {
            if ( rCheck == 0 ) {
                strcat ( removeLetters, "r" );
                rCheck = 1;
            }
        } else if ( userInput[i] == 's' ) {
            if ( sCheck == 0 ) {
                strcat ( removeLetters, "s" );
                sCheck = 1;
            }
        } else if ( userInput[i] == 't' ) {
            if ( tCheck == 0 ) {
                strcat ( removeLetters, "t" );
                tCheck = 1;
            }
        } else if ( userInput[i] == 'u' ) {
            if ( uCheck == 0 ) {
                strcat ( removeLetters, "u" );
                uCheck = 1;
            }
        } else if ( userInput[i] == 'v' ) {
            if ( vCheck == 0 ) {
                strcat ( removeLetters, "v" );
                vCheck = 1;
            }
        } else if ( userInput[i] == 'w' ) {
            if ( wCheck == 0 ) {
                strcat ( removeLetters, "w" );
                wCheck = 1;
            }
        } else if ( userInput[i] == 'x' ) {
            if ( xCheck == 0 ) {
                strcat ( removeLetters, "x" );
                xCheck = 1;
            }
        } else if ( userInput[i] == 'y' ) {
            if ( yCheck == 0 ) {
                strcat ( removeLetters, "y" );
                yCheck = 1;
            }
        } else if ( userInput[i] == 'z' ) {
            if ( zCheck == 0 ) {
                strcat ( removeLetters, "z" );
                zCheck = 1;
            }
        }
    }

    strcpy ( randomizedCharacters, removeLetters );

    // Randomize input dictionary to create encryption dictionary
    // Code partially received from: https://www.tilcode.com/simple-program-to-shuffle-an-array-of-strings-in-c/
    for ( ran = 0; ran < 10; ran++ ) {

        for ( i = 0; i < strlen ( removeLetters ); i++ ) {

            j = 0;
            j = rand() % strlen ( removeLetters );

            char temp = randomizedCharacters[j];
            randomizedCharacters[j] = randomizedCharacters[i];
            randomizedCharacters[i] = temp;

        }
    }

    // Initialize final encryption with an empty string
    strcpy ( finalEncryption, "" );

    // Use encryption dictionary to create encrypted string
    for ( i = 0; i < strlen ( userInput ); i++ ) {

        if ( userInput[i] != ' ' ) {

            char temp1 = userInput[i];
            char temp2;

            for ( j = 0; j < strlen ( removeLetters ); j++ ) {

                if ( removeLetters[j] == temp1 ) {
                    temp2 = randomizedCharacters[j];
                }

            }

            finalEncryption[i] = temp2;

        }

        // Spaces are to be excluded from the encryption
        else {
            finalEncryption[i] = ' ';
        }

    }

    // End string with terminating null character
    finalEncryption[i] = '\0';

    // Write encrypted string to ciphertext.txt
    fputs ( finalEncryption, fp );

    free ( userInput );
    free ( removeLetters );
    free ( randomizedCharacters );
    free ( finalEncryption );

    fclose ( fp );

    return ( 0 );

}
