/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for fixed-point operation */
/*                                                                           */
/* Format used: SEEEEEEEFFFFFFFF */
/*   - 1 sign bit (bit 15), */
/*   - 7 of integer part (bits 8-14), */
/*   - 8 fractional part (bits 0-7). */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef gba_fp_h
#define gba_fp_h

#include "std_c.h"

/*****************************************************************************/

/*
 *  Macros for type conversion from and to fixed_p
 */
#define I2FP(i)               ((fixed_p) (((int) i) << 8))
#define FP2I(fp)              ((signed int) (((fixed_p) fp) >> 8))
#define F2FP(f)               ((fixed_p) (((float) f) * 256))
#define FP2F(fp)              (((float) fp) / 256)

/*****************************************************************************/

/*
 *  Add two fixed-point numbers
 *  @param a First number
 *  @param b Second number
 */
extern fixed_p gba_fp_add(fixed_p a, fixed_p b);

/*****************************************************************************/

/*
 *  Subtract two fixed-point numbers
 *  @param a First number
 *  @param b Second number
 */
extern fixed_p gba_fp_sub(fixed_p a, fixed_p b);

/*****************************************************************************/

/*
 *  Multiply two numbers in fixed comma
 *  @param a First number
 *  @param b Second number
 */
extern fixed_p gba_fp_mult(fixed_p a, fixed_p b);

/*****************************************************************************/

/*
 *  Divide two numbers into fixed commas
 *  @param a First number
 *  @param b Second number
 */
extern fixed_p gba_fp_div(fixed_p a, fixed_p b);

/*****************************************************************************/

/*
 *  Returns the sine of an angle
 *  @param ang angulo
 */
extern fixed_p gba_fp_sin(signed int ang);

/*****************************************************************************/

/*
 *  Returns the cosine of an angle
 *  @param ang angulo
 */
extern fixed_p gba_fp_cos(signed int ang);

/*****************************************************************************/

#endif /* gba_fp_h */
