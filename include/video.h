/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants for the GBA video driver */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef video_h
#define video_h

#include    "std_c.h"

/*****************************************************************************/
/* Console video driver interface */
/*****************************************************************************/

typedef struct
{
  short DISPCNT;       /* Lcd control */
  short undocumented;  /* Green swap */
  short DISPSTAT;      /* General lcd status (stat, lyc) */
  short VCOUNT;        /* Vertical counter (ly) */
  short BG0CNT;        /* Bg0 control */
  short BG1CNT;        /* Bg1 control */
  short BG2CNT;        /* Bg2 control */
  short BG3CNT;        /* Bg3 control */
  short BG0HOFS;       /* Bg0 x-offset */
  short BG0VOFS;       /* Bg0 y-offset */
  short BG1HOFS;       /* Bg1 x-offset */
  short BG1VOFS;       /* Bg1 and offset */
  short BG2HOFS;       /* Bg2 x-offset */
  short BG2VOFS;       /* Bg2 and offset */
  short BG3HOFS;       /* Bg3 x-offset */
  short BG3VOFS;       /* Bg3 y-offset */
  short BG2PA;         /* Bg2 rotation / scaling parameter a (dx) */
  short BG2PB;         /* Bg2 rotation / scaling parameter b (dmx) */
  short BG2PC;         /* Bg2 rotation / scaling parameter c (dy) */
  short BG2PD;         /* Bg2 rotation / scaling parameter d (dmy) */
  int   BG2X;          /* Bg2 reference point x-coordinate */
  int   BG2Y;          /* Bg2 reference point y-coordinate */
  short BG3PA;         /* Bg3 rotation / scaling parameter a (dx) */
  short BG3PB;         /* Bg3 rotation / scaling parameter b (dmx) */
  short BG3PC;         /* Bg3 rotation / scaling parameter c (dy) */
  short BG3PD;         /* Bg3 rotation / scaling parameter d (dmy) */
  int   BG3X;          /* Bg3 reference point x-coordinate */
  int   BG3Y;          /* Bg3 reference point y-coordinate */
  short WIN0H;         /* Window 0 horizontal dimensions */
  short WIN1H;         /* Window 1 horizontal dimensions */
  short WIN0V;         /* Window 0 vertical dimensions */
  short WIN1V;         /* Window 1 vertical dimensions */
  short WININ;         /* Control inside of window (s) */
  short WINOUT;        /* Control outside of windows & inside of obj window */
  short MOSAIC;        /* Mosaic size */
  short filler;        /* Not used */
  short BLDCNT;        /* Color special effects selection (formerly bldmod) */
  short BLDALPHA;      /* Alpha blending coefficients (formerly colev) */
  short BLDY;          /* Brightness (fade-in / out) coefficient (before coly) */
} gba_video_regs_struct;

extern volatile gba_video_regs_struct gba_video_regs;


/*****************************************************************************/
/* DISPCNT: Screen control */
/*****************************************************************************/

/*
 * BG Mode
 */
#define VIDEO_BG_MODE0             ((short) 0)         /* Video mode 0 */
#define VIDEO_BG_MODE1             ((short) 1)         /* Video mode 1 */
#define VIDEO_BG_MODE2             ((short) 2)         /* Video mode 2 */
#define VIDEO_BG_MODE3             ((short) 3)         /* Bitmap, 240x160, 15 bit */
#define VIDEO_BG_MODE4             ((short) 4)         /* Bitmap, 240x160, 8 bit, double buffer */
#define VIDEO_BG_MODE5             ((short) 5)         /* Bitmap, 160x128, 15 bit, double buffer */

/*
 * Display Frame Selection (0 -> Frontbuffer, 1 -> Backbuffer)
 */
#define VIDEO_FRONTBUFFER          ((short) (0 << 4))  /* (Modes 4 and 5 only) */
#define VIDEO_BACKBUFFER           ((short) (1 << 4))  /* (Modes 4 and 5 only) */

/*
 * H-Blank Interval OBJ Processing Flag
 */
#define VIDEO_HBLANK_OAM           ((short) (1 << 5))  /* Allow access to OAM during H-Blank */

/*
 * OBJ Character VRAM Mapping Format
 */
#define VIDEO_OBJ_MAP_2D           ((short) (0 << 6))  /* Two dimensional mapping */
#define VIDEO_OBJ_MAP_1D           ((short) (1 << 6))  /* One dimensional mapping */

/*
 * Forced Blank
 */
#define VIDEO_FORCE_BLANK          ((short) (1 << 7))  /* Allow access to VRAM, Palette, OAM */

/*
 * Individual Screens Display Flag
 */
#define VIDEO_BG0                  ((short) (1 << 8))  /* Display background 0 */
#define VIDEO_BG1                  ((short) (1 << 9))  /* Display background 1 */
#define VIDEO_BG2                  ((short) (1 << 10)) /* Display background 2 */
#define VIDEO_BG3                  ((short) (1 << 11)) /* Display background 3 */
#define VIDEO_OBJ                  ((short) (1 << 12)) /* OBJ Display */

/*
 * Display Flags for Windows 0 and 1
 */
#define VIDEO_WIN0                 ((short) (1 << 13)) /* Display window 0 */
#define VIDEO_WIN1                 ((short) (1 << 14)) /* Display window 1 */

/*
 * OBJ Window Display Flag
 */
#define VIDEO_WINOBJ               ((short) (1 << 15)) /* Display OBJ window */


/*****************************************************************************/
/* UNDOCUMENTED: Interchange of green between two consecutive pixels */
/*****************************************************************************/

#define VIDEO_GREEN_SWAP           ((short) 1)         /* Green exchange */


/*****************************************************************************/
/* DISPSTAT: Screen state and generation of interrupts */
/*****************************************************************************/

/*
 * Status
 */
#define VIDEO_VBLANK               ((short) (1 << 0))  /* 1 = V_Blank (Read Only) */
#define VIDEO_HBLANK               ((short) (1 << 1))  /* 1 = H_Blank (Read Only) */
#define VIDEO_VCOUNTER             ((short) (1 << 2))  /* 1 = Match (Read Only) */

/*
 * Interrupt Request Enable Flags
 */
#define VIDEO_VBLANK_INT           ((short) (1 << 3))  /* 1 = Generation of int. */
#define VIDEO_HBLANK_INT           ((short) (1 << 4))  /* 1 = Generation of int. */
#define VIDEO_VCOUNTER_INT         ((short) (1 << 5))  /* 1 = Generation of int. */

/*
 * V Count Setting
 * When n (0-227) reaches the value of VCOUNT an interrupt will be generated
 * (If enabled)
 */
#define VIDEO_VCOUNT_SETTING(n)    ((((short)(n)) & 0x00ff) << 8)

/*****************************************************************************/
/* BGxCNT: Funds Control */
/*****************************************************************************/

/*
 * Priority Among BGs
 */
#define VIDEO_BG_MAX_PRIOR         ((short) 0)         /* Maximum priority */
#define VIDEO_BG_MEDIUM_PRIOR      ((short) 1)         /* Medium priority */
#define VIDEO_BG_LOW_PRIOR         ((short) 2)         /* Low Priority */
#define VIDEO_BG_MIN_PRIOR         ((short) 3)         /* Minimum Priority */

/*
 * Character Base Block Specification - n = (0-3)
 * Specifies the block of VRAM memory in which the characters are
 * Will use to define the wallpaper.
 * The VRAM is divided in 4 blocks of 16Kb in which they can accommodate characters
 * Which will be used to design the funds.
 */
#define VIDEO_CHAR_BLOCK(n)        ((((short)(n)) & 0x0003) << 2)

/*
 * Mosaic
 */
#define VIDEO_BG_MOSAIC            ((short) (1 << 6))  /* Turn mosaic mode on */

/*
 * Color Mode
 */
#define VIDEO_BG_COLOR16           ((short) (0 << 7))
#define VIDEO_BG_COLOR256          ((short) (1 << 7))

/*
 * Screen Base Block Specification - n = (0-31)
 * Specifies the VRAM memory block in which the map defines which
 * Characters are used to design the screen.
 * The VRAM is divided in 32 blocks of 2Kb in which the maps of the
 * Designs of different screens.
 */
#define VIDEO_SCREEN_BLOCK(n)      ((((short)(n)) & 0x001f) << 8)

/*
 * Area Overflow Processing (Only in rotation backgrounds)
 */
#define VIDEO_BG_WRAPAROUND        ((short) (1 << 13))

/*
 * Screen Size
 */
#define VIDEO_BG_WIDTH_TEXT_256     ((short) (0 << 14))
#define VIDEO_BG_WIDTH_TEXT_512     ((short) (1 << 14))
#define VIDEO_BG_HEIGHT_TEXT_256    ((short) (0 << 15))
#define VIDEO_BG_HEIGHT_TEXT_512    ((short) (1 << 15))

#define VIDEO_BG_SIZE_TEXT_256x256  (VIDEO_BG_WIDTH_TEXT_256|VIDEO_BG_HEIGHT_TEXT_256)
#define VIDEO_BG_SIZE_TEXT_512x256  (VIDEO_BG_WIDTH_TEXT_512|VIDEO_BG_HEIGHT_TEXT_256)
#define VIDEO_BG_SIZE_TEXT_256x512  (VIDEO_BG_WIDTH_TEXT_256|VIDEO_BG_HEIGHT_TEXT_512)
#define VIDEO_BG_SIZE_TEXT_512x512  (VIDEO_BG_WIDTH_TEXT_512|VIDEO_BG_HEIGHT_TEXT_512)

#define VIDEO_BG_SIZE_ROT_128x128   ((short) (0 << 14))
#define VIDEO_BG_SIZE_ROT_256x256   ((short) (1 << 14))
#define VIDEO_BG_SIZE_ROT_512x512   ((short) (2 << 14))
#define VIDEO_BG_SIZE_ROT_1024x1024 ((short) (3 << 14))


/*****************************************************************************/
/* WINxH: Horizontal dimensions of windows */
/*****************************************************************************/

/*
 * X1 and x2 are the left and right coordinates of the window
 */
#define VIDEO_WIN_HSIZE(x1,x2) ((short) ((((x1) & 0x00ff)<<8) + (((x2)+1) & 0x00ff)))


/*****************************************************************************/
/* WINxV: Vertical dimensions of windows */
/*****************************************************************************/

/*
 * Y1 and y2 are the upper and lower coordinates of the window
 */
#define VIDEO_WIN_VSIZE(y1,y2) ((short) ((((y1) & 0x00ff)<<8) + (((y2)+1) & 0x00ff)))


/*****************************************************************************/
/* WININ: Control inside windows 0 and 1 */
/*****************************************************************************/

/*
 * Window 0 Flags
 */
#define VIDEO_WIN0_BG0             ((short) (1 << 0))  /* Enable BG0 in Window 0 */
#define VIDEO_WIN0_BG1             ((short) (1 << 1))  /* Enable BG1 in Window 0 */
#define VIDEO_WIN0_BG2             ((short) (1 << 2))  /* Enable BG2 in Window 0 */
#define VIDEO_WIN0_BG3             ((short) (1 << 3))  /* Enable BG3 in Window 0 */
#define VIDEO_WIN0_OBJ             ((short) (1 << 4))  /* Enable OBJ in Window 0 */
#define VIDEO_WIN0_BLD             ((short) (1 << 5))  /* Enable Color BLD in Window 0 */

/*
 * Window 1 Flags
 */
#define VIDEO_WIN1_BG0             ((short) (1 << 8))  /* Enable BG0 in Window 1 */
#define VIDEO_WIN1_BG1             ((short) (1 << 9))  /* Enable BG1 in Window 1 */
#define VIDEO_WIN1_BG2             ((short) (1 << 10)) /* Enable BG2 in Window 1 */
#define VIDEO_WIN1_BG3             ((short) (1 << 11)) /* Enable BG3 in Window 1 */
#define VIDEO_WIN1_OBJ             ((short) (1 << 12)) /* Enable OBJ in Window 1 */
#define VIDEO_WIN1_BLD             ((short) (1 << 13)) /* Enable Color BLD in Window 1 */


/*****************************************************************************/
/* WINOUT: Control outside the windows and inside the OBJ window */
/*****************************************************************************/

/*
 * Flags for the area outside the windows
 */
#define VIDEO_OUT_BG0              ((short) (1 << 0))  /* Enable BG0 outside windows */
#define VIDEO_OUT_BG1              ((short) (1 << 1))  /* Enable BG1 outside windows */
#define VIDEO_OUT_BG2              ((short) (1 << 2))  /* Enable BG2 outside windows */
#define VIDEO_OUT_BG3              ((short) (1 << 3))  /* Enable BG3 outside windows */
#define VIDEO_OUT_OBJ              ((short) (1 << 4))  /* Enable OBJ outside windows */
#define VIDEO_OUT_BLD              ((short) (1 << 5))  /* Enable Color BLD outside windows */

/*
 * OBJ window flags
 */
#define VIDEO_WINOBJ_BG0           ((short) (1 << 8))  /* Enable BG0 in OBJ Window */
#define VIDEO_WINOBJ_BG1           ((short) (1 << 9))  /* Enable BG1 in OBJ Window */
#define VIDEO_WINOBJ_BG2           ((short) (1 << 10)) /* Enable BG2 in OBJ Window */
#define VIDEO_WINOBJ_BG3           ((short) (1 << 11)) /* Enable BG3 in OBJ Window */
#define VIDEO_WINOBJ_OBJ           ((short) (1 << 12)) /* Enable OBJ in OBJ Window */
#define VIDEO_WINOBJ_BLD           ((short) (1 << 13)) /* Enable Color BLD in OBJ Window */


/*****************************************************************************/
/* MOSAIC: Tile Size */
/*****************************************************************************/

/*
 * N is 4 bits (0-31)
 */
#define VIDEO_BG_MOSAIC_HSIZE(n)  (((((short)(n)) - 1) & 0x000f) << 0)
#define VIDEO_BG_MOSAIC_VSIZE(n)  (((((short)(n)) - 1) & 0x000f) << 4)
#define VIDEO_OBJ_MOSAIC_HSIZE(n) (((((short)(n)) - 1) & 0x000f) << 8)
#define VIDEO_OBJ_MOSAIC_VSIZE(n) (((((short)(n)) - 1) & 0x000f) << 12)


/*****************************************************************************/
/* BLDCNT (formerly BLDMOD): Select. Special Color Effects (BLEND) */
/*****************************************************************************/

#define VIDEO_BLD_BG0_1ST          ((short) (1 << 0))  /* BG0 is the first layer to apply the effects */
#define VIDEO_BLD_BG1_1ST          ((short) (1 << 1))  /* BG1 is the first layer to apply the effects */
#define VIDEO_BLD_BG2_1ST          ((short) (1 << 2))  /* BG2 is the first layer to apply the effects */
#define VIDEO_BLD_BG3_1ST          ((short) (1 << 3))  /* BG3 is the first layer to apply the effects */
#define VIDEO_BLD_OBJ_1ST          ((short) (1 << 4))  /* OBJ is the first layer to apply the effects */
#define VIDEO_BLD_BD_1ST           ((short) (1 << 5))  /* BD is the first layer to apply the effects */
#define VIDEO_BLD_ALPHA            ((short) (1 << 6))  /* Enable alpha-blending */
#define VIDEO_BLD_BRIGHT_INC       ((short) (2 << 6))  /* Increases brightness */
#define VIDEO_BLD_BRIGHT_DEC       ((short) (3 << 6))  /* Decrease brightness */
#define VIDEO_BLD_BG0_2ND          ((short) (1 << 8))  /* BG0 is the second layer to apply the effects */
#define VIDEO_BLD_BG1_2ND          ((short) (1 << 9))  /* BG1 is the second layer to apply the effects */
#define VIDEO_BLD_BG2_2ND          ((short) (1 << 10)) /* BG2 is the second layer to apply the effects */
#define VIDEO_BLD_BG3_2ND          ((short) (1 << 11)) /* BG3 is the second layer to apply the effects */
#define VIDEO_BLD_OBJ_2ND          ((short) (1 << 12)) /* OBJ is the second layer to apply the effects */
#define VIDEO_BLD_BD_2ND           ((short) (1 << 13)) /* BD is the second layer to apply the effects */


/*****************************************************************************/
/* BLDALPHA (formerly COLEV) and BLDY (formerly COLY): Coefficients for */
/*****************************************************************************/

/*
 *  EVA coefficient (for the first layer selected in BLDCNT)
 *  0..16 = 0 / 16..16 / 16, 17..31 = 16/16
 */
#define VIDEO_BLD_EVA(n)           ((((short)(n)) & 0x000f) << 0)

/*
 *  EVB coefficient (for the second layer selected in BLDCNT) (BLDALPHA only)
 *  0..16 = 0 / 16..16 / 16, 17..31 = 16/16
 */
#define VIDEO_BLD_EVB(n)           ((((short)(n)) & 0x000f) << 8)

/*****************************************************************************/

#endif /* video_h */
