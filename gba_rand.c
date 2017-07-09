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

#include "gba_rand.h"
#include "gba_bios.h"

/*****************************************************************************/

/*
 * Static generator variables
 */
u_int  gba_rand_state[RAND_N+1]; /* Status Buffer (plus 1 extra not to check limits) */
u_int* gba_rand_next;            /* The next random value is calculated from here */
int    gba_rand_left=-1;         /* Items remaining in the buffer before reloading */

/*****************************************************************************/

/*
 * Fix the seed of the generator.
 * @param seed Seed
 */
void gba_srand(u_int seed)
{
  u_int x = (seed | 0x1) & 0xFFFFFFFF;
  u_int* s = gba_rand_state;
  u_int j = RAND_N;
  gba_rand_left = 0;
  *s++ = x;
   
  while (--j)
  {
    x *= 69069;
    *s = ( x & 0xFFFFFFFF );
    s++;
  }
}

/*****************************************************************************/

/*
 * Buffer Recharge
 */
u_int gba_rand_reload_buffer()
{
  u_int* p0 = gba_rand_state;
  u_int* p2 = gba_rand_state + 2;
  u_int* pM = gba_rand_state + RAND_M;
  u_int s0;
  u_int s1;
  int j;
   
  /* If no seed has been fixed, it is now fixed */
  if (gba_rand_left < -1)
    gba_srand(4357U);
   
  gba_rand_left = RAND_N-1;
  gba_rand_next = &(gba_rand_state[1]);
   
  for(s0 = gba_rand_state[0], s1 = gba_rand_state[1], j = (RAND_N-RAND_M+1) ; --j ; s0 = s1, s1 = *p2++)
    *p0++ = ( *pM++ ^ (mixBits(s0, s1) >> 1) ^ (LSBit(s1) ? RAND_K : 0));
   
  for(pM = gba_rand_state, j = RAND_M ; --j ; s0 = s1, s1 = *p2++)
    *p0++ = ( *pM++ ^ ( mixBits(s0, s1) >> 1 ) ^ ( LSBit(s1) ? RAND_K : 0 ));
   
  s1 = gba_rand_state[0];
  *p0 = ( *pM ^ ( mixBits( s0, s1 ) >> 1 ) ^ ( LSBit( s1 ) ? RAND_K : 0 ));
  s1 ^= ( s1 >> 11 );
  s1 ^= ( s1 <<  7 ) & 0x9D2C5680U;
  s1 ^= ( s1 << 15 ) & 0xEFC60000U;
  
  return ( s1 ^ ( s1 >> 18 ));
}

/*****************************************************************************/

/*
 * Returns the next random number
 */
u_int gba_rand()
{
  u_int y;
   
  gba_rand_left--;
  if ( gba_rand_left < 0 ) 
    return gba_rand_reload_buffer();
   
  y  = *gba_rand_next;
  gba_rand_next++;
  y ^= ( y >> 11 );
  y ^= ( y <<  7 ) & 0x9D2C5680;
  y ^= ( y << 15 ) & 0xEFC60000;
  y ^= ( y >> 18 );
  
  return y;
}

/*****************************************************************************/

/*
 * Returns a random number in the interval [0, max_val].
 * @param max_val Maximum value for the random number
 */
u_int gba_rand_mod(u_int max)
{
  // If max is 0, do not hang the console
  if (max==0)
    return max;

  signed int n = gba_rand();

  if (n<=0 || n>= max)
    n = gba_mod(n, max);

  return (n<0?-n:n);
}

/*****************************************************************************/

