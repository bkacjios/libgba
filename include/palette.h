/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to handle GBA palette memory */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef palette_h
#define palette_h

#include    "std_c.h"

/*****************************************************************************/
/* Memory interface to store the palettes */
/*****************************************************************************/

/*
 *  In the GBA you can use either a palette of 256 colors or 16 palettes
 *  Of 16 colors for both backgrounds and sprites
 */
typedef short gba_palette16[16];
typedef short gba_palette256[256];

typedef struct
{
  union  /* Pallet for backgrounds */
  {
    gba_palette16 bg16[16];
    gba_palette256 bg256;
  } __attribute__ ((packed));

  union  /* Palette for sprites */
  {
    gba_palette16 obj16[16];
    gba_palette256 obj256;
  };
} gba_palette_struct;

extern volatile gba_palette_struct gba_palette;

/*****************************************************************************/

#endif /* palette_h */
