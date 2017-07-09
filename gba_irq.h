/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling interruptions in the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef gba_irq_h
#define gba_irq_h

#include "gba.h"

/*****************************************************************************/

/*
 *  Enable interrupt management
 */
inline void gba_irq_enable_all();

/*****************************************************************************/

/*
 *  Disable interrupt management
 */
inline void gba_irq_disable_all();

/*****************************************************************************/

/*
 *  Function to enable the attention of concrete interrupts
 *  @param masc Mask to select interrupts
 *  @param handler Function to handle interrupts
 */
void gba_irq_enable(short masc, gba_irq_handler handler);

/*****************************************************************************/

/*
 *  Disable specific interrupts
 *  @param masc Mask to select interrupts
 */
inline void gba_irq_disable(short masc);

/*****************************************************************************/

#endif /* gba_irq_h */
