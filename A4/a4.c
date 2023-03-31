// Name: Carson Mifsud ( 1089936 )
// Date: 2021-05-12
/* Description: This is an OpenCL program written in C that implements the 1-D Game of Life cellular automation described by J. Millen. 
 * This program creates an image made up of ASCII characters by calculating one row at a time based upon the previous row. This program 
 * can accept the -n flag followed by an integer to represent the number of kernels. This program can accept the -s flag followed by an 
 * integer to represent the height and width of the array. This program can accept the -i flag followed by an integer which indicates 
 * which initial configuration will be used in the first row of the array. The program will print the final output of the 1-D Game of 
 * Life to the terminal once completed.
 */

#define PROGRAM_FILE "a4.cl"
#define KERNEL_FUNC "a4"

#include <math.h>       /* Time */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <unistd.h>     /* Symbolic Constants */
#include <string.h>     /* String */
#include <pthread.h>    /* Pthread */
#include <sys/time.h>   /* Time */
#include <time.h>       /* Time */

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

// Code received from Professor Nikitenko via the Moodle course website from zip folder titled "OpenCL-add"
/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   // Identify a platform
   err = clGetPlatformIDs ( 1, &platform, NULL );
   if ( err < 0 ) {
      perror ( "Couldn't identify a platform" );
      exit ( 1 );
   }

   // Access a device
   err = clGetDeviceIDs ( platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL );
   if ( err == CL_DEVICE_NOT_FOUND ) {
      err = clGetDeviceIDs ( platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL );
   }
   if ( err < 0 ) {
      perror ( "Couldn't access any devices" );
      exit ( 1 );
   }

   return dev;

}

// Code received from Professor Nikitenko via the Moodle course website from zip folder titled "OpenCL-add"
// Create program from a file and compile it
cl_program build_program ( cl_context ctx, cl_device_id dev, const char* filename ) {

   cl_program program;
   FILE *program_handle;
   char *program_buffer;
   char *program_log;
   size_t program_size;
   size_t log_size;
   int err;

   // Read program file and place content into buffer
   program_handle = fopen ( filename, "r" );
   if ( program_handle == NULL ) {
      perror ( "Couldn't find the program file" );
      exit ( 1 );
   }

   fseek ( program_handle, 0, SEEK_END );
   program_size = ftell ( program_handle );
   rewind ( program_handle );
   program_buffer = ( char * ) malloc ( program_size + 1 );
   program_buffer[program_size] = '\0';
   fread ( program_buffer, sizeof ( char ), program_size, program_handle );
   fclose ( program_handle );

   // Create program from file
   program = clCreateProgramWithSource ( ctx, 1, ( const char ** ) &program_buffer, &program_size, &err );
   if ( err < 0 ) {
      perror ( "Couldn't create the program" );
      exit ( 1 );
   }
   free ( program_buffer );

   // Build program
   err = clBuildProgram ( program, 0, NULL, NULL, NULL, NULL );
   if ( err < 0 ) {

      // Find size of log and print to std output
      clGetProgramBuildInfo ( program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size );
      program_log = ( char * ) malloc ( log_size + 1 );
      program_log[log_size] = '\0';
      clGetProgramBuildInfo ( program, dev, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL );
      printf ( "%s\n", program_log );
      free ( program_log );
      exit ( 1 );

   }

   return program;
}

// Some OpenCL code and error handling was received from Professor Nikitenko via the Moodle course website from zip folder titled "OpenCL-add"
int main ( int argc, char **argv ) {

   cl_int i = 0;
   cl_int j = 0;
   cl_int n = 0;
   cl_int m = 0;
   int g = 0;
   cl_int nCheck = 0;
   cl_int sCheck = 0;
   cl_int iCheck = 0;

   // OpenCL structures
   cl_device_id device;
   cl_context context;
   cl_program program;
   cl_kernel kernel;
   cl_command_queue queue;
   cl_int err;
   size_t local_size;
   size_t global_size;

   // Data and buffers
   cl_mem gameBoard_buffer;
   int numberOfKernels = 1;
   int arraySize = 20;
   int configurationNumber = 0;

   // Checking for max input of command line arguments
   if ( argc > 7 ) {
      printf ( "Error: Input format: \"./a4 <-n #> (optional) <-s #> (optional) <-i #> (optional)\"\n" );
      exit ( -1 );
   }

   // Checking if the -n flag was inputted in the command line arguments
   for ( i = 0; i < argc; i++ ) {

      if ( strcmp ( argv[i], "-n" ) == 0 ) {

         numberOfKernels = atoi ( argv[i+1] );

         // Checking if the number of kernels is greater than 0
         if ( numberOfKernels <= 0 ) {
            printf ( "Error: Invalid number of kernels: The number of kernels must be greater than 0\n" );
            exit ( -1 );
         } else {
            nCheck = 1;
         }

      }

   }

   // Checking if the -s flag was inputted in the command line arguments
   for ( i = 0; i < argc; i++ ) {

      if ( strcmp ( argv[i], "-s" ) == 0 ) {
         arraySize = atoi ( argv[i+1] );
         sCheck = 1;
      }

   }

   // Checking if the -i flag was inputted in the command line arguments
   for ( i = 0; i < argc; i++ ) {

      if ( strcmp ( argv[i], "-i" ) == 0 ) {

         configurationNumber = atoi ( argv[i+1] );

         // Checking if the configuration number is between 0-4
         if ( configurationNumber <= -1 || configurationNumber >= 5 ) {
            printf ( "Error: Invalid configuration number: The configuration number must be between 0-4\n" );
            exit ( -1 );
         } else {   
            iCheck = 1;
         }

      }

   }

   if ( arraySize == 1 && configurationNumber == 0 ) {

      srand ( time ( NULL ) );

      for ( j = 0; j < arraySize; j++ ) {

         int randomNum = ( rand() % 2 );

         if ( randomNum == 0 ) {
            printf ( "0\n" );
         } else {
            printf ( ".\n" );
         }

      }

      exit ( -1 );
   }

   if ( arraySize <= 0 ) {
      printf ( "Error: Invalid array size: The array size must be greater than 0\n" );
      exit ( -1 );
   }

   if ( numberOfKernels > arraySize ) {
      printf ( "Error: Invalid number of kernels: The number of kernels must not be greater than the array size\n" );
      exit ( -1 );
   }

   // Error checking implemented to make sure the array size is greater than or equal to the number of spaces the configuration will use.
   if ( configurationNumber == 1 && arraySize < 4 ) {
      printf ( "Error: Invalid array size: The array size must be greater than or equal to the number of space the congiguration will use\n" );
      exit ( -1 );
   }

   if ( configurationNumber == 2 && arraySize < 6 ) {
      printf ( "Error: Invalid array size: The array size must be greater than or equal to the number of space the congiguration will use\n" );
      exit ( -1 );
   }

   if ( configurationNumber == 3 && arraySize < 5 ) {
      printf ( "Error: Invalid array size: The array size must be greater than or equal to the number of space the congiguration will use\n" );
      exit ( -1 );
   }

   if ( configurationNumber == 4 && arraySize < 7 ) {
      printf ( "Error: Invalid array size: The array size must be greater than or equal to the number of space the congiguration will use\n" );
      exit ( -1 );
   }

   // If the -n flag was not included in the command line arguments, the number of kernels defaults to 1
   if ( nCheck == 0 ) {
      numberOfKernels = 1;
   }

   // If the -s flag was not included in the command line arguments, the height and width of the array defaults to 20
   if ( sCheck == 0 ) {
      arraySize = 20;
   }

   // If the -i flag was not included in the command line arguments, the initial configuration defaults to 0
   if ( iCheck == 0 ) {
      configurationNumber = 0;
   }

   // Declare the array used for the 1D Game of Life generation
   float **gameBoard = malloc ( sizeof ( float * ) * ( arraySize ) * ( arraySize ) * sizeof ( gameBoard ));
   for ( i = 0; i < arraySize; i++ ) {
      gameBoard[i] = malloc ( sizeof ( float ) * ( arraySize ) * sizeof ( gameBoard[i] ));
   }

   // Declare the array used for the 1D Game of Life generation
   float **gameBoard2 = malloc ( sizeof ( float * ) * ( arraySize ) * ( arraySize ) * sizeof ( gameBoard2 ));
   for ( i = 0; i < arraySize; i++ ) {
      gameBoard2[i] = malloc ( sizeof ( float ) * ( arraySize ) * sizeof ( gameBoard2[i] ));
   }

   // Initialize each index in the maze array as -1
   for ( i = 0; i < arraySize; i++ ) {
      for ( j = 0; j < arraySize; j++ ) {
         gameBoard[i][j] = -1;
         gameBoard2[i][j] = -1;
      }
   }

   // Get the middle point of the array size for the configuration
   int middlePoint = arraySize / 2;

   // Initialize the first row of the array using the specified configuration number
   if ( configurationNumber == 0 ) {

      srand ( time ( NULL ) );

      for ( j = 0; j < arraySize; j++ ) {

            int randomNum = ( rand() % 2 );

            if ( randomNum == 1 ) {
               gameBoard[0][j] = 0;
            }

      }

   } else if ( configurationNumber == 1 ) {

      gameBoard[0][middlePoint-2] = 0;
      gameBoard[0][middlePoint] = 0;
      gameBoard[0][middlePoint+1] = 0;

   } else if ( configurationNumber == 2 ) {

      gameBoard[0][middlePoint-3] = 0;
      gameBoard[0][middlePoint-2] = 0;
      gameBoard[0][middlePoint-1] = 0;
      gameBoard[0][middlePoint] = 0;
      gameBoard[0][middlePoint+1] = 0;
      gameBoard[0][middlePoint+2] = 0;
      
   } else if ( configurationNumber == 3 ) {

      gameBoard[0][middlePoint-2] = 0;
      gameBoard[0][middlePoint] = 0;
      gameBoard[0][middlePoint+1] = 0;
      gameBoard[0][middlePoint+2] = 0;
      
   } else if ( configurationNumber == 4 ) {

      gameBoard[0][middlePoint-3] = 0;
      gameBoard[0][middlePoint-2] = 0;
      gameBoard[0][middlePoint-1] = 0;
      gameBoard[0][middlePoint] = 0;
      gameBoard[0][middlePoint+1] = 0;
      gameBoard[0][middlePoint+2] = 0;
      gameBoard[0][middlePoint+3] = 0;

   }

   /* Create device and context */
   device = create_device();
   context = clCreateContext ( NULL, 1, &device, NULL, NULL, &err );
   if ( err < 0 ) {
      perror ( "Couldn't create a context" );
      exit ( 1 );   
   }

   /* Build program */
   program = build_program ( context, device, PROGRAM_FILE );

   float *testingValues = malloc ( sizeof ( float * ) * ( arraySize ) * ( arraySize ) * sizeof ( testingValues ));
   float *testingValues2 = malloc ( sizeof ( float * ) * ( arraySize ) * ( arraySize ) * sizeof ( testingValues2 ));

   for ( i = 0; i < arraySize; i++ ) {
      for ( j = 0; j < arraySize; j++) {
         testingValues[i * arraySize + j] = gameBoard[i][j];
         testingValues2[i * arraySize + j] = 0;
      }
   }

   // Create buffers
   global_size = numberOfKernels;
   local_size = numberOfKernels;

   gameBoard_buffer = clCreateBuffer ( context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, arraySize * arraySize * sizeof ( float ), testingValues, &err );
   if ( err < 0 ) {
      perror ( "Couldn't create a buffer" );
      exit ( 1 );
   };

   // Create a command queue
   queue = clCreateCommandQueue ( context, device, 0, &err );
   if ( err < 0 ) {
      perror ( "Couldn't create a command queue" );
      exit ( 1 );
   };

   // Create a kernel
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if ( err < 0 ) {
      perror ( "Couldn't create a kernel" );
      exit ( 1 );
   };

   // Create kernel arguments
   err = clSetKernelArg ( kernel, 0, sizeof ( cl_mem ), &gameBoard_buffer );
   err |= clSetKernelArg ( kernel, 1, sizeof ( int ), &configurationNumber );
   err |= clSetKernelArg ( kernel, 2, sizeof ( int ), &arraySize );
   err |= clSetKernelArg ( kernel, 3, arraySize * arraySize * sizeof ( float ), NULL );
   err |= clSetKernelArg ( kernel, 4, sizeof ( int ), &numberOfKernels );
   err |= clSetKernelArg ( kernel, 5, arraySize * arraySize * sizeof ( float ), NULL );
   if ( err < 0 ) {
      perror ( "Couldn't create a kernel argument" );
      exit ( 1 );
   }

   // Enqueue kernel
   err = clEnqueueNDRangeKernel ( queue, kernel, 1, NULL, &global_size,  &local_size, 0, NULL, NULL );
   if ( err < 0 ) {
      switch (err){
         case CL_OUT_OF_RESOURCES:
            printf ( "CL_OUT_OF_RESOURCES\n" );
      }

      perror ( "Couldn't enqueue the kernel" );
      exit ( 1 );
   }

   // Read the kernel's output
   err = clEnqueueReadBuffer ( queue, gameBoard_buffer, CL_TRUE, 0, arraySize * arraySize * sizeof ( float ), testingValues2, 0, NULL, NULL );
   if ( err < 0 ) {
      perror ( "Couldn't read the buffer" );
      exit ( 1 );
   }

   // Initialize gameBoard2 with the values read from the kernel's output
   for ( i = 0; i < arraySize; i++ ) {
       for ( j = 0; j < arraySize; j++) {
           gameBoard2[i][j] = testingValues2[g];
           g++;
       }
   }

   // If the number of kernels is greater than or equal than 10, print an X instead of the kernel id
   if ( numberOfKernels >= 11 ) {
      for ( n = 0; n < arraySize; n++ ) {
         for ( m = 0; m < arraySize; m++ ) {
            if ( gameBoard2[n][m] == -1 ) {
               printf ( ". " );
            } else {
               printf ( "X " );
            }
         }
         printf ( "\n" );
      }
   }
   
   // If the number of kernels is less than 10, print the kernel id
   else {
      for ( n = 0; n < arraySize; n++ ) {
         for ( m = 0; m < arraySize; m++ ) {
            if ( gameBoard2[n][m] == -1 ) {
               printf ( ". " );
            } else {
               printf ( "%.0f ", gameBoard2[n][m] );
            }
         }
         printf ( "\n" );
      }
   }

   // Deallocate resources
   clReleaseKernel ( kernel );
   clReleaseMemObject ( gameBoard_buffer );
   clReleaseCommandQueue ( queue );
   clReleaseProgram ( program );
   clReleaseContext ( context );

   return ( 0 );

}