/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to handle the VRAM memory of the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef vram_h
#define vram_h

#include    "std_c.h"

/*****************************************************************************/
/* Display dimensions */
/*****************************************************************************/

#define BG_MODE_WIDTH3   240
#define BG_MODE_HEIGHT3  160
#define BG_MODE_WIDTH5   160
#define BG_MODE_HEIGHT5  128


/*****************************************************************************/
/* Format of the RBG triplets in the VRAM */
/*****************************************************************************/

#define RGB5(r,g,b) ((r)|((g)<<5)|((b)<<10))
#define RGB8(r,g,b) ( (((b)>>3)<<10) | (((g)>>3)<<5) | ((r)>>3) )

/*****************************************************************************/
/* VRAM memory interface */
/*****************************************************************************/

/*
 * In text modes, each character consists of 8x8 matrices
 * Pixels, and each pixel occupies 4 bits (for 16-color palettes) or one byte
 * (For 256-color palettes).
 */
typedef char gba_video_char16[32];
typedef char gba_video_char256[64];

/*
 * In text modes, the VRAM is divided into 4 character blocks, from
 * 16 KB each, in which the characters will be stored to define the
 * money.
 */
typedef gba_video_char256 gba_video_char_block[256];

/*
 * To define a wallpaper, a map is used that indicates which character of the
 * Character block will be used for each character on the screen
 *  * In the text backgrounds each map entry is 2 bytes:
 *     | Palette 16 (4 bits) | FlipV (1 bit) | FlipH (1 bit) | Index (10 bits) |
 *  * In the rotation backgrounds each entry occupies one byte:
 *     | Index of character (8 bits) |
 * In text mode, the 256x512, 512x256, and 512x512 maps do not fit into a
 * Screen-block, so they split in two (256x512 and 512x256) or four blocks
 * (512x512)
 */
typedef short gba_video_screen_block[1024];

/*
 * Structure to access the different zones of the VRAM
 */
typedef union
{
  struct /* Text modes */
  {
    union
    {
      gba_video_char_block char_block[4];
      gba_video_screen_block screen_block[32];
    } __attribute__ ((packed));
    
    /*
     * The memory for storing the sprites is organized in characters of
     * 16 colors. If 256 color sprites are used, each character in the sprite
     * Will occupy two characters in the memory.
     */
    gba_video_char16 obj_data[1024];
  };
  
  short video_buffer[BG_MODE_WIDTH3*BG_MODE_HEIGHT3]; /* Mode 3 */
    
  struct /* Modes 4 and 5 */
  {
    short front_buffer[BG_MODE_WIDTH5*BG_MODE_HEIGHT5];
    short back_buffer[BG_MODE_WIDTH5*BG_MODE_HEIGHT5];
  };
}gba_vram_struct;

extern volatile gba_vram_struct gba_vram;

/*****************************************************************************/

#endif /* vram_h */
