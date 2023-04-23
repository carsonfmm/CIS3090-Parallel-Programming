Name: Carson Mifsud (  )
Date: 2021-02-11
Description a2encrypt: A serial program that will encrypt plain text entered by the user. This is done using a simple substitution cipher and will place 
the encrypted string in the file called "ciphertext.txt". The encryption dictionary for this program was implemented via the use of arrays.

Description a2decrypt: A parallel program using MPI that will use a brute-force decryption algorithm on the encrypted string in "ciphertext.txt". The 
program will decrypt the string read from "ciphertext.txt" and display all the valid decrypted values as well as its process rank. The SoCS system 
dictionary is used to check the decrypted results are valid English words. This program requires one process per unique decryption dictionary and 
uses permutation to brute-force calculate all possible decryptions.

Compile:
    make
Execute a2encrypt:
    ./a2encrypt
Execute a2decrypt:
    mpiexec -n (Number of processes) ./a2decrypt
Clean:
    make clean

Limitations:
    - Maximum user input length is 1000 characters

Assumptions:
    - Character input for the encryption string will only be lowercase alphabetical letters and the space key
    - Did not account for case where encryption string will shuffle back into the original input string
    - File (ciphertext.txt) to be written to must be in the same directory that the program is running as this was hard coded
    - Assume the tester of the a2decrypt.c program knows how many processes to input in the command line (One process per unique decryption dictionary)
    - No penalty for memory leaks
    - The message that prints out after executing the a2decrypt.c program may be ignored. The message states the following: "beignet-opencl-icd: 
    no supported GPU found, this is probably the wrong opencl-icd package for this hardware (If you have multiple ICDs installed and OpenCL works, 
    you can ignore this message)"
    - Time was measured within the program using the calcTime function. The time begins counting at the start of the program and ends after the
    MPI_Finalize() function call. The time was printed using process 0. All code used for timing for the A2report.pdf has been commented in the a2decrypt.c
    program.


Test Case:
    - The user will compile the a2encrypt.c program.
    - The user will execute the a2encrypt.c program and will be prompted to enter a string.
    - The a2encrypt.c program will use a simple substitution cipher and will place the encrypted string in the file called "ciphertext.txt".
    - When the encrypted string is sent to the file it will overwrite the file and the file will only contain the encrypted string.
    - If there is no file called "ciphertext.txt" in the current working directory of the program, the a2encrypt.c program will automatically create the 
    file called "ciphertext.txt" and write the encrypted string to the file.
    - The total amount of time in seconds the a2encrypt.c program took to run will be printed to the terminal at the end of the program.
    - The user will compile the a2decrypt.c program.
    - The user will execute the a2decrypt.c program and input the required amount of processes for MPI (One process per unique decryption dictionary).
    - The a2decrypt.c program will read the encrypted string from the "ciphertext.txt" file.
    - The a2decrypt.c program will display all the valid decrypted values as well as its process rank.
    - The total amount of time in seconds the a2decrypt.c program took to run will be printed to the terminal at the end of the program.

Citations:
    - MPI code received from Professor Nikitenko via the Moodle course website from slides titled "CIS3090 Lecture 7 - parallel programming with MPI"
    - MPI code received from Professor Nikitenko via the Moodle course website from zip folder titled "ch3"
    - https://www.tilcode.com/simple-program-to-shuffle-an-array-of-strings-in-c/
    - https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
    - https://stackoverflow.com/questions/57848538/char-permutation-algorithm-in-c-that-stores-the-output-in-an-array
    - https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    - https://man7.org/linux/man-pages/man3/system.3.html
    - https://linux.die.net/man/1/grep
