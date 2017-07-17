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
#ifndef input_h
#define input_h

#include    "std_c.h"

/*****************************************************************************/
/* Interface of the controller the buttons of the GBA */
/*****************************************************************************/

typedef struct
{
  short KEYINPUT;      /* Button status */
  short KEYCNT;        /* Interruption generation */
} gba_input_regs_struct;

extern volatile gba_input_regs_struct gba_input_regs;


/*****************************************************************************/
/* KEYINPUT: Flags to check the status of buttons */
/*****************************************************************************/

#define KEY_A                   ((short) (1 << 0))  /* 0 = pressed, 1 = released */
#define KEY_B                   ((short) (1 << 1))
#define KEY_SELECT              ((short) (1 << 2))
#define KEY_START               ((short) (1 << 3))
#define KEY_RIGHT               ((short) (1 << 4))
#define KEY_LEFT                ((short) (1 << 5))
#define KEY_UP                  ((short) (1 << 6))
#define KEY_DOWN                ((short) (1 << 7))
#define KEY_R                   ((short) (1 << 8))
#define KEY_L                   ((short) (1 << 9))


/*****************************************************************************/
/* KEYCNT: Interrupt Generation */
/*****************************************************************************/

/*
 *  Generation of an interrupt
 */
#define INPUT_INT                 ((short) (1 << 14))

/*
 *  Generating an interrupt when multiple buttons are pressed
 */
#define INPUT_OR                  ((short) (0 << 15))
#define INPUT_AND                 ((short) (1 << 15))

/*****************************************************************************/

#endif /* input_h */
