/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for managing the timers in the GBA */
/*                                                                           */
/*****************************************************************************/

#include "gba_timer.h"

/*****************************************************************************/

/*
 * Static library variables
 */

timer_struct gba_timers[4]=
{
  /* Timer 0 */
  {
    (gba_word16) &(gba_timer_regs.TM0CNT_L),  /* Timer counter / reload */
    (gba_word16) &(gba_timer_regs.TM0CNT_H)   /* Timer control */
  },

  /* Timer 1 */
  {
    (gba_word16) &(gba_timer_regs.TM1CNT_L),  /* Timer counter / reload */
    (gba_word16) &(gba_timer_regs.TM1CNT_H)   /* Timer control */
  },

  /* Timer 2 */
  {
    (gba_word16) &(gba_timer_regs.TM2CNT_L),  /* Timer counter / reload */
    (gba_word16) &(gba_timer_regs.TM2CNT_H)   /* Timer control */
  },

  /* Timer 3 */
  {
    (gba_word16) &(gba_timer_regs.TM3CNT_L),  /* Timer counter / reload */
    (gba_word16) &(gba_timer_regs.TM3CNT_H)   /* Timer control */
  }
};

/*****************************************************************************/

/*
 * Initializes a timer.
 * @param timer Number of timer
 * @param conf Configuration Flags
 * @param val Initial value
 */
inline void gba_timer_init(u_char timer, u_short conf, u_short val)
{
  *(gba_timers[timer].TMCNT_L) = val;
  *(gba_timers[timer].TMCNT_H) = conf;
}

/*****************************************************************************/

/*
 * Indicates the timer that starts counting.
 * @param timer Number of timer
 */
inline void gba_timer_start(u_char timer)
{
  *(gba_timers[timer].TMCNT_H) |= TIMER_ENABLE;
}

/*****************************************************************************/

/*
 * It tells the timer to stop counting.
 * @param timer Number of timer
 */
inline void gba_timer_stop(u_char timer)
{
  *(gba_timers[timer].TMCNT_H) &= ~TIMER_ENABLE;
}

/*****************************************************************************/

/*
 * Function that returns the count value of a timer.
 * @param timer Number of timer
 */
inline u_short gba_timer_get_counter(u_char timer)
{
  return *(gba_timers[timer].TMCNT_L);
}

/*****************************************************************************/

/*
 * Loads a new value for a timer. The value will be copied to the
 * Timer when it is restarted, either manually or because it has arrived
 * To an overflow condition.
 * @param timer Number of timer
 * @param val Value to load
 */
inline void gba_timer_reload(u_char timer, u_short val)
{
  *(gba_timers[timer].TMCNT_L) = val;
}

/*****************************************************************************/

