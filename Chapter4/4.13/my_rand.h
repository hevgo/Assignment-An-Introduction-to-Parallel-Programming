/* File:     my_rand.h
 * Purpose:  Header file for my_rand.c, which implements a simple
 *           pseudo-random number generator.
 *
 * IPP:  Not discussed, but needed by the multithreaded linked list programs
 *       discussed in Section 4.9.2-4.9.4 (pp. 183-190).
 */
#ifndef _MY_RAND_H_
#define _MY_RAND_H_

#include <stdio.h>
#include <stdlib.h>
#include "my_rand.h"

#define MR_MULTIPLIER 279470273 
#define MR_INCREMENT 0
#define MR_MODULUS 4294967291U
#define MR_DIVISOR ((double) 4294967291U)

unsigned my_rand(unsigned* a_p);
double my_drand(unsigned* a_p);


/* Function:      my_rand
 * In/out arg:    seed_p
 * Return value:  A new pseudo-random unsigned int in the range
 *                0 - MR_MODULUS
 *
 * Notes:  
 * 1.  This is a slightly modified version of the generator in the 
 *     Wikipedia article "Linear congruential generator"
 * 2.  The seed_p argument stores the "state" for the next call to
 *     the function.
 */
unsigned my_rand(unsigned* seed_p) {
   long long z = *seed_p;
   z *= MR_MULTIPLIER; 
// z += MR_INCREMENT;
   z %= MR_MODULUS;
   *seed_p = z;
   return *seed_p;
}

/* Function:      my_drand
 * In/out arg:    seed_p
 * Return value:  A new pseudo-random double in the range 0 - 1
 */
double my_drand(unsigned* seed_p) {
   unsigned x = my_rand(seed_p);
   double y = x/MR_DIVISOR;
   return y;
}


#endif
