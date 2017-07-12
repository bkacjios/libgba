/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic video library functions */
/*                                                                           */
/*****************************************************************************/

#include <stdlib.h>
#include "gba_draw.h"
#include "gba_bg.h"

/*****************************************************************************/

/*
 * Static library variables
 */

u_short gba_draw_def_buffer = FRONT_BUFFER; /* Default buffer to draw pixels */
u_short gba_draw_def_color = 0;         /* Default color to draw pixels */
u_char gba_draw_def_cursor_x = 0;       /* Default x position for cursor */
u_char gba_draw_def_cursor_y = 0;       /* Position and default cursor */
font_struct gba_draw_def_font;          /* Default font */

/*****************************************************************************/

/*
 * Function to set the buffer to be used for drawing in bitmap modes
 * With double buffer (modes 4 and 5).
 * @param buffer Buffer ID (FRONT_BUFFER or BACK_BUFFER)
 */
inline void gba_draw_set_buffer(u_short buffer)
{
  gba_draw_def_buffer = buffer;
}

/*****************************************************************************/

/*
 * Function to set the color to be used for drawing in bitmap modes.
 * For modes 3 and 5 it will be used as a 15 bit RGB triplet.
 * For mode 4 only the lower 8 bits will be used as the index of the palette.
 * @param color The color
 */
inline void gba_draw_set_color(u_short color)
{
  gba_draw_def_color = color;
}

/*****************************************************************************/

/*
 * Function to set cursor coordinates.
 * @param x coordinate x
 * @param and coordinate y
 */
inline void gba_draw_set_cursor(u_char x, u_char y)
{
  gba_draw_def_cursor_x = MIN(x, gba_bg_width(BG2));
  gba_draw_def_cursor_y = MIN(y, gba_bg_height(BG2));
}

/*****************************************************************************/

/*
 * Function to set the default font.
 * @param font The font
 */
inline void gba_draw_set_font(font_struct font)
{
  gba_draw_def_font = font;
}

/*****************************************************************************/

/*
 * Function for drawing a pixel in modes 3, 4 and 5
 * @param x x coordinate of the screen
 * @param and Coordinate and display
 */
void gba_draw_pixel(u_short x, u_short y)
{
  // TODO
  // This function should check that the arguments are correct
  // x should be lesser than SCREEN_WIDTH
  // x should be lesser than SCREEN_HEIGHT
  u_short pixel;
  gba_word16 video_buffer = gba_vram.video_buffer;
  short bg_mode = gba_bg_get_mode();
  u_short bg_width = gba_bg_width(BG2);
  u_short bg_height = gba_bg_height(BG2);
  u_short offset = x + bg_width*y;

  
  if (gba_draw_def_buffer==BACK_BUFFER)
    video_buffer = gba_vram.back_buffer;

  if (x<bg_width && y<bg_height)
  {
    if (bg_mode == BG_MODE3)
    {
      gba_vram.video_buffer[offset] = gba_draw_def_color;
    }
    else if (bg_mode == BG_MODE4)
    {
      offset = offset >> 1;
      pixel = video_buffer[offset];
      if (x & 1)
        video_buffer[offset] = ((gba_draw_def_color & 0x00ff) << 8) + (pixel & 0x00ff);
      else
        video_buffer[offset] = (pixel & 0xff00) + (gba_draw_def_color & 0x00ff);
    }
    else if (bg_mode == BG_MODE5)
    {
      video_buffer[offset] = gba_draw_def_color;
    }
  }
}

/*****************************************************************************/

/*
 * Function for drawing a line using the Bresenham algorithm
 * @param x1 x-coordinate of the first end
 * @param y1 Coordinate and the first end
 * @param x2 x coordinate of the second end
 * @param y2 Coordinate and the second end
 */
void gba_draw_line(u_short x1, u_short y1, u_short x2, u_short y2)
{
  // TODO
  // This function should correctly handle both negative values and
  // greater than SCREEN_WIDTH and SCREEN_HEIGHT values
  short i, deltax, deltay, numpixels;
  short d, dinc1, dinc2;
  short x, xinc1, xinc2;
  short y, yinc1, yinc2;
  
  // calculate deltaX and deltaY
  deltax = abs(x2 - x1);
  deltay = abs(y2 - y1);
  
  //initialize
  if(deltax >= deltay)
  {
    //If x is independent variable
    numpixels = deltax + 1;
    d = (2 * deltay) - deltax;
    dinc1 = deltay << 1;
    dinc2 = (deltay - deltax) << 1;
    xinc1 = 1;
    xinc2 = 1;
    yinc1 = 0;
    yinc2 = 1;
  }
  else
  {
    //if y is independent variable
    numpixels = deltay + 1;
    d = (2 * deltax) - deltay;
    dinc1 = deltax << 1;
    dinc2 = (deltax - deltay) << 1;
    xinc1 = 0;
    xinc2 = 1;
    yinc1 = 1;
    yinc2 = 1;
  }

  //move the right direction
  if(x1 > x2)
  {
    xinc1 = -xinc1;
    xinc2 = -xinc2;
  }
  if(y1 > y2)
  {
    yinc1 = -yinc1;
    yinc2 = -yinc2;
  }
  x = x1;
  y = y1;

  //draw the pixels
  for(i = 1; i < numpixels; i++)
  {
    gba_draw_pixel(x, y);
    if(d < 0)
    {
      d = d + dinc1;
      x = x + xinc1;
      y = y + yinc1;
    }
    else
    {
      d = d + dinc2;
      x = x + xinc2;
      y = y + yinc2;
    }
  }
}

/*****************************************************************************/

/*
 * Function to draw a circle
 * @param xCenter x coordinate of the center
 * @param yCenter Coordinate and center
 * @param radius Radio
 */
void gba_draw_circle(u_short xCenter, u_short yCenter, u_short radius)
{
  // TODO
  // This function should correctly handle both negative values and
  // greater than SCREEN_WIDTH and SCREEN_HEIGHT values

  // TODO
  // A function that drew a filled circle would be interesting.
  short i, deltax, deltay, numpixels;
  short x = 0;
  short y = radius;
  short p = 3 - 2 * radius;
  while (x <= y)
  {
    gba_draw_pixel(xCenter + x, yCenter + y);
    gba_draw_pixel(xCenter - x, yCenter + y);
    gba_draw_pixel(xCenter + x, yCenter - y);
    gba_draw_pixel(xCenter - x, yCenter - y);
    gba_draw_pixel(xCenter + y, yCenter + x);
    gba_draw_pixel(xCenter - y, yCenter + x);
    gba_draw_pixel(xCenter + y, yCenter - x);
    gba_draw_pixel(xCenter - y, yCenter - x);
    if (p < 0)
      p += 4 * x++ + 6;
    else
      p += 4 * (x++ - y--) + 10;
  }
}

/*****************************************************************************/

/*
 * Function to draw a colored box
 * @param left x-coordinate of the upper-left corner
 * @param top Coordinate and top-left corner
 * @param right x coordinate of the bottom-right corner
 * @param bottom Coordinate and bottom-right corner
 */
void gba_draw_box(u_short left, u_short top, u_short right, u_short bottom)
{
  // TODO
  // This function should correctly handle both negative values and
  // greater than SCREEN_WIDTH and SCREEN_HEIGHT values

  // TODO
  // A function that drew an empty box would be interesting.
  short i, deltax, deltay, numpixels;
  u_short x, y;

  for(y = top; y < bottom; y++)
    for(x = left; x < right; x++)
      gba_draw_pixel(x, y);
}

/*****************************************************************************/

/*
 * Function to draw a character on screen.
 * Previously the cursor should be set to the position where it will start
 * Write, which by default is (0,0).
 * No screen scroll or character characters have been implemented at this time.
 * control.
 * @param the_char The character
 */
void gba_draw_char(u_char the_char)
{
  // TODO
  // This function should correctly handle both negative values and
  // greater than SCREEN_WIDTH and SCREEN_HEIGHT values
  short deltax, deltay, numpixels;
  u_short i, j;          /* To traverse the character's bitmap */
  u_char pintar;         /* If TRUE, you must paint the character's pixel */
  u_short bitmaps_width; /* Total width of bitmaps in pixels */
  u_short offset;        /* First character pixel */
  
  /* I can only draw the characters that are in the font */
  if (the_char < gba_draw_def_font.first_ascii || the_char > gba_draw_def_font.last_ascii)
    return;
  
  bitmaps_width = (gba_draw_def_font.last_ascii - gba_draw_def_font.first_ascii + 1) * gba_draw_def_font.width;
  offset = the_char * gba_draw_def_font.width;

  for(j = 0; j < gba_draw_def_font.height ; j++)
  {
    for (i = 0; i < gba_draw_def_font.width; i++)
    {
        // Selecciono un pixel de la fuente
        pintar = ((u_char*)gba_draw_def_font.bitmaps)[offset+i];
        
        // Si el pintar = 1, se colorea el píxel en la pantalla
        if (pintar)
          gba_draw_pixel(gba_draw_def_cursor_x+i, gba_draw_def_cursor_y+j);
    }
    offset += bitmaps_width;
  }

  /* I select a pixel from the font */
  gba_draw_def_cursor_x += gba_draw_def_font.width;
  gba_draw_def_cursor_x = MIN(gba_draw_def_cursor_x, gba_bg_width(BG2));

}

/*****************************************************************************/

