/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic sound function library */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_sound_h
#define gba_sound_h

#include "gba.h"
#include "gba_timer.h"

/*****************************************************************************/

/*
 * Identifiers of the direct sound channels
 */
#define SOUND_DSA                      0
#define SOUND_DSB                      1

/*
 * Constants for selecting the volume of the direct sound channels
 */
#define SOUND_DS_OUTPUT_RATIO_50       0       /*  fifty% */
#define SOUND_DS_OUTPUT_RATIO_100      1       /* 100% */

/*
 * Constants to select output type.
 */
#define SOUND_DS_OUTPUT_TO_RIGHT  (1 << 0)
#define SOUND_DS_OUTPUT_TO_LEFT   (1 << 1)
#define SOUND_DS_OUTPUT_TO_BOTH   (SOUND_DS_OUTPUT_TO_LEFT | SOUND_DS_OUTPUT_TO_RIGHT)


/*****************************************************************************/

/*
 * Data structure to handle direct sound channels (DSA and DSB)
 */
typedef struct
{                              /* Displacements in SOUNDCNT_H */
  u_char     volume_offset;    /* volume */
  u_char     lr_offset;        /* Left and right lines */
  u_char     timer_offset;     /* Timer */
  u_char     reset_offset;     /* Reset */
  gba_word32 FIFO;             /* Pointer to the FIFO queue */
} sound_ds_struct;

/*****************************************************************************/

/*
 * Enable sound driver
 */
extern void gba_sound_enable();

/*****************************************************************************/

/*
 * Disable sound driver
 */
extern void gba_sound_disable();

/*****************************************************************************/

/*
 * Initializes the timer to be used on a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param timer Timer (TIMER0 or TIMER1 only)
 */
extern void gba_sound_ds_set_timer(u_char channel, u_char timer);

/*****************************************************************************/

/*
 * Initializes the volume of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param volume Volume (SOUND_DS_OUTPUT_RATIO_50 or SOUND_DS_OUTPUT_RATIO_100)
 */
extern void gba_sound_ds_set_volume(u_char channel, u_char volume);

/*****************************************************************************/

/*
 * Initializes the output of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param output Output Type (SOUND_DS_OUTPUT_TO_RIGHT,
 *                SOUND_DS_OUTPUT_TO_LEFT or SOUND_DS_OUTPUT_TO_BOTH)
 */
extern void gba_sound_ds_set_output(u_char channel, u_char output);

/*****************************************************************************/

/*
 * Resets the queue of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 */
extern void gba_sound_ds_fifo_reset(u_char channel);

/*****************************************************************************/

/*
 * Plays a sample sound.
 * Use the timers TIMER0 and TIMER1 for playback. TIMER0 measures the
 * Time between samples and TIMER1 counts the number of samples that go
 * Reproducing.
 * It also programs an interrupt in TIMER1 that will be
 * Finished playback and will be answered by the gba_sound_ds_stop
 * @param channel Direct sound channel (SOUND_DSA or SOUND_DSB)
 * @param samples Buffer with samples
 * @param size Buffer size in bytes
 * @param rate Sampling frequency
 */
extern void gba_sound_ds_play(u_char channel, const void* samples, u_short size, u_short rate);

/*****************************************************************************/

/*
 * Ends sound playback
 */
extern void gba_sound_ds_stop();

/*****************************************************************************/

#endif /* gba_sound_h */
