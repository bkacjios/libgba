/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants for the GBA DMA driver */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef dma_H
#define dma_H

#include    "std_c.h"

/*****************************************************************************/
/* Console DMA driver interface */
/*****************************************************************************/

typedef struct
{
  int   DMA0SAD;       /* DMA 0 source address */
  int   DMA0DAD;       /* DMA 0 destination address */
  short DMA0CNT_L;     /* DMA 0 word count */
  short DMA0CNT_H;     /* DMA 0 control */
  int   DMA1SAD;       /* DMA 1 source address */
  int   DMA1DAD;       /* DMA 1 destination address */
  short DMA1CNT_L;     /* DMA 1 word count */
  short DMA1CNT_H;     /* DMA 1 control */
  int   DMA2SAD;       /* DMA 2 source address */
  int   DMA2DAD;       /* DMA 2 destination address */
  short DMA2CNT_L;     /* DMA 2 word count */
  short DMA2CNT_H;     /* DMA 2 control */
  int   DMA3SAD;       /* DMA 3 source address */
  int   DMA3DAD;       /* DMA 3 destination address */
  short DMA3CNT_L;     /* DMA 3 word count */
  short DMA3CNT_H;     /* DMA 3 control */
} gba_dma_regs_struct;

extern volatile gba_dma_regs_struct gba_dma_regs;


/*****************************************************************************/
/* DMAxCNT_H: DMA Control */
/*****************************************************************************/

/*
 * Control of destination address
 */
#define DMA_DEST_INCREMENT         ((short) (0 << 5))  /* Increase */
#define DMA_DEST_DECREMENT         ((short) (1 << 5))  /* Decrement */
#define DMA_DEST_FIXED             ((short) (2 << 5))  /* Fixed */
#define DMA_DEST_RELOAD            ((short) (3 << 5))  /* Increase / Reload */

/*
 * Control of the origin address
 */
#define DMA_SOURCE_INCREMENT       ((short) (0 << 7))  /* Increase */
#define DMA_SOURCE_DECREMENT       ((short) (1 << 7))  /* Decrement */
#define DMA_SOURCE_FIXED           ((short) (2 << 7))  /* Fixed */

/*
 * Repeat Transfer
 */
#define DMA_REPEAT                 ((short) (1 << 9))  /* Enable repeat */
 
/*
 * Type of transfer
 */
#define DMA_16                     ((short) (0 << 10)) /* Transfer in 16-bit units */
#define DMA_32                     ((short) (1 << 10)) /* Transfer in 32-bit drives */

/*
 * DMA to / from the cartridge (Only for channel 3)
 */
#define DMA_GAME_PACK_DRQ          ((short) (1 << 11))

/*
 * Timing of the start of the transfer
 */
#define DMA_TIMING_IMMEDIATE       ((short) (0 << 12))
#define DMA_TIMING_VBLANK          ((short) (1 << 12))
#define DMA_TIMING_HBLANK          ((short) (2 << 12))
#define DMA_TIMING_SYNC_TO_DISPLAY ((short) (3 << 12)) /* Only for DMA3 */
#define DMA_TIMING_DSOUND          ((short) (3 << 12)) /* Only for DMA1 and DMA2 */

/*
 * Generating an interrupt when the copy is finished
 */
#define DMA_INT                    ((short) (1 << 14))

/*
 * DMA Enablement
 */
#define DMA_ENABLE                 ((short) (1 << 15))

/*****************************************************************************/

#endif /* dma_H */
