/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants for the GBA sound driver */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef sound_h
#define sound_h

#include    "std_c.h"

/*****************************************************************************/
/* Interface for console sound driver */
/*****************************************************************************/

typedef struct
{
  short SOUND1CNT_L;   /* FM synthesis channel 1 sweep */
  short SOUND1CNT_H;   /* FM synthesis channel 1 duty / length / envelope */
  short SOUND1CNT_X;   /* FM synthesis channel 1 frequency / control */
  short filler1;       /* Not used */
  short SOUND2CNT_L;   /* FM synthesis channel 2 duty / length / envelope */
  short filler2;       /* Not used */
  short SOUND2CNT_H;   /* FM synthesis channel 2 frequency / control */
  short filler3;       /* Not used */
  short SOUND3CNT_L;   /* FM synthesis channel 3 stop / wave ram select */
  short SOUND3CNT_H;   /* FM synthesis channel 3 length / volume */
  short SOUND3CNT_X;   /* FM synthesis channel 3 frequency / control */
  short filler4;       /* Not used */
  short SOUND4CNT_L;   /* FM synthesis channel 4 duty / length / envelope */
  short filler5;       /* Not used */
  short SOUND4CNT_H;   /* FM synthesis channel 4 frequency / control */
  short filler6;       /* Not used */
  short SOUNDCNT_L;    /* Control stereo / volume / enable */
  short SOUNDCNT_H;    /* Control mixing / dma control */
  short SOUNDCNT_X;    /* Control sound on / off */
  short filler7;       /* Not used */
  short SOUNDBIAS;     /* Sound pwm control */
  short filler8[3];    /* Not used */
  short WAVE_RAM[8];   /* FM synthesis channel 3 wave pattern ram */
  int   FIFO_A;        /* Direct sound channel to fifo, date 0-3 */
  int   FIFO_B;        /* Direct sound channel b fifo, date 0-3 */
} gba_sound_regs_struct;

extern volatile gba_sound_regs_struct gba_sound_regs;


/*****************************************************************************/
/* SOUNDCNT_H: Control Mixing / DMA Control */
/*****************************************************************************/

/*
 * Volume percentage for channels 1-4
 */
#define SOUND_FM_OUTPUT_RATIO_25   ((short) 0)         /*  25% */
#define SOUND_FM_OUTPUT_RATIO_50   ((short) 1)         /*  fifty% */
#define SOUND_FM_OUTPUT_RATIO_100  ((short) 2)         /* 100% */

/*
 * Volume percentage for channels A-B
 */
#define SOUND_DSA_OUTPUT_RATIO_50  ((short) (0 << 2))  /*  50% for channel A */
#define SOUND_DSA_OUTPUT_RATIO_100 ((short) (1 << 2))  /* 100% for channel A */
#define SOUND_DSB_OUTPUT_RATIO_50  ((short) (0 << 3))  /*  50% for channel B */
#define SOUND_DSB_OUTPUT_RATIO_100 ((short) (1 << 3))  /* 100% for channel B */

/*
 * Selecting output lines for A-B channels
 */
#define SOUND_DSA_OUTPUT_TO_RIGHT  ((short) (1 << 8))
#define SOUND_DSA_OUTPUT_TO_LEFT   ((short) (1 << 9))
#define SOUND_DSA_OUTPUT_TO_BOTH   (SOUND_DSA_OUTPUT_TO_RIGHT|SOUND_DSA_OUTPUT_TO_LEFT)
#define SOUND_DSB_OUTPUT_TO_RIGHT  ((short) (1 << 12))
#define SOUND_DSB_OUTPUT_TO_LEFT   ((short) (1 << 13))
#define SOUND_DSB_OUTPUT_TO_BOTH   (SOUND_DSB_OUTPUT_TO_RIGHT|SOUND_DSB_OUTPUT_TO_LEFT)

/*
 * Selecting Timers for A-B Channels
 */
#define SOUND_DSA_TIMER0           ((short) (0 << 10))
#define SOUND_DSA_TIMER1           ((short) (1 << 10))
#define SOUND_DSB_TIMER0           ((short) (0 << 14))
#define SOUND_DSB_TIMER1           ((short) (1 << 14))

/*
 * Reset of the FIFO queues of channels A-B
 */
#define SOUND_DSA_FIFO_RESET       ((short) (1 << 11))
#define SOUND_DSB_FIFO_RESET       ((short) (1 << 15))
 
/*****************************************************************************/
/* SOUNDCNT_X: Sound on / off control */
/*****************************************************************************/

/*
 * Flags to check the status of sound channels 1-4
 */
#define SOUND_FM_1                 ((short) (1 << 0))
#define SOUND_FM_2                 ((short) (1 << 1))
#define SOUND_FM_3                 ((short) (1 << 2))
#define SOUND_FM_4                 ((short) (1 << 3))
 
/*
 * Enabling / Disabling the Sound Driver
 */
#define SOUND_ENABLE               ((short) (1 << 7))

/*****************************************************************************/

#endif /* sound_h */
