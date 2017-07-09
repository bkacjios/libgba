/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants for the GBA timers */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef timer_h
#define timer_h

#include    "std_c.h"

/*****************************************************************************/
/* Interface for console timers */
/*****************************************************************************/

typedef struct
{
  short TM0CNT_L;      /* Timer 0 counter / reload */
  short TM0CNT_H;      /* Timer 0 control */
  short TM1CNT_L;      /* Timer 1 counter / reload */
  short TM1CNT_H;      /* Timer 1 control */
  short TM2CNT_L;      /* Timer 2 counter / reload */
  short TM2CNT_H;      /* Timer 2 control */
  short TM3CNT_L;      /* Timer 3 counter / reload */
  short TM3CNT_H;      /* Timer 3 control */
} gba_timer_regs_struct;

extern volatile gba_timer_regs_struct gba_timer_regs;


/*****************************************************************************/
/* TMxCNT_H: Timer Control */
/*****************************************************************************/

/*
 *  Pre-scaling the timer frequency
 */
#define TIMER_FREQ_1               ((short) 0)         /* Each cycle of the system clock */
#define TIMER_FREQ_64              ((short) 1)         /* Every 64 cycles */
#define TIMER_FREQ_256             ((short) 2)         /* Every 256 cycles */
#define TIMER_FREQ_1024            ((short) 3)         /* Every 1024 cycles */

/*
 *  To chain the timers. In this case the pre-scaling factor is ignored.
 */
#define TIMER_CASCADE              ((short) (1 << 2))
  
/*
 *  Generation of an interruption when the timer exceeds the limit value
 */
#define TIMER_INT                  ((short) (1 << 6))

/*
 *  Enabling the timer
 */
#define TIMER_ENABLE               ((short) (1 << 7))

/*****************************************************************************/

#endif /* timer_h */

