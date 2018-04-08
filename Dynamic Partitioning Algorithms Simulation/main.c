#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include "mem.h"

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  duration of the each simulation run.  The third parameter is the
  number of times to repeat each experiment (ie. number of runs). The
  fourth parameter is a random number seed. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 runs with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
  */

int run(mem_strats_t strat){
  int  size = (rand() + MIN_REQUEST_SIZE)%(MAX_REQUEST_SIZE +1);
  while(size > MAX_REQUEST_SIZE || size < MIN_REQUEST_SIZE){
    size = (rand() + MIN_REQUEST_SIZE)%(MAX_REQUEST_SIZE +1);
  }
  int duration = (rand() + MIN_DURATION)%(MAX_DURATION +1);
  while(duration > MAX_DURATION || duration < MIN_DURATION){
    duration = (rand() + MIN_DURATION)%(MAX_DURATION +1);
  }

  int probes = mem_allocate(strat,size, duration);

  return probes;

}


int main(int argc, char** argv)
{  
  float testq = ((float)299809/(float)300000);
  printf("%f\n", testq);
  int memVal = atoi(argv[1]);
  int numTimes = atoi(argv[2]);
  int numRuns = atoi(argv[3]);
  int seed  = atoi(argv[4]);
  
  srand(seed);
  
  mem_init(memVal);

  mem_strats_t strat = FIRSTFIT;
  float probeTot = 0;
  float failTot = 0; 
  float fragTot =0;
  for(int i = 0; i < numRuns; i++){
    for(int k = 0; k < numTimes; k++){
      int res = run(strat);
      if(res == -1){
        failTot++;
      }else{
        probeTot += (float)res;
      }
      mem_single_time_unit_transpired();
      fragTot += (float)mem_fragment_count(1);    
    }
  }

  float probeAvg = (probeTot / (numTimes * numRuns));
  float failAvg = (failTot / (numTimes * numRuns));
  float fragAvg = (fragTot / (numTimes * numRuns));

  printf("-------Results For First Fit Partitioning-------\n");
  printf("Average # of Probes : %f\n", probeAvg);
  printf("Average # of Failures: %f\n", failAvg);
  printf("Average # of Fragments: %f\n", fragAvg);
  printf("\n");

  mem_clear();

  strat = NEXTFIT;

  probeTot = 0;
  failTot = 0; 
  fragTot =0;
  for(int i = 0; i < numRuns; i++){
    for(int k = 0; k < numTimes; k++){
      int res = run(strat);
      if(res == -1){
        failTot++;
      }else{
        probeTot += (float)res;
      }
      mem_single_time_unit_transpired();
      fragTot += (float)mem_fragment_count(1);    
    }
  }

  probeAvg = (probeTot / (numTimes * numRuns));
  failAvg = (failTot / (numTimes * numRuns));
  fragAvg = (fragTot / (numTimes * numRuns));


  printf("-------Results For Next Fit Partitioning-------\n");
  printf("Average # of Probes : %f\n", probeAvg);
  printf("Average # of Failures: %f\n", failAvg);
  printf("Average # of Fragments: %f\n", fragAvg);

  printf("\n");

  mem_clear();

  strat = BESTFIT;

  probeTot = 0;
  failTot = 0; 
  fragTot =0;
  for(int i = 0; i < numRuns; i++){
    for(int k = 0; k < numTimes; k++){
      int res = run(strat);
      if(res == -1){
        failTot++;
      }else{
        probeTot += (float)res;
      }
      mem_single_time_unit_transpired();
      fragTot += (float)mem_fragment_count(1);    
    }
  }


  probeAvg = (float)(probeTot / (numTimes * numRuns));
  failAvg = (float)(failTot / (numTimes * numRuns));
  fragAvg = (float)(fragTot / (numTimes * numRuns));


  printf("-------Results For Best Fit Partitioning-------\n");
  printf("Average # of Probes : %f\n", probeAvg);
  printf("Average # of Failures: %f\n", failAvg);
  printf("Average # of Fragments: %f\n", fragAvg);

  printf("%f\n", probeTot);


}
