#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char *argv[]){

   int numtasks, rank;
   int N;
   int result = 0;
   int *a, *b, *recA, *recB;
   int sendRank;

  double startTime = 0.0, endTime = 0.0, totalTime = 0.0;
 

 //init MPI  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


 //init array size and vector value in master process
  if (rank == 0){
    N = 100000;
    a = (int*)malloc(N*sizeof(int));
    b = (int*)malloc(N*sizeof(int));

   //init vecotorarray with value
    for(int i = 0; i<N;i++){
         a[i] = b[i]=2;
    }
 }

  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

   //calculate number of element to get by each node, 
   //init new array to recive data
  sendRank = N/numtasks;
  recA = (int *)malloc(sendRank*sizeof(int));
  recB = (int *)malloc(sendRank*sizeof(int));
  
   //start the time
   startTime = MPI_Wtime();

   //MPI_Scatter sends chunks of an array to different processes. 
  MPI_Scatter(a,sendRank,MPI_INT,recA,sendRank,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatter(b,sendRank,MPI_INT,recB,sendRank,MPI_INT,0,MPI_COMM_WORLD);

  //calculate the dot product
  result = 0;
  for(int i = 0; i<sendRank;i++)
        result +=(recA[i] * recB[i]);
  //free(recA);
  //free(recB);

  //stop the timer
  endTime = MPI_Wtime();
 //calculate total time taken
  totalTime = endTime - startTime;
 
  MPI_Allreduce(MPI_IN_PLACE, &result,1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Rank: %d: \tresult: %d \tTime = %f\n",rank,result,totalTime);
 
 MPI_Finalize();
 return 0;

}
