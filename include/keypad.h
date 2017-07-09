/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to manage GBA buttons */
/*                                                                           */
/*****************************************************************************/

/*
 *  We only include this file once
 */
#ifndef keypad_h
#define keypad_h

#include    "std_c.h"

/*****************************************************************************/
/* Interface of the controller the buttons of the GBA */
/*****************************************************************************/

typedef struct
{
  short KEYINPUT;      /* Button status */
  short KEYCNT;        /* Interruption generation */
} gba_keypad_regs_struct;

extern volatile gba_keypad_regs_struct gba_keypad_regs;


/*****************************************************************************/
/* KEYINPUT: Flags to check the status of buttons */
/*****************************************************************************/

#define KEYPAD_A                   ((short) (1 << 0))  /* 0 = pressed, 1 = released */
#define KEYPAD_B                   ((short) (1 << 1))
#define KEYPAD_SELECT              ((short) (1 << 2))
#define KEYPAD_START               ((short) (1 << 3))
#define KEYPAD_RIGHT               ((short) (1 << 4))
#define KEYPAD_LEFT                ((short) (1 << 5))
#define KEYPAD_UP                  ((short) (1 << 6))
#define KEYPAD_DOWN                ((short) (1 << 7))
#define KEYPAD_R                   ((short) (1 << 8))
#define KEYPAD_L                   ((short) (1 << 9))


/*****************************************************************************/
/* KEYCNT: Interrupt Generation */
/*****************************************************************************/

/*
 *  Generation of an interrupt
 */
#define KEYPAD_INT                 ((short) (1 << 14))

/*
 *  Generating an interrupt when multiple buttons are pressed
 */
#define KEYPAD_OR                  ((short) (0 << 15))
#define KEYPAD_AND                 ((short) (1 << 15))

/*****************************************************************************/

#endif /* keypad_h */
