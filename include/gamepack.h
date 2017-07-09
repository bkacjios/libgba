/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to manage access to the cartridges */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gamepack_h
#define gamepack_h

#include    "std_c.h"

/*****************************************************************************/
/* Memory controller interface */
/*****************************************************************************/

typedef struct
{
  short WAITCNT;       /* Game pack memory control */
} gba_gamepack_regs_struct;

extern volatile gba_gamepack_regs_struct gba_gamepack_regs;


/*****************************************************************************/
/* WAITCNT: Game Pack Memory Control */
/*****************************************************************************/

/*
 * Game Pack RAM Wait Control
 */
#define GAMEPACK_RAM_WAIT_4        ((short) 0)         /* 4 hold cycles */
#define GAMEPACK_RAM_WAIT_3        ((short) 1)         /* 3 hold cycles */
#define GAMEPACK_RAM_WAIT_2        ((short) 2)         /* 2 hold cycles */
#define GAMEPACK_RAM_WAIT_8        ((short) 3)         /* 8 hold cycles */

/*
 * Wait Control for the first access in Wait State mode 0
 */
#define GAMEPACK_WS0_WAIT_1_4      ((short) (0 << 2))  /* 4 hold cycles */
#define GAMEPACK_WS0_WAIT_1_3      ((short) (1 << 2))  /* 3 hold cycles */
#define GAMEPACK_WS0_WAIT_1_2      ((short) (2 << 2))  /* 2 hold cycles */
#define GAMEPACK_WS0_WAIT_1_8      ((short) (3 << 2))  /* 8 hold cycles */

/*
 * Wait Control for the second access in Wait State mode 0
 */
#define GAMEPACK_WS0_WAIT_2_2      ((short) (0 << 4))  /* 2 hold cycles */
#define GAMEPACK_WS0_WAIT_2_1      ((short) (1 << 4))  /* 1 hold cycle */

/*
 * Wait Control for the first access in Wait State mode 1
 */
#define GAMEPACK_WS1_WAIT_1_4      ((short) (0 << 5))  /* 4 hold cycles */
#define GAMEPACK_WS1_WAIT_1_3      ((short) (1 << 5))  /* 3 hold cycles */
#define GAMEPACK_WS1_WAIT_1_2      ((short) (2 << 5))  /* 2 hold cycles */
#define GAMEPACK_WS1_WAIT_1_8      ((short) (3 << 5))  /* 8 hold cycles */

/*
 * Wait Control for the second access in Wait State mode 1
 */
#define GAMEPACK_WS1_WAIT_2_4      ((short) (0 << 7))  /* 4 hold cycles */
#define GAMEPACK_WS1_WAIT_2_1      ((short) (1 << 7))  /* 1 hold cycle */

/*
 * Wait Control for the first access in Wait State 2 mode
 */
#define GAMEPACK_WS2_WAIT_1_4      ((short) (0 << 8))  /* 4 hold cycles */
#define GAMEPACK_WS2_WAIT_1_3      ((short) (1 << 8))  /* 3 hold cycles */
#define GAMEPACK_WS2_WAIT_1_2      ((short) (2 << 8))  /* 2 hold cycles */
#define GAMEPACK_WS2_WAIT_1_8      ((short) (3 << 8))  /* 8 hold cycles */

/*
 * Wait Control for the second access in Wait State 2 mode
 */
#define GAMEPACK_WS2_WAIT_2_8      ((short) (0 << 10)) /* 8 hold cycles */
#define GAMEPACK_WS2_WAIT_2_1      ((short) (1 << 10)) /* 1 hold cycle */

/*
 * PHI Terminal Output Control
 */
#define GAMEPACK_PHI_DISABLE       ((short) (0 << 11)) /* No output (default) */
#define GAMEPACK_PHI_4_19          ((short) (1 << 11)) /*  4.19 MHz */
#define GAMEPACK_PHI_8_38          ((short) (2 << 11)) /*  8.38 MHz */
#define GAMEPACK_PHI_16_78         ((short) (3 << 11)) /* 16.78 MHz */

/*
 * Enablement of precaptation
 */
#define GAMEPACK_PREFETCH_ENABLE   ((short) (1 << 14))

/*
 * Cartridge Type
 */
#define GAMEPACK_GBA               ((short) (0 << 15)) /* Gameboy Advance */
#define GAMEPACK_CGB               ((short) (1 << 15)) /* Gameboy Color */

/*****************************************************************************/

#endif /* gamepack_h */

