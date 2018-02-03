/*
Coursera HW/SW Interface
Lab 4 - Mystery Caches

Mystery Cache Geometries (for you to keep notes):
mystery0:
    block size =
    cache size =
    associativity =
mystery1:
    block size =
    cache size =
    associativity =
mystery2:
    block size =
    cache size =
    associativity =
mystery3:
    block size =
    cache size =
    associativity =
*/

#include <stdlib.h>
#include <stdio.h>

#include "mystery-cache.h"

/*
 * NOTE: When using access_cache() you do not need to provide a "real"
 * memory addresses. You can use any convenient integer value as a
 * memory address, you should not be able to cause a segmentation
 * fault by providing a memory address out of your programs address
 * space as the argument to access_cache.
 */

/*
   Returns the size (in B) of each block in the cache.
*/
int get_block_size(void) {
  /* YOUR CODE GOES HERE */
  //starting address in access pattern we decide to use in order to discover cache's block sizes
  addr_t address = 0x00;
  //warm up the first cache block by firstly making some memory access resulting obviously in cache miss
  access_cache(address);
  //accumulator of block size
  int block_size = 0;
  //increment block size until second cache miss happens indicating the end of block
  //block size obviously would be equal to the number of hits before that miss
  while(1) {
  	block_size++;
  	//trying to access the next memory address with respect to the previous accessed address
  	if(!access_cache(++address)) {
  		break;
  	}
  }
  return block_size;
}

/*
   Returns the size (in B) of the cache.
*/
int get_cache_size(int size) {
  /* YOUR CODE GOES HERE */
  addr_t start_address = 0x00;
  //assume initial cache size equals to *size* bytes
  int cache_size = size, i;
  while(1) {
    /* before begining the new stage of filling cache lines initially in set by set order 
     * and futher in line by line order we clear cache to be pretty sure all must be fine with 
     * line replacement etc.
     */
    flush_cache();
    /* cache size must be power of 2 as indicated in the task documentation
     * cache_size variable maintains this invariant, so in this loop
     * we load blocks line by line until either current limit being achieved
     * or whole cache being full in which case current size matches actual cache size
     */
    for(i = 0; i < cache_size/size; i++) {
	    access_cache(start_address + i*size);
    }
    /*
     * Access block just after the last loaded block in loop.
     * This results in either simple load new cache line
     * or overwrite block contains starting address in case
     * in prev loop whole cache has been filled (block with
     * start_address would be overwitten because of LRU replacement 
     * policy within cache set)
     */
    access_cache(start_address + i*size);
    /*
     * check result of prev access
     * if hit happens - cache is not full now, so we must double guessing size,
     * otherwise current cache_size is equal to actual cache size
     * so, we exit from while loop
     */ 
    if(!access_cache(start_address)) {
        break;
    } 
    cache_size = cache_size << 1;
  }
  return cache_size;
}

/*
   Returns the associativity of the cache.
*/
int get_cache_assoc(int size) {
  /* YOUR CODE GOES HERE */
  //initial guess of set associativity is 1 (or 0th power of two)
  int assoc = 1, i, power = 0;
  addr_t start_address = 0x00;
  while(1){
    //clear cache to be sure that subsequent associativity guesses have no effect on each other 
    flush_cache();
    //access guessed number of lines in 0th cache set plus additional line in order to
    //give a chance to overwrite cache line containing starting address as least recently used line
    //we "allocate in mind" (power + 1) number of bits in cache line *tag field* (starting from MSB) 
    //to make room for accessing 0th cache set in a manner we access lines in the following loop. 
    //Additional bit (that is additional "+ 1" in (power + 1)) we need in order to have an opportunity 
    //to access a line "just above" under the guessed value of cache associativity in order to decide 
    //whether or not cache set has been filled by the last access by trying to access start_address again
  	for(i = 0; i <= assoc; i++) {
      access_cache(start_address | (i << (31 - power)));
  	}
  	//if start_address no futher be alive in cache, guessed assocoativity matches actual one,
  	//otherwise double guessed value and correspondingly increment associativity's power of two 
  	if(!access_cache(start_address)) {
      break;
  	}
  	assoc <<= 1; 
  	power++;
  }
  return assoc;
}

//// DO NOT CHANGE ANYTHING BELOW THIS POINT
int main(void) {
  int size;
  int assoc;
  int block_size;

  /* The cache needs to be initialized, but the parameters will be
     ignored by the mystery caches, as they are hard coded.  You can
     test your geometry paramter discovery routines by calling
     cache_init() w/ your own size and block size values. */
  cache_init(0,0);

  block_size=get_block_size();
  size=get_cache_size(block_size);
  assoc=get_cache_assoc(size);

  printf("Cache block size: %d bytes\n", block_size);
  printf("Cache size: %d bytes\n", size);
  printf("Cache associativity: %d\n", assoc);

  return EXIT_SUCCESS;
}
