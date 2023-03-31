__kernel void a4(__global float4* testingValues, int configurationNumber, int arraySize, __local float* tempArray, int numberOfKernels, __local float* tempArray2) {

   uint n;
   uint m;
   uint i;
   uint j;
   uint k;
   uint oldj = 0;
   uint oldk = 0;
   uint x = 0;
   uint y = 3;
   uint z;
   uint row_counter;
   uint thread_counter = 0;

   // Get the specific number of columns each kernel needs to calculate
   uint specificColumns = arraySize / numberOfKernels;
   if ( ( specificColumns * numberOfKernels ) != arraySize ) {
      specificColumns++;
   }

   uint initialStart = specificColumns;

   // Copy the configuration
   if ( get_local_id(0) == 0 ) {

      for ( i = 0; i < arraySize; i++ ) {
         tempArray[i] = -1;
      }

      j = 0;
      k = 0;

      for ( i = 0; i < arraySize; i++ ) {

         if ( j == 4 ) {
            j = 0;
            k++;
         }

         j++;

      }

   }

   // Begin the calculations for each kernel
   for ( thread_counter = 0; thread_counter < numberOfKernels; thread_counter++ ) {

      if ( (get_global_id(0) == thread_counter) ) {

         row_counter = 0;
         j = 0;
         k = 0;

         // Copy the values of a row from global memory to local memory
         for ( row_counter = 0; row_counter < arraySize-1; row_counter++ ) {
                                 
            for ( i = 0; i < arraySize; i++ ) {

               if ( j == 4 ) {
                  j = 0;
                  k++;
               }
               
               tempArray[i] = testingValues[k][j];

               j++;

            }

            // Perform the calculations for the next row based on the current row
            if ( get_local_id(0) == thread_counter ) {

               for ( i = get_local_id(0) * specificColumns; i < (get_local_id(0) * specificColumns) + initialStart; i++ ) {

                  if ( i < arraySize ) {

                     uint cellCount = 0;

                     // Edge cases for the closed topology
                     if ( i == 0 ) {

                        if ( tempArray[i+1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i+2] != -1 ) {
                           cellCount++;
                        }

                     } else if ( i == 1 ) {

                        if ( tempArray[i-1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i+1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i+2] != -1 ) {
                           cellCount++;
                        }

                     } else if ( i == arraySize-1 ) {

                        if ( tempArray[i-1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i-2] != -1 ) {
                           cellCount++;
                        }

                     } else if ( i == arraySize-2 ) {

                        if ( tempArray[i+1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i-1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i-2] != -1 ) {
                           cellCount++;
                        }

                     } else {

                        if ( tempArray[i+2] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i+1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i-1] != -1 ) {
                           cellCount++;
                        }
                        if ( tempArray[i-2] != -1 ) {
                           cellCount++;
                        }

                     }

                     // Initialize the second local array for the next row
                     if ( (tempArray[i] != -1) && ( cellCount == 2 || cellCount == 4 ) ) {
                        tempArray2[i] = get_local_id(0);
                     } else if ( (tempArray[i] == -1) && ( cellCount == 2 || cellCount == 3 ) ) {
                        tempArray2[i] = get_local_id(0);
                     } else {
                        tempArray2[i] = -1;
                     }

                  }

               }
            
            }

            // Barrier
            barrier ( CLK_LOCAL_MEM_FENCE );

            oldj = j;
            oldk = k;

            // Copy the local values from the array to the global value array
            if ( get_local_id(0) == 0 ) {
               
               for ( i = 0; i < arraySize; i++ ) {

                  if ( j == 4 ) {
                     j = 0;
                     k++;
                  }

                  testingValues[k][j] = tempArray2[i];

                  j++;

               }

            }

            j = oldj;
            k = oldk;

         }

      }

   }

   if ( get_local_id(0) == 0 ) {

      uint replace = 0;

      j = 0;
      k = 0;

      for ( i = 0; i < arraySize; i++ ) {

         if ( j == 4 ) {
            j = 0;
            k++;
         }

         if ( (i % specificColumns == 0) && i > 0 ) {
            replace++;
         }

         if ( testingValues[k][j] != -1 ) {
            testingValues[k][j] = replace;
         }

         j++;
         
      }

   }

   barrier(CLK_GLOBAL_MEM_FENCE);

}
