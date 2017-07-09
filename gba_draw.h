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
#ifndef gba_draw_h
#define gba_draw_h

#include "gba.h"

/*****************************************************************************/

/*
 * Identifiers for video buffers in dual-buffered modes
 */
#define FRONT_BUFFER           0
#define BACK_BUFFER            1

/*****************************************************************************/

/*
 * Structure to handle a screen font.
 */
typedef struct
{
  u_char        width;         /* Width of characters */
  u_char        height;        /* High characters */
  u_char        first_ascii;   /* ASCII code of the first character of the font */
  u_char        last_ascii;    /* ASCII code of the last character of the font */
  const u_char* bitmaps;       /* Pointer to character bitmaps */
} font_struct;

/*****************************************************************************/

/*
 * Function to set the buffer to be used for drawing in bitmap modes
 * With double buffer (modes 4 and 5).
 * @param buffer Buffer ID (FRONT_BUFFER or BACK_BUFFER)
 */
inline void gba_draw_set_buffer(u_short buffer);

/*****************************************************************************/

/*
 * Function to set the color to be used for drawing in bitmap modes.
 * For modes 3 and 5 it will be used as a 15 bit RGB triplet.
 * For mode 4 only the lower 8 bits will be used as the index of the palette.
 * @param color The color
 */
inline void gba_draw_set_color(u_short color);

/*****************************************************************************/

/*
 * Function to set cursor coordinates.
 * @param x coordinate x
 * @param and coordinate y
 */
inline void gba_draw_set_cursor(u_char x, u_char y);

/*****************************************************************************/

/*
 * Function to set the default font.
 * @param font The font
 */
inline void gba_draw_set_font(font_struct font);

/*****************************************************************************/

/*
 * Function for drawing a pixel in modes 3, 4 and 5
 * @param x x coordinate of the screen
 * @param and Coordinate and display
 */
void gba_draw_pixel(u_short x, u_short y);

/*****************************************************************************/

/*
 * Function for drawing a line using the Bresenham algorithm
 * @param x1 x-coordinate of the first end
 * @param y1 Coordinate and the first end
 * @param x2 x coordinate of the second end
 * @param y2 Coordinate and the second end
 */
void gba_draw_line(u_short x1, u_short y1, u_short x2, u_short y2);

/*****************************************************************************/

/*
 * Function to draw a circle
 * @param xCenter x coordinate of the center
 * @param yCenter Coordinate and center
 * @param radius Radio
 */
void gba_draw_circle(u_short xCenter, u_short yCenter, u_short radius);

/*****************************************************************************/

/*
 * Function to draw a colored box
 * @param left x-coordinate of the upper-left corner
 * @param top Coordinate and top-left corner
 * @param right x coordinate of the bottom-right corner
 * @param bottom Coordinate and bottom-right corner
 */
void gba_draw_box(u_short left, u_short top, u_short right, u_short bottom);

/*****************************************************************************/

/*
 * Function to draw a character on screen.
 * Previously the cursor should be set to the position where it will start
 * Write, which by default is (0,0).
 * No screen scroll or character characters have been implemented at this time.
 * control.
 * @param the_char The character
 */
void gba_draw_char(u_char the_char);

/*****************************************************************************/

#endif /* gba_draw_h */
