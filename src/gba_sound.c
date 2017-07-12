/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic sound function library */
/*                                                                           */
/*****************************************************************************/

#include "gba_sound.h"
#include "gba_irq.h"
#include "gba_timer.h"
#include "gba_dma.h"
#include "gba_bios.h"

/*****************************************************************************/

/*
 * Static library variables
 */

sound_ds_struct gba_sound_dss[2]=
{
  /* SOUND_DSA */
  {
    2,         /* SOUNDCNT_H: Shift to Volume */
    8,         /* SOUNDCNT_H: displacement for lines L and R */
    10,        /* SOUNDCNT_H: scroll to the timer */
    11,        /* SOUNDCNT_H: scroll to the reset */
    (gba_word32) &(gba_sound_regs.FIFO_A)  /* Pointer to the FIFO queue */
  },

  /* SOUND_DSB */
  {
    3,         /* SOUNDCNT_H: Shift to Volume */
    12,        /* SOUNDCNT_H: displacement for lines L and R */
    14,        /* SOUNDCNT_H: scroll to the timer */
    15,        /* SOUNDCNT_H: scroll to the reset */
    (gba_word32) &(gba_sound_regs.FIFO_B) /* Pointer to the FIFO queue */
  }
};

/*****************************************************************************/

/*
 * Enable sound driver
 */
inline void gba_sound_enable()
{
  gba_sound_regs.SOUNDCNT_X |= SOUND_ENABLE;
}

/*****************************************************************************/

/*
 * Disable sound driver
 */
inline void gba_sound_disable()
{
  gba_sound_regs.SOUNDCNT_X &= ~SOUND_ENABLE;
}

/*****************************************************************************/

/*
 * Initializes the timer to be used on a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param timer Timer (TIMER0 or TIMER1 only)
 */
inline void gba_sound_ds_set_timer(u_char channel, u_char timer)
{
  gba_sound_regs.SOUNDCNT_H |= (timer & 0x01) << gba_sound_dss[channel].timer_offset;
}

/*****************************************************************************/

/*
 * Initializes the volume of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param volume Volume (SOUND_DS_OUTPUT_RATIO_50 or SOUND_DS_OUTPUT_RATIO_100)
 */
inline void gba_sound_ds_set_volume(u_char channel, u_char volume)
{
  gba_sound_regs.SOUNDCNT_H |= (volume & 0x01) << gba_sound_dss[channel].volume_offset;
}

/*****************************************************************************/

/*
 * Initializes the output of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 * @param output Output Type (SOUND_DS_OUTPUT_TO_RIGHT,
 *                SOUND_DS_OUTPUT_TO_LEFT or SOUND_DS_OUTPUT_TO_BOTH)
 */
inline void gba_sound_ds_set_output(u_char channel, u_char output)
{
  gba_sound_regs.SOUNDCNT_H |= (output & 0x03) << gba_sound_dss[channel].lr_offset;
}

/*****************************************************************************/

/*
 * Resets the queue of a direct sound channel.
 * @param channel Channel number (SOUND_DSA or SOUND_DSB)
 */
inline void gba_sound_ds_fifo_reset(u_char channel)
{
  gba_sound_regs.SOUNDCNT_H |= 1 << gba_sound_dss[channel].reset_offset;
}

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
void gba_sound_ds_play(u_char channel, const void* samples, u_short size, u_short rate)
{
  u_short clocks_per_sample;

  gba_dma_sound_init(DMA1, (void *)gba_sound_dss[channel].FIFO, samples);

  /* Reset the FIFO queue */
  gba_sound_ds_fifo_reset(channel);

  /* Set the timer to be used to manage the frequency */
  gba_sound_ds_set_timer(channel, TIMER0);

  /* We calculate the number of cycles that must pass between each sample */
  clocks_per_sample = gba_div(MCK, rate);

  /* We calculate the initial value for the timer */
  gba_timer_init(TIMER0, TIMER_FREQ_1, TIMER_OVERFLOW - clocks_per_sample);

  /*
   * We program the timer that will count the number of samples played
   * To generate an interrupt when the last sample is played
   */
  gba_timer_init(TIMER1, TIMER_CASCADE | TIMER_INT, TIMER_OVERFLOW - size);
  
  /* We enable the attention of the interruption */
  gba_irq_enable(IRQ_TIMER1);
  gba_irq_set(IRQ_TIMER1, gba_sound_ds_stop);
  
  /* We enable the sound driver */
  gba_sound_enable();

  /* We enable the DMA1 channel */
  gba_dma_enable(DMA1);

  /* We enable timers */
  gba_timer_start(TIMER0);
  gba_timer_start(TIMER1);
}

/*****************************************************************************/

/*
 * Ends sound playback
 */
inline void gba_sound_ds_stop()
{
  gba_timer_stop(TIMER0);
  gba_dma_disable(DMA1);
  gba_sound_disable();
}

/*****************************************************************************/

