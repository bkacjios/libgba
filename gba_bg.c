/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic video library functions */
/*                                                                           */
/*****************************************************************************/

#include "gba_bg.h"
#include "gba_video.h"
#include "gba_dma.h"
#include "gba_fp.h"

/*****************************************************************************/

/*
 * Static library variables
 */

bg_struct gba_bgs[4]=
{
  /* Background 0 */
  {
    VIDEO_BG0,                            /* Mask in DISPCNT */
    (gba_word16) &(gba_video_regs.BG0CNT),   /* Pointer to control register */
    (gba_word16) &(gba_video_regs.BG0HOFS),  /* Pointer to horizontal shift register */
    (gba_word16) &(gba_video_regs.BG0VOFS),  /* Pointer to vertical scroll register */
    NULL,                                 /* Pointer to PA record (only for rotating backgrounds) */
    NULL,                                 /* Pointer to PB record (only for rotating backgrounds) */
    NULL,                                 /* Pointer to PC record (only for rotating backgrounds) */
    NULL,                                 /* Pointer to PD record (only for rotating backgrounds) */
    NULL,                                 /* Pointer to the x-coordinate of the reference point (only for rotating backgrounds) */
    NULL,                                 /* Pointer to the coordinate and the reference point (only for rotating backgrounds) */
    0,                                    /* Block for storing characters */
    0,                                    /* Block to store the map that defines the background */
    0,                                    /* Horizontal shift */
    0,                                    /* Vertical shift */
    0,                                    /* Width in pixels */
    0,                                    /* Height in pixels */
    I2FP(1),                              /* Horizontal scaling factor */
    I2FP(1),                              /* Vertical scaling factor */
    0,                                    /* Angle of rotation in degrees */
    0,                                    /* Reference point for rotation - x */
    0                                     /* Reference point for rotation - y */
  },
  
  /* Background 1 */
  {
    VIDEO_BG1,                            /* Mask in DISPCNT */
    (gba_word16) &(gba_video_regs.BG1CNT),   /* Pointer to control register */
    (gba_word16) &(gba_video_regs.BG1HOFS),  /* Pointer to horizontal shift register */
    (gba_word16) &(gba_video_regs.BG1VOFS),  /* Pointer to vertical scroll register */
    NULL,                                 /* Pointer to PA record (only for rotation backgrounds) */
    NULL,                                 /* Pointer to PB record (only for rotation backgrounds) */
    NULL,                                 /* Pointer to PC record (only for rotating backgrounds) */
    NULL,                                 /* Pointer to PD record (only for rotation backgrounds) */
    NULL,                                 /* Pointer to the x-coordinate of the reference point (only for rotation backgrounds) */
    NULL,                                 /* Pointer to the coordinate and the reference point (only for rotation backgrounds) */
    0,                                    /* Block for storing characters */
    0,                                    /* Block to store the map that defines the background */
    0,                                    /* Horizontal shift */
    0,                                    /* Vertical displacement */
    0,                                    /* Width in pixels */
    0,                                    /* High in pixels */
    I2FP(1),                              /* Horizontal scaling factor */
    I2FP(1),                              /* Vertical scaling factor */
    0,                                    /* Angle of rotation in degrees */
    0,                                    /* Reference point for rotation - x */
    0                                     /* Reference point for rotation - y */
  },

  /* Background 2 */
  {
    VIDEO_BG2,                            /* Mask in DISPCNT */
    (gba_word16) &(gba_video_regs.BG2CNT),   /* Pointer to control register */
    (gba_word16) &(gba_video_regs.BG2HOFS),  /* Pointer to horizontal shift register */
    (gba_word16) &(gba_video_regs.BG2VOFS),  /* Pointer to vertical scroll register */
    (gba_word16) &(gba_video_regs.BG2PA),    /* Pointer to PA record (only for rotation backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PB),    /* Pointer to PB record (only for rotation backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PC),    /* Pointer to PC record (only for rotating backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PD),    /* Pointer to PD record (only for rotation backgrounds) */
    (gba_word32) &(gba_video_regs.BG2X),     /* Pointer to the x-coordinate of the reference point (only for rotation backgrounds) */
    (gba_word32) &(gba_video_regs.BG2Y),     /* Pointer to the coordinate and the reference point (only for rotation backgrounds) */
    0,                                    /* Block for storing characters */
    0,                                    /* Block to store the map that defines the background */
    0,                                    /* Horizontal shift */
    0,                                    /* Vertical displacement */
    0,                                    /* Width in pixels */
    0,                                    /* High in pixels */
    I2FP(1),                              /* Horizontal scaling factor */
    I2FP(1),                              /* Vertical scaling factor */
    0,                                    /* Angle of rotation in degrees */
    0,                                    /* Reference point for rotation - x */
    0                                     /* Reference point for rotation - y */
  },

  /* Background 3 */
  {
    VIDEO_BG3,                            /* Mask in DISPCNT */
    (gba_word16) &(gba_video_regs.BG3CNT),   /* Pointer to control register */
    (gba_word16) &(gba_video_regs.BG3HOFS),  /* Pointer to horizontal shift register */
    (gba_word16) &(gba_video_regs.BG3VOFS),  /* Pointer to vertical scroll register */
    (gba_word16) &(gba_video_regs.BG2PA),    /* Pointer to PA record (only for rotation backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PB),    /* Pointer to PB record (only for rotation backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PC),    /* Pointer to PC record (only for rotating backgrounds) */
    (gba_word16) &(gba_video_regs.BG2PD),    /* Pointer to PD record (only for rotation backgrounds) */
    (gba_word32) &(gba_video_regs.BG2X),     /* Pointer to the x-coordinate of the reference point (only for rotation backgrounds) */
    (gba_word32) &(gba_video_regs.BG2Y),     /* Pointer to the coordinate and the reference point (only for rotation backgrounds) */
    0,                                    /* Block for storing characters */
    0,                                    /* Block to store the map that defines the background */
    0,                                    /* Horizontal shift */
    0,                                    /* Vertical displacement */
    0,                                    /* Width in pixels */
    0,                                    /* High in pixels */
    I2FP(1),                              /* Horizontal scaling factor */
    I2FP(1),                              /* Vertical scaling factor */
    0,                                    /* Angle of rotation in degrees */
    0,                                    /* Reference point for rotation - x */
    0                                     /* Reference point for rotation - y */
  }
};

/*
 * Copy of MOSAIC record. This is necessary because MOSAIC registration is not
 * can read. It's just writing.
 */
short gba_bg_mosaic_shadow = 0x0000;

/*****************************************************************************/

/*
 * Function that returns the backgrounds mode.
 */
inline short gba_bg_get_mode(void)
{
  return gba_video_regs.DISPCNT & (BG_MODE1|BG_MODE2|BG_MODE3|BG_MODE4|BG_MODE5);
}

/*****************************************************************************/

/*
 * Function to set the mode of the backgrounds.
 * @param mode (BG_MODE0, BG_MODE1, BG_MODE2, BG_MODE3, BG_MODE4, BG_MODE5)
 */
void gba_bg_set_mode(u_char mode)
{
  gba_video_regs.DISPCNT = mode;
  if (mode < BG_MODE3)
  {
    gba_bgs[0].width  = 0;
    gba_bgs[0].height = 0;
    gba_bgs[1].width  = 0;
    gba_bgs[1].height = 0;
    gba_bgs[2].width  = 0;
    gba_bgs[2].height = 0;
    gba_bgs[3].width  = 0;
    gba_bgs[3].height = 0;
  }
  else if (mode < BG_MODE5)
  {
    gba_bgs[2].width = BG_MODE_WIDTH3;
    gba_bgs[2].height = BG_MODE_HEIGHT3;
  }
  else /* BG_MODE5 */
  {
    gba_bgs[2].width = BG_MODE_WIDTH5;
    gba_bgs[2].height = BG_MODE_HEIGHT5;
  }

}

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
void gba_bg_init_text(short id, short color, short size,
                      short char_block, short screen_block,
                      short sizeof_chars, const void* chars, const void* map)
{
  short bg_mode = gba_bg_get_mode();

  short sizeof_map_entry;
  short sizeof_map;

  gba_bgs[id].char_block = char_block;      /* Block of characters */
  gba_bgs[id].screen_block = screen_block;  /* Map block */

  /* Set the background */
  *(gba_bgs[id].BGCNT) = color | size | VIDEO_CHAR_BLOCK(char_block) |
                                        VIDEO_SCREEN_BLOCK(screen_block);

  /*
   * We check if it is a text background without rotation
   *  - in mode 0 the four backgrounds are text
   *  - in mode 1 only backgrounds 0 and 1 are text
   */
  if (bg_mode==0 || (bg_mode==1 && id<BG2))
  {
    /* If it is a background without rotation, each map entry occupies 2 bytes */
    sizeof_map_entry = 2;

    if (size & VIDEO_BG_WIDTH_TEXT_512)
      gba_bgs[id].width = 512;
    else
      gba_bgs[id].width = 256;
      
    if (size & VIDEO_BG_HEIGHT_TEXT_512)
      gba_bgs[id].height = 512;
    else
      gba_bgs[id].height = 256;
  }
  /*
   * Let's check if it's a rotated text background
   *  - in mode 1 only the bottom 2 is rotating
   *  - in mode 2 only backgrounds 2 and 3 are rotating
   */
  else if ((bg_mode==1 && id==BG2) || (bg_mode==2 && (id==BG2 || id==BG3)))
  {
    /* If it is a background with rotation, each entry of the map occupies 1 byte */
    sizeof_map_entry = 1;

    switch (size)
    {
      case VIDEO_BG_SIZE_ROT_128x128:
        gba_bgs[id].width = 128;
        gba_bgs[id].height = 128;
        break;
      case VIDEO_BG_SIZE_ROT_256x256:
        gba_bgs[id].width = 256;
        gba_bgs[id].height = 256;
        break;
      case VIDEO_BG_SIZE_ROT_512x512:
        gba_bgs[id].width = 512;
        gba_bgs[id].height = 512;
        break;
      case VIDEO_BG_SIZE_ROT_1024x1024:
        gba_bgs[id].width = 1024;
        gba_bgs[id].height = 1024;
        break;
    }
  }
  else /* If it is not defined, we return without doing anything */
    return;

  /* Map size in bytes (each character has 64 pixels) */
  sizeof_map = (gba_bgs[id].width*gba_bgs[id].height*sizeof_map_entry) / 64;
    
  /* Copy the characters into the video memory */
  gba_dma_memcpy((void*) gba_vram.char_block[gba_bgs[id].char_block],
                 chars, sizeof_chars);

  /* Copy the map in memory of the background 0 */
  gba_dma_memcpy((void*) gba_vram.screen_block[gba_bgs[id].screen_block],
                 map, sizeof_map);
}

/*****************************************************************************/

/*
 * Function to initialize the frontbuffer in bitmap mode (BG2, modes 3-5).
 * @param bitmap Pixel buffer pointer
 */
void gba_bg_init_front_buffer(const void* bitmap)
{
  char pix_size = 1;
  if(gba_bg_get_mode()==BG_MODE3)
    pix_size = 2;
  gba_dma_memcpy((void*)gba_vram.front_buffer, (void*)bitmap,
                 gba_bgs[BG2].width*gba_bgs[BG2].height*pix_size);
}

/*****************************************************************************/

/*
 * Function to initialize the backbuffer in bitmap mode (BG2, modes 4-5).
 * @param bitmap Pixel buffer pointer
 */
void gba_bg_init_back_buffer(const void* bitmap)
{
  gba_dma_memcpy((void*)gba_vram.back_buffer, (void*)bitmap,
                 gba_bgs[BG2].width*gba_bgs[BG2].height);
}

/*****************************************************************************/

/*
 * Initialize a palette of 256 colors for the background.
 * @param palette The palette
 */
inline void gba_bg_init_palette256(const void * palette)
{
  gba_dma_memcpy((void*) gba_palette.bg256, palette, sizeof(gba_palette256));
}

/*****************************************************************************/

/*
 * Initialize one of 16 16-color palettes for the background.
 * @param index Palette index (0..15)
 * @param palette The palette
 */
inline void gba_bg_init_palette16(short index, const void * palette)
{
  gba_dma_memcpy((void*) gba_palette.bg16[index], palette, sizeof(gba_palette16));
}

/*****************************************************************************/

/*
 * It shows a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline void gba_bg_show(short id)
{
  gba_video_regs.DISPCNT |= gba_bgs[id].mask;
}

/*****************************************************************************/

/*
 * Hides a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline void gba_bg_hide(short id)
{
  gba_video_regs.DISPCNT &= ~gba_bgs[id].mask;
}

/*****************************************************************************/

/*
 * Returns the width in pixels of a background
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline short gba_bg_width(short id)
{
  return gba_bgs[id].width;
}

/*****************************************************************************/

/*
 * Returns the height in pixels of a background
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline short gba_bg_height(short id)
{
  return gba_bgs[id].height;
}

/*****************************************************************************/

/*
 * Sets the pixel size of the backgrounds in mosaic mode.
 * @param h_size Horizontal mosaic size of backgrounds (1-16)
 * @param v_size Vertical mosaic size of backgrounds (1-16)
 */
inline void gba_bg_set_mosaic_size(short h_size, short v_size)
{
  gba_bg_mosaic_shadow &= 0xff00;
  gba_bg_mosaic_shadow |= (((v_size-1) & 0x000f) << 4 ) | ((h_size-1) & 0x000f);
  gba_video_regs.MOSAIC =  gba_bg_mosaic_shadow;
}

/*****************************************************************************/

/*
 * Enables mosaic mode on a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline void gba_bg_enable_mosaic(short id)
{
  *(gba_bgs[id].BGCNT) |= VIDEO_BG_MOSAIC;
}

/*****************************************************************************/

/*
 * Disables mosaic mode in a background.
 * @param id Identifier (BG0, BG1, BG2, BG3)
 */
inline void gba_bg_disable_mosaic(short id)
{
  *(gba_bgs[id].BGCNT) &= ~VIDEO_BG_MOSAIC;
}

/*****************************************************************************/

/*
 * Function to apply hardware shift to backgrounds.
 * @param bg Pointer in the background
 * @param dx Horizontal shift
 * @param dy Vertical scrolling
 */
inline void gba_bg_scroll(short id, short dx, short dy)
{
  gba_bgs[id].ofs_h += dx;
  gba_bgs[id].ofs_v += dy;

  *(gba_bgs[id].BGHOFS) = gba_bgs[id].ofs_h;
  *(gba_bgs[id].BGVOFS) = gba_bgs[id].ofs_v;
}

/*****************************************************************************/


/*
 * Function to update the rotation / scaling attributes.
 * @param id Identifier (BG2, BG3)
 */
void gba_bg_update_rotation(short id)
{
  fixed_p dx, dy, dmx, dmy;
 
  /* Displacement on the x-axis of the same line */
  dx = gba_fp_mult(gba_bgs[id].scale_h, gba_fp_cos(gba_bgs[id].rot_angle));

  /* Displacement on the y axis of the same line */
  dy = gba_fp_mult(gba_bgs[id].scale_h, -gba_fp_sin(gba_bgs[id].rot_angle));

  /* Displacement on the x-axis of the next line */
  dmx = gba_fp_mult(gba_bgs[id].scale_v, gba_fp_sin(gba_bgs[id].rot_angle));

  /* Offset on the y-axis of the next line */
  dmy = gba_fp_mult(gba_bgs[id].scale_v, gba_fp_cos(gba_bgs[id].rot_angle));

  /* We update the records */
  *(gba_bgs[id].BGX)  = I2FP(gba_bgs[id].rot_x);
  *(gba_bgs[id].BGY)  = I2FP(gba_bgs[id].rot_y);
  *(gba_bgs[id].BGPA) = dx;
  *(gba_bgs[id].BGPC) = dy;
  *(gba_bgs[id].BGPB) = dmx;
  *(gba_bgs[id].BGPD) = dmy;
}

/*****************************************************************************/

/*
 * Modifies the current rotation center of the fund.
 * @param id Identifier (BG2, BG3)
 * @param dx Horizontal increase / decrease
 * @param dy Vertical increase / decrease
 */
inline void gba_bg_move_rotation_center(short id, int dx, int dy)
{
  gba_bgs[id].rot_x += dx;
  gba_bgs[id].rot_y += dy;
  gba_bg_update_rotation(id);
}

/*****************************************************************************/

/*
 * Modifies the current rotation angle of the background.
 * @param id Identifier (BG2, BG3)
 * @param dangle Increment / decrease to current rotation angle
 */
inline void gba_bg_rotate(short id, int dangle)
{
  gba_bgs[id].rot_angle += dangle;
  gba_bg_update_rotation(id);
}

/*****************************************************************************/

/*
 * Modifies the current scaling of the background.
 * @param id Identifier (BG2, BG3)
 * @param dscale_h Horizontal scaling increment / decrement
 * @param dscale_v Vertical scaling increment / decrement
 */
inline void gba_bg_scale(short id, fixed_p dscale_h, fixed_p dscale_v)
{
  gba_bgs[id].scale_h += dscale_h;
  gba_bgs[id].scale_v += dscale_v;
  gba_bg_update_rotation(id);
}

/*****************************************************************************/

/*
 * Repeat the background to fill the screen.
 * @param id Identifier (BG2, BG3)
 */
inline void gba_bg_overflow_wraparound(short id)
{
  *(gba_bgs[id].BGCNT) |= VIDEO_BG_WRAPAROUND;
}

/*****************************************************************************/

/*
 * It leaves transparent the part of screen that does not occupy a fund of rotation.
 * @param id Identifier (BG2, BG3)
 */
inline void gba_bg_overflow_transparent(short id)
{
  *(gba_bgs[id].BGCNT) &= ~VIDEO_BG_WRAPAROUND;
}

/*****************************************************************************/

