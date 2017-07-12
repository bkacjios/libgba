/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling sprites */
/*                                                                           */
/*****************************************************************************/

#include "gba_sprite.h"
#include "gba_video.h"
#include "gba_bg.h"
#include "gba_dma.h"
#include "gba_fp.h"
#include "gba_bios.h"

/*****************************************************************************/

/*
 * Static library variables
 */

/*
 * Copy of the OAM to modify the sprites before refreshing them
 */
gba_oam_struct gba_sprite_oam_shadow;

/*
 * Entry in the OAM for the next sprite that is believed
 */
u_short gba_sprite_next_obj_entry = 0;

/*
 * Character memory shift for the next sprite
 */
u_short gba_sprite_next_obj_name = 0;

/*
 * Copy of MOSAIC record. This is necessary because MOSAIC registration is not
 * can read. It's just writing. Defined in gba_bg.c.
 */
extern short gba_bg_mosaic_shadow;

/*****************************************************************************/

/*
 * Set the storage mode of the sprites in the VRAM in 1 dimension
 * And initializes the OAM (hides all sprites and sets the rotation parameters).
 */
inline void gba_oam_init()
{
  int i;

  /* We hide the sprites */
  for(i = 0; i < 128; i++)
  {
    gba_sprite_oam_shadow.obj[i].ATTRIBUTE[0] = SCREEN_HEIGHT;
    gba_sprite_oam_shadow.obj[i].ATTRIBUTE[1] = SCREEN_WIDTH;
  }

  /* We set all rotation matrices to the fixed-point identity matrix */
  for(i = 0; i < 32; i++)
  {
    gba_sprite_oam_shadow.rotation[i].PA = I2FP(1); /* Dx */
    gba_sprite_oam_shadow.rotation[i].PC = I2FP(0); /* Dy */
    gba_sprite_oam_shadow.rotation[i].PB = I2FP(0); /* Dmx */
    gba_sprite_oam_shadow.rotation[i].PD = I2FP(1); /* Dmy */
  }
}

/*****************************************************************************/

/*
 * Refresh the sprites on the screen
 */
inline void gba_oam_update(void)
{
  gba_dma_memcpy((void*) &gba_oam, (void*)&gba_sprite_oam_shadow, sizeof(gba_oam_struct));
}

/*****************************************************************************/

/*
 * Function to enable the sprites.
 */
inline void gba_sprite_enable()
{
  /* Sprites are stored in 1D */
  gba_video_regs.DISPCNT |= VIDEO_OBJ_MAP_1D | VIDEO_OBJ;
}

/*****************************************************************************/

/*
 * Function to disable sprites.
 */
inline void gba_sprite_disable()
{
  gba_video_regs.DISPCNT &= ~VIDEO_OBJ;
}

/*****************************************************************************/

/*
 * Initialize a 256-color palette for sprites.
 * @param palette The palette
 */
inline void gba_sprite_init_palette256(const void * palette)
{
  gba_dma_memcpy((void*) gba_palette.obj256, palette, sizeof(gba_palette256));
}

/*****************************************************************************/

/*
 * Initiate one of 16 16 color palettes for sprites.
 * @param index Palette index (0..15)
 * @param palette The palette
 */
inline void gba_sprite_init_palette16(short index, const void * palette)
{
  gba_dma_memcpy((void*) gba_palette.obj16[index], palette, sizeof(gba_palette16));
}

/*****************************************************************************/

/*
 * Create a new sprite
 * @param sp Pointer to sprite
 * @param size Sprite size (SPRITE_SIZE_X)
 * @param Color Color Format (SPRITE_COLOR16, SPRITE_COLOR256)
 * @param data Pointer to the buffer containing the frames that define the sprite
 * @return The sprite created.
 */
void gba_sprite_new(sprite_struct * sp, short size, short color, const void *data, int len)
{
  short obj_size = (size & 0xc000);
  short obj_shape = (size & 0x0003) << 14;
  
  /*
   * If the screen is in graphic mode, only half of the screen is available.
   * Memory for sprites
   */
  if (gba_sprite_next_obj_name==0 && (gba_bg_get_mode() & (BG_MODE3|BG_MODE4|BG_MODE5)))
    gba_sprite_next_obj_name = 512;

  /* Initialize the sprite fields */
  sp->oam_entry = gba_sprite_next_obj_entry++; /* Entry into the OAM */
  sp->is_shown = FALSE;             /* Initially hidden */
  sp->pos_x=0;                      /* Default position */
  sp->pos_y=0;
  sp->scale_h=I2FP(1);              /* Scaling by default */
  sp->scale_v=I2FP(1);
  sp->angle=0;                      /* Default Rotation */
  sp->rotated=FALSE;                /* No rotation / scaling applied */
  sp->data = data;
  
  /* Initialize height and width */
  switch (obj_shape)
  {
    case OBJ_SQUARE:
      switch (obj_size)
      {
        case OBJ_SIZE8:
          sp->width = 8;
          sp->height = 8;
          break;
        case OBJ_SIZE16:
          sp->width = 16;
          sp->height = 16;
          break;
        case OBJ_SIZE32:
          sp->width = 32;
          sp->height = 32;
          break;
        case OBJ_SIZE64:
          sp->width = 64;
          sp->height = 64;
          break;
      }  
      break;

    case OBJ_WIDE:
      switch (obj_size)
      {
        case OBJ_SIZE8:
          sp->width = 16;
          sp->height = 8;
          break;
        case OBJ_SIZE16:
          sp->width = 32;
          sp->height = 8;
          break;
        case OBJ_SIZE32:
          sp->width = 32;
          sp->height = 16;
          break;
        case OBJ_SIZE64:
          sp->width = 64;
          sp->height = 32;
          break;
      }  
      break;
      
    case OBJ_TALL:
      switch (obj_size)
      {
        case OBJ_SIZE8:
          sp->width = 8;
          sp->height = 16;
          break;
        case OBJ_SIZE16:
          sp->width = 8;
          sp->height = 32;
          break;
        case OBJ_SIZE32:
          sp->width = 16;
          sp->height = 32;
          break;
        case OBJ_SIZE64:
          sp->width = 32;
          sp->height = 64;
          break;
      }  
      break;
  }

  /* Initialize its attributes (pos_x and pos_y and set to 0) */
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= color | obj_shape;
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] |= obj_size;
  
  /* Size in transfer pixels */
  sp->frame_size = sp->width * sp->height;

  /* Transfer size in bytes */
  if (color == SPRITE_COLOR16)
    sp->frame_size /= 2;

  /* Copy the characters from the first frame to the sprite */
  gba_dma_memcpy((void*)gba_vram.obj_data[gba_sprite_next_obj_name], data, len);
  
  /* We set the offset for the next sprite */
  gba_sprite_next_obj_name += sp->frame_size/sizeof(gba_video_char16);
}

/*****************************************************************************/

/*
 * Function to update the position, rotation and scaling of a sprite in the
 * OAM.
 * @param sp Pointer to sprite
 */
void gba_sprite_update(sprite_struct * sp)
{
  fixed_p dx, dy, dmx, dmy;
  short rot_index;
 
  if (sp->is_shown)
  {
    gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] &= 0xfe00;
    gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] |= (sp->pos_x & 0x01ff);

    gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] &= 0xff00;
    gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= (sp->pos_y & 0x00ff);
    
    /* If any rotation / scaling has been applied */
    if(sp->rotated)
    {
      sp->rotated = FALSE;
      /* Displacement on the x-axis of the same line */
      dx = gba_fp_mult(sp->scale_h, gba_fp_cos(sp->angle));

      /* Displacement on the y axis of the same line */
      dy = gba_fp_mult(sp->scale_h, -gba_fp_sin(sp->angle));

      /* Displacement on the x-axis of the next line */
      dmx = gba_fp_mult(sp->scale_v, gba_fp_sin(sp->angle));

      /* Offset on the y-axis of the next line */
      dmy = gba_fp_mult(sp->scale_v, gba_fp_cos(sp->angle));

      rot_index = (gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] & (31 << 9)) >> 9;

      /* We update the OAM */
      gba_sprite_oam_shadow.rotation[rot_index].PA = dx;
      gba_sprite_oam_shadow.rotation[rot_index].PC = dy;
      gba_sprite_oam_shadow.rotation[rot_index].PB = dmx;
      gba_sprite_oam_shadow.rotation[rot_index].PD = dmy;
    }
  }
}

/*****************************************************************************/

/*
 * Show a sprite.
 * @param sp The sprite
 */
inline void gba_sprite_show(sprite_struct *sp)
{
  sp->is_shown = 1;
}

/*****************************************************************************/

/*
 * Hides a sprite.
 * @param sp The sprite
 */
inline void gba_sprite_hide(sprite_struct *sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] &= 0xfe00;
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] |= SCREEN_WIDTH;

  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] &= 0xff00;
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= SCREEN_HEIGHT;

  sp->is_shown = 0;
}

/*****************************************************************************/

/*
 * Sets the starting position of a sprite.
 * @param sp The sprite
 * @param x Position in x
 * @param and Position in y
 */
inline void gba_sprite_set_pos(sprite_struct *sp, short x, short y)
{
  sp->pos_x = x;
  sp->pos_y = y;
}

/*****************************************************************************/

/*
 * Move a sprite in the direction indicated by the mask.
 * @param sp The sprite
 * @param dx Offset x
 * @param dy Offset in y
 */
void gba_sprite_move(sprite_struct *sp, short dx, short dy)
{
  gba_sprite_set_pos(sp, sp->pos_x+dx, sp->pos_y+dy);
}

/*****************************************************************************/

/*
 * Enable alpha-blending for a sprite
 * @param sp Pointer to sprite
 */
inline void gba_sprite_enable_bld(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= OBJ_MODE_BLD;
}

/*****************************************************************************/

/*
 * Overrides alpha-blending for a sprite
 * @param sp Pointer to sprite
 */
inline void gba_sprite_disable_bld(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] &= ~OBJ_MODE_BLD;
}

/*****************************************************************************/

/*
 * Flip the sprite horizontally
 * @param sp Pointer to sprite
 */
inline void gba_sprite_flip_h(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] ^= OBJ_HORIZONTAL_FLIP;
}

/*****************************************************************************/

/*
 * Flip the sprite vertically
 * @param sp Pointer to sprite
 */
inline void gba_sprite_flip_v(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] ^= OBJ_VERTICAL_FLIP;
}

/*****************************************************************************/

/*
 * Enables rotation and scaling of a sprite
 * @param sp Pointer to sprite
 * @param rot_index Index in the rotation table for parameters
 */
inline void gba_sprite_enable_rotation(sprite_struct * sp, short rot_index)
{
  /* We activate the rotation for the sprite */
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= OBJ_ROTATION;
  
  /* Fix your entry in the rotation table */
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] &= ~OBJ_ROTDATA(31);
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[1] |= OBJ_ROTDATA(rot_index);
}

/*****************************************************************************/

/*
 * Sets the initial hardware rotation angle for a sprite.
 * @param sp Pointer to sprite
 * @param angle Angle of rotation in whole degrees
 */
inline void gba_sprite_set_angle(sprite_struct * sp, int angle)
{
  sp->angle = angle;
  sp->rotated = TRUE;
}

/*****************************************************************************/

/*
 * Modifies the current rotation angle of the sprite.
 * @param sp Pointer to sprite
 * @param dangle Increment / decrease to current rotation angle
 */
inline void gba_sprite_rotate(sprite_struct * sp, int dangle)
{
  sp->angle += dangle;
  sp->rotated = TRUE;
}

/*****************************************************************************/

/*
 * Sets the initial hardware scaling for a sprite.
 * @param sp Pointer to sprite
 * @param scale_h Scaled horizontal
 * @param scale_v Vertical scaling
 */
inline void gba_sprite_set_scale(sprite_struct * sp, fixed_p scale_h, fixed_p scale_v)
{
  sp->scale_h = scale_h;
  sp->scale_v = scale_v;
  sp->rotated = TRUE;
}

/*****************************************************************************/

/*
 * If the rotation of the sprite is activated, double the size of the sprite.
 * Otherwise, the sprite is hidden
 * @param sp Pointer to sprite
 */
void inline gba_sprite_enable_double_size(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= OBJ_SIZE_DOUBLE;
}

/*****************************************************************************/

/*
 * If sprite rotation is enabled, resize the sprite
 * Otherwise, display the sprite again.
 * @param sp Pointer to sprite
 */
void inline gba_sprite_disable_double_size(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] &= ~OBJ_SIZE_DOUBLE;
}

/*****************************************************************************/

/*
 * Set the frame of the animation
 * @param sp Pointer to sprite
 * @param offset Frame offset of the animation
 */
void gba_sprite_set_frame(sprite_struct * sp, short offset)
{
  /* clear the old offset */
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[2] &= 0xfc00;

  /* apply the new one */
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[2] |= (offset & 0x03ff);
}

/*****************************************************************************/

/*
 * Sets the pixel size of the sprites in mosaic mode
 * @param h_size Horizontal mosaic size of backgrounds (1-16)
 * @param v_size Vertical mosaic size of backgrounds (1-16)
 */
inline void gba_sprite_set_mosaic_size(short h_size, short v_size)
{
  gba_bg_mosaic_shadow &= 0x00ff;
  gba_bg_mosaic_shadow |= (((v_size-1)  & 0x000f) << 12 ) | (((h_size-1) & 0x000f) << 8);
  gba_video_regs.MOSAIC =  gba_bg_mosaic_shadow;
}

/*****************************************************************************/

/*
 * Enable mosaic mode in the sprite.
 * @param sp Pointer to sprite
 */
void gba_sprite_enable_mosaic(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] |= OBJ_MOSAIC;
}

/*****************************************************************************/

/*
 * Disables mosaic mode in the sprite.
 * @param sp Pointer to sprite
 */
void gba_sprite_disable_mosaic(sprite_struct * sp)
{
  gba_sprite_oam_shadow.obj[sp->oam_entry].ATTRIBUTE[0] &= ~OBJ_MOSAIC;
}

/*****************************************************************************/

/*
 * Check if two sprites overlap.
 * @param sp1 Pointer to first sprite
 * @param sp2 Pointer to second sprite
 * @return TRUE if overlap or FALSE otherwise
 */
char gba_sprite_overlap(sprite_struct * sp1, sprite_struct * sp2)
{
  char overlap = FALSE;

  /* Photos of sp1 */
  short min_x1 = sp1->pos_x;
  short min_y1 = sp1->pos_y;
  short max_x1 = sp1->pos_x + sp1->width - 1;
  short max_y1 = sp1->pos_y + sp1->height -1;

  /* Photos of sp2 */
  short min_x2 = sp2->pos_x;
  short min_y2 = sp2->pos_y;
  short max_x2 = sp2->pos_x + sp2->width - 1;
  short max_y2 = sp2->pos_y + sp2->height -1;
  
  /* I check for overlap */
  if (min_x2 >= min_x1 && min_x2 <= max_x1 &&
      min_y2 >= min_y1 && min_y2 <= max_y1)
    overlap = TRUE;
  else if (max_x2 >= min_x1 && max_x2 <= max_x1 &&
           min_y2 >= min_y1 && min_y2 <= max_y1)
    overlap = TRUE;
  else if (min_x2 >= min_x1 && min_x2 <= max_x1 &&
           max_y2 >= min_y1 && max_y2 <= max_y1)
    overlap = TRUE;
  else if (max_x2 >= min_x1 && max_x2 <= max_x1 &&
           max_y2 >= min_y1 && max_y2 <= max_y1)
    overlap = TRUE;
  else if (min_x1 >= min_x2 && min_x1 <= max_x2 &&
           min_y1 >= min_y2 && min_y1 <= max_y2)
    overlap = TRUE;
  else if (max_x1 >= min_x2 && max_x1 <= max_x2 &&
           min_y1 >= min_y2 && min_y1 <= max_y2)
    overlap = TRUE;
  else if (min_x1 >= min_x2 && min_x1 <= max_x2 &&
           max_y1 >= min_y2 && max_y1 <= max_y2)
    overlap = TRUE;
  else if (max_x1 >= min_x2 && max_x1 <= max_x2 &&
           max_y1 >= min_y2 && max_y1 <= max_y2)
    overlap = TRUE;

  return overlap;
}

/*****************************************************************************/

