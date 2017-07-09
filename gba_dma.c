/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic Functions Library for DMA Transfers */
/*                                                                           */
/*****************************************************************************/

#include "gba_dma.h"

/*****************************************************************************/

/*
 * Static library variables
 */

dma_struct gba_dmas[4]=
{
  /* DMA 0 */
  {
    (gba_word32) &(gba_dma_regs.DMA0SAD),     /* Source address */
    (gba_word32) &(gba_dma_regs.DMA0DAD),     /* Destination address */
    (gba_word16) &(gba_dma_regs.DMA0CNT_L),   /* Block number to be transmitted */
    (gba_word16) &(gba_dma_regs.DMA0CNT_H)    /* Control record */
  },

  /* DMA 1 */
  {
    (gba_word32) &(gba_dma_regs.DMA1SAD),     /* Source address */
    (gba_word32) &(gba_dma_regs.DMA1DAD),     /* Destination address */
    (gba_word16) &(gba_dma_regs.DMA1CNT_L),   /* Block number to be transmitted */
    (gba_word16) &(gba_dma_regs.DMA1CNT_H)    /* Control record */
  },

  /* DMA 2 */
  {
    (gba_word32) &(gba_dma_regs.DMA2SAD),     /* Source address */
    (gba_word32) &(gba_dma_regs.DMA2DAD),     /* Destination address */
    (gba_word16) &(gba_dma_regs.DMA2CNT_L),   /* Block number to be transmitted */
    (gba_word16) &(gba_dma_regs.DMA2CNT_H)    /* Control record */
  },

  /* DMA 3 */
  {
    (gba_word32) &(gba_dma_regs.DMA3SAD),     /* Source address */
    (gba_word32) &(gba_dma_regs.DMA3DAD),     /* Destination address */
    (gba_word16) &(gba_dma_regs.DMA3CNT_L),   /* Block number to be transmitted */
    (gba_word16) &(gba_dma_regs.DMA3CNT_H)    /* Control record */
  }
};

/*****************************************************************************/

/*
 * Enables the DMA channel.
 * @param channel Channel number
 */
inline void gba_dma_enable(u_char channel)
{
  *(gba_dmas[channel].DMACNT_H) |= DMA_ENABLE;
}

/*****************************************************************************/

/*
 * Disables the DMA channel
 * @param channel Channel number
 */
inline void gba_dma_disable(u_char channel)
{
  *(gba_dmas[channel].DMACNT_H) &= ~DMA_ENABLE;
}

/*****************************************************************************/

/*
 * Copies an area of ​​memory through the DMA3 channel in blocks of 32 bits, for
 * Therefore, ADDRESSES SHOULD BE ALIGNED TO A 32 BIT BORDER
 * @param channel Channel number
 * @param dest Destination address
 * @param source Source address
 * @param size Size in bytes of the transfer
 */
inline void gba_dma_memcpy(void* dest, const void* source, u_int size)
{
  short shadow = *(gba_dmas[DMA3].DMACNT_H);
  
  *(gba_dmas[DMA3].DMASAD)   = (int) source;
  *(gba_dmas[DMA3].DMADAD)   = (int) dest;
  *(gba_dmas[DMA3].DMACNT_L) = (u_short) (size >> 2);
  *(gba_dmas[DMA3].DMACNT_H) = DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32;
  
  *(gba_dmas[DMA3].DMACNT_H) = shadow;
}

/*****************************************************************************/

/*
 * Initializes a direct sound transfer.
 * @param channel (only DMA1 and DMA2)
 * @param FIFO Cola FIFO channel sound
 * @param Buffer buffer with sound samples
 */
inline void gba_dma_sound_init(u_char channel, void* fifo, const void* buffer)
{
  *(gba_dmas[channel].DMASAD) = (int)buffer;
  *(gba_dmas[channel].DMADAD) = (int)fifo;
  *(gba_dmas[channel].DMACNT_H) = DMA_DEST_FIXED | DMA_SOURCE_INCREMENT |
                                  DMA_REPEAT | DMA_32 | DMA_TIMING_DSOUND;
}

/*****************************************************************************/

