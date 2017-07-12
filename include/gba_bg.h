/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic video library functions */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_bg_h
#define gba_bg_h

#include "gba.h"

/*****************************************************************************/

/*
 * Identifiers of ways to manage funds
 */
#define BG_MODE0               0
#define BG_MODE1               1
#define BG_MODE2               2
#define BG_MODE3               3
#define BG_MODE4               4
#define BG_MODE5               5

/*
 * Identifiers of funds
 */
#define BG0                    0
#define BG1                    1
#define BG2                    2
#define BG3                    3

/*
 * Color Modes
 */
#define BG_COLOR16             VIDEO_BG_COLOR16
#define BG_COLOR256            VIDEO_BG_COLOR256

/*
 * Sizes in pixels for text backgrounds without rotation
 */
#define BG_SIZE_TEXT_256x256   VIDEO_BG_SIZE_TEXT_256x256
#define BG_SIZE_TEXT_512x256   VIDEO_BG_SIZE_TEXT_512x256
#define BG_SIZE_TEXT_256x512   VIDEO_BG_SIZE_TEXT_256x512
#define BG_SIZE_TEXT_512x512   VIDEO_BG_SIZE_TEXT_512x512

#define BG_SIZE_ROT_128x128    VIDEO_BG_SIZE_ROT_128x128
#define BG_SIZE_ROT_256x256    VIDEO_BG_SIZE_ROT_256x256
#define BG_SIZE_ROT_512x512    VIDEO_BG_SIZE_ROT_512x512
#define BG_SIZE_ROT_1024x1024  VIDEO_BG_SIZE_ROT_512x512

/*****************************************************************************/

/*
 * Data structure for managing funds
 */
typedef struct
{
  short      mask;         /* Mask in DISPCNT (VIDEO_BG0, ..., VIDEO_BG3) */
  gba_word16 BGCNT;        /* Pointer to control register */
  gba_word16 BGHOFS;       /* Pointer to horizontal shift register */
  gba_word16 BGVOFS;       /* Pointer to vertical scroll register */
  gba_word16 BGPA;         /* Pointer to PA record (only for rotation backgrounds) */
  gba_word16 BGPB;         /* Pointer to PB record (only for rotation backgrounds) */
  gba_word16 BGPC;         /* Pointer to PC record (only for rotating backgrounds) */
  gba_word16 BGPD;         /* Pointer to PD record (only for rotation backgrounds) */
  gba_word32 BGX;          /* Pointer to the x-coordinate of the reference point (only for rotation backgrounds) */
  gba_word32 BGY;          /* Pointer to the coordinate and the reference point (only for rotation backgrounds) */
  short      char_block;   /* Block for storing characters */
  short      screen_block; /* Block to store the map that defines the background */
  short      ofs_h;        /* Horizontal shift */
  short      ofs_v;        /* Vertical displacement */
  short      width;        /* Width in pixels */
  short      height;       /* High in pixels */
  fixed_p    scale_h;      /* Horizontal scaling factor */
  fixed_p    scale_v;      /* Vertical scaling factor */
  int        rot_angle;    /* Angle of rotation in degrees */
  int        rot_x;        /* Reference point for rotation - x */
  int        rot_y;        /* Reference point for rotation - y */
} bg_struct;

/*****************************************************************************/

/*
 * Function that returns the backgrounds mode.
 */
extern short gba_bg_get_mode(void);

/*****************************************************************************/

/*
 * Function to set the mode of the backgrounds.
 * @param mode (BG_MODE0, BG_MODE1, BG_MODE2, BG_MODE3, BG_MODE4, BG_MODE5)
 */
extern void gba_bg_set_mode(u_char mode);

/*****************************************************************************/

/*
 * Function to initialize a background in text mode.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 * @param color Color setting (BG_COLOR16 or BG_COLOR256)
 * @param size Background size (BG_SIZE_X)
 * @param char_block Block to store characters (0-3)
 * @param screen_block Block to store the map that defines the background (0-31)
 * @param sizeof_chars Character buffer size in bytes
 * @param chars Character buffer pointer
 * @param map Pointer to the buffer that stores the map
 */
extern void gba_bg_init_text(short id, short color, short size,
                      short char_block, short screen_block,
                      short sizeof_chars, const void* chars, const void* map);
                      
/*****************************************************************************/

/*
 * Function to initialize the frontbuffer in bitmap mode (BG2, modes 3-5).
 * @param bitmap Pixel buffer pointer
 */
extern void gba_bg_init_front_buffer(const void* bitmap);

/*****************************************************************************/

/*
 * Function to initialize the backbuffer in bitmap mode (BG2, modes 4-5).
 * @param bitmap Pixel buffer pointer
 */
extern void gba_bg_init_back_buffer(const void* bitmap);

/*****************************************************************************/

/*
 * Initialize a palette of 256 colors for the background.
 * @param palette The palette
 */
extern void gba_bg_init_palette256(const void * palette);

/*****************************************************************************/

/*
 * Initialize one of 16 16-color palettes for the background.
 * @param index Palette index (0..15)
 * @param palette The palette
 */
extern void gba_bg_init_palette16(short index, const void * palette);

/*****************************************************************************/

/*
 * It shows a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern void gba_bg_show(short id);

/*****************************************************************************/

/*
 * Hides a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern void gba_bg_hide(short id);

/*****************************************************************************/

/*
 * Returns the width in pixels of a background
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern short gba_bg_width(short id);

/*****************************************************************************/

/*
 * Returns the height in pixels of a background
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern short gba_bg_height(short id);

/*****************************************************************************/

/*
 * Sets the pixel size of the backgrounds in mosaic mode.
 * @param h_size Horizontal mosaic size of backgrounds (1-16)
 * @param v_size Vertical mosaic size of backgrounds (1-16)
 */
extern void gba_bg_set_mosaic_size(short h_size, short v_size);

/*****************************************************************************/

/*
 * Enables mosaic mode on a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern void gba_bg_enable_mosaic(short id);

/*****************************************************************************/

/*
 * Disables mosaic mode in a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
extern void gba_bg_disable_mosaic(short id);

/*****************************************************************************/

/*
 * Function to apply hardware shift to backgrounds.
 * @param bg Pointer in the background
 * @param dx Horizontal shift
 * @param dy Vertical scrolling
 */
extern void gba_bg_scroll(short id, short dx, short dy);

/*****************************************************************************/


/*
 * Function to update the rotation / scaling attributes.
 * @param id Identifier (BG2, BG3)
 */
void gba_bg_update_rotation(short id);

/*****************************************************************************/

/*
 * Modifies the current rotation center of the fund.
 * @param id Identifier (BG2, BG3)
 * @param dx Horizontal increase / decrease
 * @param dy Vertical increase / decrease
 */
extern void gba_bg_move_rotation_center(short id, int dx, int dy);

/*****************************************************************************/

/*
 * Modifies the current rotation angle of the background.
 * @param id Identifier (BG2, BG3)
 * @param dangle Increment / decrease to current rotation angle
 */
extern void gba_bg_rotate(short id, int dangle);

/*****************************************************************************/

/*
 * Modifies the current scaling of the background.
 * @param id Identifier (BG2, BG3)
 * @param dscale_h Horizontal scaling increment / decrement
 * @param dscale_v Vertical scaling increment / decrement
 */
extern void gba_bg_scale(short id, fixed_p dscale_h, fixed_p dscale_v);

/*****************************************************************************/

/*
 * Repeat the background to fill the screen.
 * @param id Identifier (BG2, BG3)
 */
extern void gba_bg_overflow_wraparound(short id);

/*****************************************************************************/

/*
 * It leaves transparent the part of screen that does not occupy a fund of rotation.
 * @param id Identifier (BG2, BG3)
 */
extern void gba_bg_overflow_transparent(short id);

/*****************************************************************************/

#endif /* gba_bg_h */
