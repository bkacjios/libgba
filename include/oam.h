/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to handle the OAM memory of the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef oam_h
#define oam_h

#include    "std_c.h"

/*****************************************************************************/
/* OAM memory interface */
/*****************************************************************************/

/*
 * Attributes of each sprite
 */
typedef struct
{
  u_short ATTRIBUTE[3];  /* Attributes of each sprite */
  u_short filler;        /* filling */
}gba_oam_obj_entry;

/*
 * Parameters for sprites rotation / scaling
 */
typedef struct
{
  u_short filler1[3];  /* filling */
  u_short PA;          /* Rotation / scaling parameter a (dx) */
  u_short filler2[3];  /* filling */
  u_short PB;          /* Rotation / scaling parameter b (dmx) */
  u_short filler3[3];  /* filling */
  u_short PC;          /* Rotation / scaling parameter c (dy) */
  u_short filler4[3];  /* filling */
  u_short PD;          /* Rotation / scaling parameter d (dmy) */
}gba_oam_rotation_entry;

/*
 * Structure to handle the OAM
 */
typedef union
{
  gba_oam_obj_entry obj[128];
  gba_oam_rotation_entry rotation[32];
}gba_oam_struct;

extern volatile gba_oam_struct gba_oam;


/*****************************************************************************/
/* Attribute 0 */
/*****************************************************************************/

/*
 * Y coordinate of the sprite ([-128,127])
 */
#define OBJ_Y(n)                   (((short)(n)) & 0x00ff)

/*
 * Activates sprite rotation
 */
#define OBJ_ROTATION               ((short) (1 << 8))

/*
 * If rotation is enabled, bit 9 doubles the size of the sprite
 */
#define OBJ_SIZE_DOUBLE            ((short) (1 << 9))
                                              
/*
 * If rotation is NOT enabled, bit 9 prevents sprite from being displayed
 */
#define OBJ_SPRITE_OFF             ((short) (1 << 9))

/*
 * Mode flags. Control alpha blending
 */
#define OBJ_MODE_NORMAL            ((short) (0 << 10))
#define OBJ_MODE_BLD               ((short) (1 << 10))
#define OBJ_MODE_WINDOWED          ((short) (1 << 11))

/*
 * Turn mosaic mode on
 */
#define OBJ_MOSAIC                 ((short) (1 << 12))

/*
 * Select between 16 or 256 colors
 */
#define OBJ_COLOR16                ((short) (0 << 13))
#define OBJ_COLOR256               ((short) (1 << 13))

/*
 * Shape of the sprite
 */
#define OBJ_SQUARE                 ((short) (0 << 14))
#define OBJ_WIDE                   ((short) (1 << 14))
#define OBJ_TALL                   ((short) (1 << 15))


/*****************************************************************************/
/* Attribute 1 */
/*****************************************************************************/

/*
 * Sprite X coordinate ([-256,255])
 */
#define OBJ_X(n)                   (((short)(n)) & 0x01ff)

/*
 * Selecting the rotation / scaling parameters (if rotation is enabled)
 */
#define OBJ_ROTDATA(n)			       ((((short) (n)) & 0x001f) << 9)

/*
 * Flags to flip the sprite (if rotation is not active)
 */
#define OBJ_HORIZONTAL_FLIP		     ((short) (1 << 12))
#define OBJ_VERTICAL_FLIP		       ((short) (1 << 13))

/*
 * Size of the sprite
 */
#define OBJ_SIZE8			             ((short) (0 << 14))
#define OBJ_SIZE16			           ((short) (1 << 14))
#define OBJ_SIZE32		 	           ((short) (2 << 14))
#define OBJ_SIZE64	 		           ((short) (3 << 14))

/*****************************************************************************/
/* Attribute 2 */
/*****************************************************************************/

/*
 * Index of the first character of the sprite in video memory
 */
#define OBJ_NAME(n)                (((short)(n)) & 0x03ff)

/*
 * Sprite priority
 */
#define OBJ_MAX_PRIOR              ((short) (0 << 10)) /* Maximum priority */
#define OBJ_MEDIUM_PRIOR           ((short) (1 << 10)) /* Medium priority */
#define OBJ_LOW_PRIOR              ((short) (2 << 10)) /* Low Priority */
#define OBJ_MIN_PRIOR              ((short) (3 << 10)) /* Minimum Priority */

/*
 * Sprite palette (only for 16-color)
 */
#define OBJ_PALETTE_16(n)		       ((((short) (n)) & 0x000f) << 12)

/*****************************************************************************/

#endif /* oam_h */
