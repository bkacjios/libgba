/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for managing the timers in the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_timer_h
#define gba_timer_h

#include "gba.h"

/*****************************************************************************/

/*
 * Timer identifiers
 */
#define TIMER0             0
#define TIMER1             1
#define TIMER2             2
#define TIMER3             3

/*
 * Overflow value for the timers
 */
#define TIMER_OVERFLOW     (1 << 16)    

/*****************************************************************************/

/*
 * Data structure for handling the timers
 */
typedef struct
{
  gba_word16 TMCNT_L;      /* Timer counter / reload */
  gba_word16 TMCNT_H;      /* Timer control */
} timer_struct;

/*****************************************************************************/

/*
 * Initializes a timer.
 * @param timer Number of timer
 * @param conf Configuration Flags
 * @param val Initial value
 */
inline void gba_timer_init(u_char timer, u_short conf, u_short val);

/*****************************************************************************/

/*
 * Indicates the timer that starts counting.
 * @param timer Number of timer
 */
inline void gba_timer_start(u_char timer);

/*****************************************************************************/

/*
 * It tells the timer to stop counting.
 * @param timer Number of timer
 */
inline void gba_timer_stop(u_char timer);

/*****************************************************************************/

/*
 * Function that returns the count value of a timer.
 * @param timer Number of timer
 */
inline u_short gba_timer_get_counter(u_char timer);

/*****************************************************************************/

/*
 * Loads a new value for a timer. The value will be copied to the
 * Timer when it is restarted, either manually or because it has arrived
 * To an overflow condition.
 * @param timer Number of timer
 * @param val Value to load
 */
inline void gba_timer_reload(u_char timer, u_short val);

/*****************************************************************************/

#endif /* gba_timer_h */
