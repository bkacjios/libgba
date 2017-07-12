/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic Functions Library for DMA Transfers */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_dma_h
#define gba_dma_h

#include "gba.h"

/*****************************************************************************/

/*
 * DMA channel identifiers
 */
#define DMA0               0
#define DMA1               1
#define DMA2               2
#define DMA3               3

/*****************************************************************************/

/*
 * Data structure for handling DMA channels
 */
typedef struct
{
  gba_word32 DMASAD;       /* Source address */
  gba_word32 DMADAD;       /* Destination address */
  gba_word16 DMACNT_L;     /* Block number to be transmitted */
  gba_word16 DMACNT_H;     /* Control record */
} dma_struct;

/*****************************************************************************/

/*
 * Enables the DMA channel.
 * @param channel Channel number
 */
extern void gba_dma_enable(u_char channel);

/*****************************************************************************/

/*
 * Disables the DMA channel
 * @param channel Channel number
 */
extern void gba_dma_disable(u_char channel);

/*****************************************************************************/

/*
 * Copies an area of ​​memory through the DMA3 channel in blocks of 32 bits, for
 * Therefore, ADDRESSES SHOULD BE ALIGNED TO A 32 BIT BORDER
 * @param channel Channel number
 * @param dest Destination address
 * @param source Source address
 * @param size Size in bytes of the transfer
 */
extern void gba_dma_memcpy(void* dest, const void* source, u_int size);

/*****************************************************************************/

/*
 * Initializes a direct sound transfer.
 * @param channel (only DMA1 and DMA2)
 * @param FIFO Cola FIFO channel sound
 * @param Buffer buffer with sound samples
 */
extern void gba_dma_sound_init(u_char channel, void* fifo, const void* buffer);

/*****************************************************************************/

#endif /* gba_dma_h */
