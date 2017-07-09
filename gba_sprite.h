/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling sprites */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_sprite_h
#define gba_sprite_h

#include "gba.h"

/*****************************************************************************/

/*
 * Color Modes
 */
#define SPRITE_COLOR16             OBJ_COLOR16
#define SPRITE_COLOR256            OBJ_COLOR256

/*
 * Forms of sprites
 */
#define SPRITE_SQUARE              0
#define SPRITE_WIDE                1
#define SPRITE_TALL                2

/*
 * Sizes in pixels for text backgrounds without rotation
 */
#define SPRITE_SIZE_8x8            (SPRITE_SQUARE | OBJ_SIZE8)
#define SPRITE_SIZE_16x16          (SPRITE_SQUARE | OBJ_SIZE16)
#define SPRITE_SIZE_32x32          (SPRITE_SQUARE | OBJ_SIZE32)
#define SPRITE_SIZE_64x64          (SPRITE_SQUARE | OBJ_SIZE64)
#define SPRITE_SIZE_16x8           (SPRITE_WIDE   | OBJ_SIZE8)
#define SPRITE_SIZE_32x8           (SPRITE_WIDE   | OBJ_SIZE16)
#define SPRITE_SIZE_32x16          (SPRITE_WIDE   | OBJ_SIZE32)
#define SPRITE_SIZE_64x32          (SPRITE_WIDE   | OBJ_SIZE64)
#define SPRITE_SIZE_8x16           (SPRITE_TALL   | OBJ_SIZE8)
#define SPRITE_SIZE_8x32           (SPRITE_TALL   | OBJ_SIZE16)
#define SPRITE_SIZE_16x32          (SPRITE_TALL   | OBJ_SIZE32)
#define SPRITE_SIZE_32x64          (SPRITE_TALL   | OBJ_SIZE64)

/*****************************************************************************/

/*
 * Data structure for handling sprites
 */
typedef struct
{
  short oam_entry;         /* Entry into the OAM */
  short is_shown;          /* If it is nonzero, the sprite */
  char  width;             /* Width of sprite */
  char  height;            /* Top of the sprite */
  short pos_x;             /* Position on the x-axis */
  short pos_y;             /* Position on the y-axis */
  fixed_p scale_h;         /* Horizontal scaling factor */
  fixed_p scale_v;         /* Vertical scaling factor */
  int angle;               /* Angle of rotation in degrees */
  char rotated;            /* TRUE if the sprite has been rotated / clicked */
  const void  *data;       /* Pointer to sprite characters */
  short frame_size;        /* Size of a frame in bytes */
  const short *frame_seq;  /* Sequence of frames to define an animation */
  short frame_seq_len;     /* Length of the sequence */
  short frame_index;       /* Index in the current frame sequence */
} sprite_struct;


/*****************************************************************************/

/*
 * Set the storage mode of the sprites in the VRAM in 1 dimension
 * And initializes the OAM (hides all sprites and sets the rotation parameters).
 */
inline void gba_oam_init();

/*****************************************************************************/

/*
 * Refresh the sprites on the screen
 */
inline void gba_oam_update(void);

/*****************************************************************************/

/*
 * Function to enable the sprites.
 */
inline void gba_sprite_enable();

/*****************************************************************************/

/*
 * Function to disable sprites.
 */
inline void gba_sprite_disable();

/*****************************************************************************/

/*
 * Initialize a 256-color palette for sprites.
 * @param palette The palette
 */
inline void gba_sprite_init_palette256(const void * palette);

/*****************************************************************************/

/*
 * Initiate one of 16 16 color palettes for sprites.
 * @param index Palette index (0..15)
 * @param palette The palette
 */
inline void gba_sprite_init_palette16(short index, const void * palette);

/*****************************************************************************/

/*
 * Create a new sprite
 * @param sp Pointer to sprite
 * @param size Sprite size (SPRITE_SIZE_X)
 * @param Color Color Format (SPRITE_COLOR16, SPRITE_COLOR256)
 * @param data Pointer to the buffer containing the frames that define the sprite
 * @return The sprite created.
 */
void gba_sprite_new(sprite_struct * sp, short size, short color, const void *data);

/*****************************************************************************/

/*
 * Function to update the position, rotation and scaling of a sprite in the
 * OAM.
 * @param sp Pointer to sprite
 */
void gba_sprite_update(sprite_struct * sp);

/*****************************************************************************/

/*
 * Show a sprite.
 * @param sp The sprite
 */
inline void gba_sprite_show(sprite_struct *sp);

/*****************************************************************************/

/*
 * Hides a sprite.
 * @param sp The sprite
 */
inline void gba_sprite_hide(sprite_struct *sp);

/*****************************************************************************/

/*
 * Sets the starting position of a sprite.
 * @param sp The sprite
 * @param x Position in x
 * @param and Position in y
 */
inline void gba_sprite_set_pos(sprite_struct *sp, short x, short y);

/*****************************************************************************/

/*
 * Move a sprite in the direction indicated by the mask.
 * @param sp The sprite
 * @param dx Offset x
 * @param dy Offset in y
 */
void gba_sprite_move(sprite_struct *sp, short dx, short dy);

/*****************************************************************************/

/*
 * Enable alpha-blending for a sprite
 * @param sp Pointer to sprite
 */
inline void gba_sprite_enable_bld(sprite_struct * sp);

/*****************************************************************************/

/*
 * Overrides alpha-blending for a sprite
 * @param sp Pointer to sprite
 */
inline void gba_sprite_disable_bld(sprite_struct * sp);

/*****************************************************************************/

/*
 * Flip the sprite horizontally
 * @param sp Pointer to sprite
 */
inline void gba_sprite_flip_h(sprite_struct * sp);

/*****************************************************************************/

/*
 * Flip the sprite vertically
 * @param sp Pointer to sprite
 */
inline void gba_sprite_flip_v(sprite_struct * sp);

/*****************************************************************************/

/*
 * Enables rotation and scaling of a sprite
 * @param sp Pointer to sprite
 * @param rot_index Index in the rotation table for parameters
 */
inline void gba_sprite_enable_rotation(sprite_struct * sp, short rot_index);

/*****************************************************************************/

/*
 * Sets the initial hardware rotation angle for a sprite.
 * @param sp Pointer to sprite
 * @param angle Angle of rotation in whole degrees
 */
inline void gba_sprite_set_angle(sprite_struct * sp, int angle);

/*****************************************************************************/

/*
 * Modifies the current rotation angle of the sprite.
 * @param sp Pointer to sprite
 * @param dangle Increment / decrease to current rotation angle
 */
inline void gba_sprite_rotate(sprite_struct * sp, int dangle);

/*****************************************************************************/

/*
 * Sets the initial hardware scaling for a sprite.
 * @param sp Pointer to sprite
 * @param scale_h Scaled horizontal
 * @param scale_v Vertical scaling
 */
inline void gba_sprite_set_scale(sprite_struct * sp, fixed_p scale_h, fixed_p scale_v);

/*****************************************************************************/

/*
 * If the rotation of the sprite is activated, double the size of the sprite.
 * Otherwise, the sprite is hidden
 * @param sp Pointer to sprite
 */
void inline gba_sprite_enable_double_size(sprite_struct * sp);

/*****************************************************************************/

/*
 * If sprite rotation is enabled, resize the sprite
 * Otherwise, display the sprite again.
 * @param sp Pointer to sprite
 */
void inline gba_sprite_disable_double_size(sprite_struct * sp);

/*****************************************************************************/

/*
 * Define a sequence of frames to create the animation of the sprite
 * @param sp Pointer to sprite
 * @param seq Vector that defines the sequence
 * @param len Length of the sequence
 */
void inline gba_sprite_set_frame_seq(sprite_struct * sp, const short* seq, short len);

/*****************************************************************************/

/*
 * Move to the next frame of the animation
 * @param sp Pointer to sprite
 */
void gba_sprite_next_frame(sprite_struct * sp);

/*****************************************************************************/

/*
 * Sets the pixel size of the sprites in mosaic mode
 * @param h_size Horizontal mosaic size of backgrounds (1-16)
 * @param v_size Vertical mosaic size of backgrounds (1-16)
 */
inline void gba_sprite_set_mosaic_size(short h_size, short v_size);

/*****************************************************************************/

/*
 * Enable mosaic mode in the sprite.
 * @param sp Pointer to sprite
 */
void gba_sprite_enable_mosaic(sprite_struct * sp);

/*****************************************************************************/

/*
 * Disables mosaic mode in the sprite.
 * @param sp Pointer to sprite
 */
void gba_sprite_disable_mosaic(sprite_struct * sp);

/*****************************************************************************/

/*
 * Check if two sprites overlap.
 * @param sp1 Pointer to first sprite
 * @param sp2 Pointer to second sprite
 * @return TRUE if overlap or FALSE otherwise
 */
char gba_sprite_overlap(sprite_struct * sp1, sprite_struct * sp2);

/*****************************************************************************/

#endif /* gba_sprite_h */
