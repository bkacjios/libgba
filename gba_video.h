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
#ifndef gba_video_h
#define gba_video_h

#include "gba.h"

/*****************************************************************************/

/*
 * Dimensions of the LCD screen
 */
#define SCREEN_WIDTH           240
#define SCREEN_HEIGHT          160

/*
 * Layer identifiers for special color effects
 */
#define VIDEO_BLD_BG0          ((short) (1 << 0))
#define VIDEO_BLD_BG1          ((short) (1 << 1))
#define VIDEO_BLD_BG2          ((short) (1 << 2))
#define VIDEO_BLD_BG3          ((short) (1 << 3))
#define VIDEO_BLD_OBJ          ((short) (1 << 4))
#define VIDEO_BLD_BD           ((short) (1 << 5))

/*
 * Identifiers of the interrupts that the video driver can generate
 */
#define VIDEO_INT_VBLANK       ((short) (1 << 3))
#define VIDEO_INT_HBLANK       ((short) (1 << 4))
#define VIDEO_INT_VCOUNTER     ((short) (1 << 5))

/*****************************************************************************/

/*
 * Change the video buffer (in dual buffer modes)
 */
inline void gba_video_swap_buffer();

/*****************************************************************************/

/*
 * Wait until you reach a VBLANK period.
 */
inline void gba_video_wait_vblank();

/*****************************************************************************/

/*
 * Sets the boundary of lines drawn to generate the interrupt VIDEO_INT_VCOUNTER
 * @limit Limit
 */
inline void gba_video_set_vcounter_limit(u_char limit);

/*****************************************************************************/

/*
 * Enables the generation of interrupts.
 * @mask Mask of the interrupts to be generated.
 */
inline void gba_video_enable_int(u_char mask);

/*****************************************************************************/

/*
 * Disables interrupt generation.
 * @mask Mask of the interrupts to be generated.
 */
inline void gba_video_disable_int(u_char mask);

/*****************************************************************************/

/*
 * Enable alpha-blending.
 * The transparency levels are discrete values ​​from 0 to 16, indicating the
 * Value 0 total transparency and value 16 total opacity
 * @param layer1 First layer (VIDEO_BLD_X)
 * @param layer2 Second layer (VIDEO_BLD_X)
 * @param level1 Level of transparency of the first layer
 * @param level2 Level of transparency of the second layer
 */
inline void gba_video_enable_bld_alpha(short layer1, short layer2, char level1, char level2);

/*****************************************************************************/

/*
 * Enables fade-in of a layer (increase brightness).
 * Increment levels are discrete values ​​from 0 to 16, indicating the
 * Value 0 no increment and 16 saturation to the target
 * @param layer (VIDEO_BLD_X)
 * @param level
 */
inline void gba_video_enable_bld_fade_in(short layer, char level);

/*****************************************************************************/

/*
 * Enables the fade-out of a layer (decrease in brightness).
 * Decrement levels are discrete values ​​from 0 to 16, indicating the
 * Value 0 no decrement and 16 saturation to black
 * @param layer (VIDEO_BLD_X)
 * @param level Decrease level
 */
inline void gba_video_enable_bld_fade_out(short layer, char level);

/*****************************************************************************/

/*
 * Disables special color effects (alpha-blending, fade-in / fade-out)
 */
inline void gba_video_disable_bld();

/*****************************************************************************/

#endif /* gba_video_h */
