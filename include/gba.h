/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Constants useful for the management of GBA */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef gba_h
#define gba_h

/*****************************************************************************/
/* Type of data */
/*****************************************************************************/

typedef volatile short * gba_word16;       /* 16-bit memory word */
typedef volatile int   * gba_word32;       /* 32-bit memory word */


/*****************************************************************************/
/* Processor */
/*****************************************************************************/

#include "arm.h"
#define MCK                    (16*1024*1024)  /* Master clock */
#define MCK_KHz                (MCK/1000)


/*****************************************************************************/
/* Memory map */
/*****************************************************************************/

#include "palette.h"   /* Memory for color palette */
#include "vram.h"      /* VRAM */
#include "oam.h"       /* Memory for object attributes */


/*****************************************************************************/
/* Peripherals */
/*****************************************************************************/

#include "video.h"     /* Video controller */
#include "gamepack.h"  /* Cartridge Access Controller */
#include "sound.h"     /* Sound Controller */
#include "dma.h"       /* DMA Controller */
#include "timer.h"     /* Timers */
#include "serial.h"    /* Serial Port */
#include "keypad.h"    /* Buttons */
#include "irq.h"       /* Interrupt Controller */

/*****************************************************************************/

#endif /* gba_h */
