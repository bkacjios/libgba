
/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling the buttons in the GBA */
/*                                                                           */
/*****************************************************************************/

#include "gba_input.h"

/*****************************************************************************/

/*
 *  Returns the status of the buttons (0 -> pressed)
 */
inline short gba_input_getstatus()
{
  return gba_input_regs.KEYINPUT;
}

/*****************************************************************************/

/*
 *  Returns 0 if any of the selected buttons on the mask are pressed.
 *  @param masc Mask to select buttons
 */
inline short gba_input_released(short masc)
{
  return ((gba_input_getstatus() & masc) == masc);
}

/*****************************************************************************/

/*
 *  Returns 0 if none of the selected buttons on the mask are pressed.
 *  @param status Button status
 *  @param masc Mask to select buttons
 */
inline short gba_input_pressed(short masc)
{
  return !(gba_input_released(masc));
}

/*****************************************************************************/

