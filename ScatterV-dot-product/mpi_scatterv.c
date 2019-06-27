#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char *argv[]){

   int numtasks, rank;
   int N;
   int result = 0;
   int *a, *b, *recA, *recB;
   int *sendcounts;
   int *displs;
   int sum = 0, rem = 0;

   double startTime = 0.0, endTime = 0.0, totalTime = 0.0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  

  //init array size and vector value in master process
  if (rank == 0){
    N = 100000;
    a = (int*)malloc(N*sizeof(int));
    b = (int*)malloc(N*sizeof(int));
    
   //integer array (of length group size) specifying the number 
   //of elements to send to each processor
    sendcounts = (int*)malloc(numtasks*sizeof(int));

   //integer array (of length group size). 
   //Entry i specifies the displacement (relative to sendbuf from which to take the outgoing 
   //data to process i
    displs = (int*)malloc(numtasks*sizeof(int));
    //if there any remaining will added to the sendcounts
    rem = N%numtasks;

    for(int i = 0; i < numtasks; i++){
    
     //init number of element to send
     sendcounts[i] = N/numtasks;
     if(rem > 0){
       sendcounts[i]++;
	rem--;
      }

     displs[i] = sum;
     sum+=sendcounts[i];
    }
    
    //init vecotorarray with value
    for(int i = 0; i<N; i++){
         a[i] = b[i]=2;
    }
 }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
 //init new array to recive data
  int sendRank = N/numtasks;
  recA = (int *)malloc((N*N)*sizeof(int));
  recB = (int *)malloc((N*N)*sizeof(int));


  // divide the data among processes bu sendcounts and displs
  MPI_Scatterv(a,sendcounts,displs,MPI_INT,recA,N,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatterv(b,sendcounts,displs,MPI_INT,recB,N,MPI_INT,0,MPI_COMM_WORLD);

  //calculate dot product
  result = 0;
  for(int i = 0; i<sendRank;i++){
       result +=(recA[i] * recB[i]);
 }

  //start the time
  startTime = MPI_Wtime();
 
  //get the result and print it out
  
  MPI_Allreduce(MPI_IN_PLACE, &result,1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  //stop the timer
  endTime = MPI_Wtime();
 //calculate total time taken
  totalTime = endTime - startTime;
   
  printf("Rank: %d: \tresult: %d \tTime = %f\n",rank,result,totalTime);

 MPI_Finalize();
 return 0;

}

