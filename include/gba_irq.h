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

extern void gba_irq_main();

/*
 *  Enable interrupt management
 */
void gba_irq_init();

gba_irq_handler* gba_irq_set(short mask, gba_irq_handler function);

/*****************************************************************************/

/*
 *  Function to enable the attention of concrete interrupts
 *  @param masc Mask to select interrupts
 *  @param handler Function to handle interrupts
 */
void gba_irq_enable(short mask);

/*****************************************************************************/

/*
 *  Disable specific interrupts
 *  @param masc Mask to select interrupts
 */
void gba_irq_disable(short mask);

/*****************************************************************************/

/*
 *  Disable interrupt management
 */
void gba_irq_disable_all();

/*****************************************************************************/

#endif /* gba_irq_h */