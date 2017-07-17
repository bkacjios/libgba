/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling the buttons in the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef gba_input_h
#define gba_input_h

#include "gba.h"

/*****************************************************************************/

/*
 *  Returns the status of the buttons (0 -> pressed)
 */
extern short gba_input_getstatus();

/*****************************************************************************/

/*
 *  Returns 0 if any of the selected buttons on the mask are pressed.
 *  @param masc Mask to select buttons
 */
extern short gba_input_released(short masc);

/*****************************************************************************/

/*
 *  Returns 0 if none of the selected buttons on the mask are pressed.
 *  @param status Button status
 *  @param masc Mask to select buttons
 */
extern short gba_input_pressed(short masc);

/*****************************************************************************/

#endif /* gba_keypad_h */
