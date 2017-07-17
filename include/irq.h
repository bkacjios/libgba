/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants for the interrupt handler */
/* Of the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef irq_h
#define irq_h

#include    "std_c.h"

/*****************************************************************************/
/* Console Interrupt Controller Interface */
/*****************************************************************************/

typedef struct
{
  short IE;            /* Interrupt enable register */
  short IF;            /* Interrupt request flags / irq acknowledge */
  short filler[2];     /* Not used & waitcnt */
  short IME;           /* Interrput master enable register */
} gba_irq_regs_struct;

extern volatile gba_irq_regs_struct gba_irq_regs;

/*
 *  Type to define interrupt handlers.
 */
typedef void (*gba_irq_handler)(void);

/*
 *  Table of pointers to interrupt handlers
 */

struct gba_irq_handler_table{
	gba_irq_handler handler;
	short mask;
};

#define IRQ_HANDLER	*(gba_irq_handler *)(0x03007ffc)

#define IRQ_MAX	15

/*****************************************************************************/
/* IE, IF: Masks for interruptions */
/*****************************************************************************/

/*
 *  Masks for each of the interruptions
 */
#define IRQ_VBLANK                 ((short) (1 << 0))
#define IRQ_HBLANK                 ((short) (1 << 1))
#define IRQ_VCOUNT                 ((short) (1 << 2))
#define IRQ_TIMER0                 ((short) (1 << 3))
#define IRQ_TIMER1                 ((short) (1 << 4))
#define IRQ_TIMER2                 ((short) (1 << 5))
#define IRQ_TIMER3                 ((short) (1 << 6))
#define IRQ_SERIAL                 ((short) (1 << 7))
#define IRQ_DMA0                   ((short) (1 << 8))
#define IRQ_DMA1                   ((short) (1 << 9))
#define IRQ_DMA2                   ((short) (1 << 10))
#define IRQ_DMA3                   ((short) (1 << 11))
#define IRQ_KEYPAD                 ((short) (1 << 12))
#define IRQ_CART                   ((short) (1 << 13))


/*****************************************************************************/
/* IME: Enabling all interrupts */
/*****************************************************************************/

#define IRQ_DISABLE                 ((short) 0)
#define IRQ_ENABLE                  ((short) 1)

/*****************************************************************************/

#endif /* irq_h */
