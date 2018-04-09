#include <stdio.h>    /* for printf statements when debugging */
#include <stdlib.h>   /* for malloc() and free() */
#include "mem.h"

/*
  Physical memory array. This is a static global array for all
  functions in this file.  An element in the array with a value of
  zero represents a free unit of memory.
*/
static dur_t* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is
 a static global variable used by functions in this file.
 */
static int mem_size;


/*
 The last_placement_position variable contains the end position of the
 last allocated unit used by the next fit placement algorithm.
 */
static int last_placement_position = 0;


int is_enough_space(int size, int pos){

  for(int i = 0; i<size; i = i + 1){
    if(memory[pos+i] != 0 || pos+i >= mem_size){
      return 0;
    }
  }
  return 1;


}



int getBlockSize(int pos){

  int count = 0;
 // printf("BLOCK: ");
  for(int i = pos; i < mem_size && memory[i] == 0; i = i + 1){
    count++;
  //  printf("0");
  }
  //printf(", SIZE AT %d = %d\n", pos,count);
  return count;


}


int assign(int size, dur_t duration, int pos){
  for(int i = 0; i < size; i++){
    if(memory[pos+i] != 0){
      return 0;
    }
    memory[pos+i] = duration;
  }
  last_placement_position = pos;
  return 1;
}



/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
 */


int mem_allocate(mem_strats_t strategy, int size, dur_t duration)
{
  int numProbes =1;
  int inBlock = 0;
  if(strategy == FIRSTFIT){
    for(int i = 0; i < mem_size; i=i+1){
      if(inBlock == 0 && memory[i] == 0){
       if(is_enough_space(size, i) == 1){
          if(assign(size,duration,i) ==0){
            return 2;
          }else{
            return numProbes;
          }
        }else{
          inBlock = 1;
          numProbes++;
        }

      }else if(memory[i] != 0){
        inBlock = 0;
      }
    }

  }else if(strategy == NEXTFIT){
    for(int i = last_placement_position; i < (mem_size + last_placement_position); i++){
      if(inBlock == 0 && memory[i%mem_size] == 0){
       if(is_enough_space(size, (i%mem_size)) == 1){
          if(assign(size,duration,(i%mem_size)) ==0){
            return 2;
          }else{
            return numProbes;
          }
        }else{
          inBlock = 1;
          numProbes++;
        }

      }else if(memory[i%mem_size] != 0){
        inBlock = 0;
      }

    }
  }else if(strategy == BESTFIT){
    int bestSize = -1;
    int bestPos = -1;
    int fsize = 0;
    for(int i = 0; i < mem_size; i++){
      if(memory[i] == 0 && inBlock == 0){
      	  numProbes++;
          if(is_enough_space(size, i) == 1){
            if(getBlockSize(i) < bestSize || bestSize == -1){
              bestPos = i;
              bestSize = getBlockSize(i);
              inBlock = 1;

            }
        }
      }else if (inBlock == 1 && memory[i] != 0){
        inBlock = 0;
      }
    }
  
    if(bestPos != -1){
      assign(size, duration, bestPos);
      return numProbes;
    }
  }
  
  return -1;
}






 

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.  NOTE: when a
  memory cell is decremented to zero, it becomes "unallocated".
 */
int mem_single_time_unit_transpired()
{
  for(int i = 0; i< mem_size; i = i + 1){
    if(memory[i] != 0){
      --memory[i];
    }
  }

}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size)
{
  int fcount = 0;
  int inBlock =0;
  int fsize = 0;
  for(int i = 0; i < mem_size; i++){
    if(inBlock == 0 && memory[i] ==0){
      inBlock =1;
      fsize++;
    }else if(inBlock == 1 && memory[i] == 0){
      fsize++;
    }else if(inBlock == 1 && memory[i] != 0){

      if(fsize <= frag_size){
        fcount++;
      }
      inBlock = 0;
      fsize = 0;
      }
    }

    //Makes sure to count block that extends to end of memory
    if(inBlock == 1 && fsize >= frag_size){
      fcount++;
    }
    return fcount;
  }


/*
  Set the value of zero to all entries of memory.
 */
void mem_clear()
{
  for(int i = 0; i< mem_size; i++){
    memory[i] = 0;
  }
  last_placement_position = 0;
}

/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init(int size)
{
  memory = malloc(sizeof(dur_t)*size);
  mem_size = size;
}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free()
{
  free(memory);
}

/*
  Print memory for testing/debugging purposes.  This is an optional
  routine to write, but highly recommended!  You will need to test and
  debug your allocation algorithms.  Calling this routine in your
  main() after every allocation (successful or not) will help in this
  endeavor.  NOTE: you should print the memory contents in contiguous
  blocks, rather than single units; otherwise, the output will be very
  long.
 */
void mem_print()
{
  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");

}

/*int main(){
  mem_strats_t first = BESTFIT;
  mem_init(30);
  memory[1] = 4;
  mem_allocate(first, 3, 5);
  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");
  mem_allocate(first, 7,2);
  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");
  memory[12] = 9;
  mem_allocate(first ,9,4);
  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");
  mem_single_time_unit_transpired();
  
  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");

  mem_single_time_unit_transpired();

 // mem_allocate(first, 5,9);

  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");

  int k = mem_fragment_count(2);
  printf("%d\n",k);

  memory[25] = 9;
  int p = mem_allocate(first, 3,6);
  printf("%d\n", p);


  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");

  
  mem_allocate(first, 4,8);

  for(int i = 0; i<mem_size;i++){
    printf("%d", memory[i]);
  }
  printf("\n");
  


} */
