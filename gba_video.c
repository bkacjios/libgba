/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic video library functions */
/*                                                                           */
/*****************************************************************************/

#include "gba_video.h"

/*****************************************************************************/

/*
 * Change the video buffer (in dual buffer modes)
 */
inline void gba_video_swap_buffer()
{
  gba_video_regs.DISPCNT ^= VIDEO_BACKBUFFER;
}

/*****************************************************************************/

/*
 * Wait until you reach a VBLANK period.
 */
inline void gba_video_wait_vblank()
{
  while(!(gba_video_regs.DISPSTAT & VIDEO_VBLANK));
  while(gba_video_regs.DISPSTAT & VIDEO_VBLANK);
}

/*****************************************************************************/

/*
 * Sets the boundary of lines drawn to generate the interrupt VIDEO_INT_VCOUNTER
 * @limit Limit
 */
inline void gba_video_set_vcounter_limit(u_char limit)
{
  gba_video_regs.DISPSTAT |= VIDEO_VCOUNT_SETTING(limit);
}

/*****************************************************************************/

/*
 * Enables the generation of interrupts.
 * @mask Mask of the interrupts to be generated.
 */
inline void gba_video_enable_int(u_char mask)
{
  gba_video_regs.DISPSTAT |= mask;
}

/*****************************************************************************/

/*
 * Disables interrupt generation.
 * @mask Mask of the interrupts to be generated.
 */
inline void gba_video_disable_int(u_char mask)
{
  /* I set the mask for interrupts */
  gba_video_regs.DISPSTAT &= ~mask;
}

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
inline void gba_video_enable_bld_alpha(short layer1, short layer2, char level1, char level2)
{
  gba_video_regs.BLDCNT = layer1 | (layer2 << 8) | VIDEO_BLD_ALPHA;
  gba_video_regs.BLDALPHA = VIDEO_BLD_EVA(level1) | VIDEO_BLD_EVB(level2);
}

/*****************************************************************************/

/*
 * Enables fade-in of a layer (increase brightness).
 * Increment levels are discrete values ​​from 0 to 16, indicating the
 * Value 0 no increment and 16 saturation to the target
 * @param layer (VIDEO_BLD_X)
 * @param level
 */
inline void gba_video_enable_bld_fade_in(short layer, char level)
{
  gba_video_regs.BLDCNT = layer | VIDEO_BLD_BRIGHT_INC;
  gba_video_regs.BLDY = VIDEO_BLD_EVA(level);
}

/*****************************************************************************/

/*
 * Enables the fade-out of a layer (decrease in brightness).
 * Decrement levels are discrete values ​​from 0 to 16, indicating the
 * Value 0 no decrement and 16 saturation to black
 * @param layer (VIDEO_BLD_X)
 * @param level Decrease level
 */
inline void gba_video_enable_bld_fade_out(short layer, char level)
{
  gba_video_regs.BLDCNT = layer | VIDEO_BLD_BRIGHT_DEC;
  gba_video_regs.BLDY = VIDEO_BLD_EVA(level);
}

/*****************************************************************************/

/*
 * Disables special color effects (alpha-blending, fade-in / fade-out)
 */
inline void gba_video_disable_bld()
{
  gba_video_regs.BLDCNT &= ~VIDEO_BLD_BRIGHT_DEC;
}

/*****************************************************************************/

