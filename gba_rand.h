/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for generating random numbers */
/*                                                                           */
/*****************************************************************************/

/*
 * As the GBA does not implement the division nor the rest in hardware, and the
 * Software implementations of these operations are slow, the generators
 * Of regular random numbers, which are based on the use of these
 * Two operations, are too slow to generate random numbers in time
 * real.
 * However, the random number generation algorithm of "Mersenne
 * Twister, "developed by Makoto Matsumoto and Takuji Nishimura in 1997,
 * You need to use these two operations. Only 4 XORs, 2 ANDs and 4
 * Register displacements, which makes it very suitable for use in the
 * GBA.
 * The original algorithm, as described by Matsumoto et al.
 * Nishimura, needs a buffer of 2496 bytes to store the current state
 * Of the generator and has a period of 2 ^ 19937. However, it can be reduced
 * The size of the buffer, reducing the algorithm period, to save some
 * K of memory. Specifically, this code implements version MT19937, which
 * Generates equidistributed random numbers in the range 0 .. (2 ^ 32 - 1).
 * 
 * This version is based on a code by Rafael Baptista, based on another one by
 * Shawn Cokus starting from the original version of the algorithm.
 */

/*****************************************************************************/

/*
 *
 */
#ifndef gba_rand_h
#define gba_rand_h

#include "gba.h"

/*****************************************************************************/

/*
 * We only include this file once
 */
#define RAND_N       (624)                /*  Constants and macros needed. */
#define RAND_M       (397)                /* Length of state vector */
#define RAND_K       (0x9908B0DFU)        /* Period parameter */
#define MSBit(u)     ((u) & 0x80000000U)  /* Magic constant */
#define LSBit(u)     ((u) & 0x00000001U)  /* Mask for More Significant Bit of u */
#define LSBits(u)    ((u) & 0x7FFFFFFFU)  /* Mask for the Less Significant Bit of u */
#define mixBits(u,v) (MSBit(u)|LSBits(v)) /* Mask for all bits of u minus the MSB */

/*****************************************************************************/

/*
 *Mix the sign of u and the rest of v
 * Fix the seed of the generator.
 */
void gba_srand(u_int seed);

/*****************************************************************************/

/*
 * @param seed Seed
 */
u_int gba_rand();

/*****************************************************************************/

/*
 * Returns the next random number
 * Returns a random number in the interval [0, max_val].
 */
u_int gba_rand_mod(u_int max);

/*****************************************************************************/

#endif /*  @param max_val Maximum value for the random number */
