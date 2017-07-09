/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Data types and constants to handle the serial port of the GBA */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef serial_h
#define serial_h

#include    "std_c.h"

/*****************************************************************************/
/* Console serial port interface */
/* Actually the registers to control the serial port are separated */
/* By the keyboard management records. */
/* In this case, to avoid handling two independent structures, */
/* One for each part of the registers, a single */
/* Structure in which the positions in which */
/* Find keyboard logs as unused */
/*****************************************************************************/

typedef struct
{
  union  /* These records share the same position */
  {
    int SIODATA32;     /* SIO data (normal-32bit mode) */
    struct
    {
      short SIOMULTI0; /* SIO data 0 (parent) (multi-player mode) */
      short SIOMULTI1; /* SIO data 1 (1st child) (multi-player mode) */
    } __attribute__ ((packed));
  } __attribute__ ((packed));

  short SIOMULTI2;     /* SIO data 2 (2nd child) (multi-player mode) */
  short SIOMULTI3;     /* SIO data 3 (3rd child) (multi-player mode) */
  short SIOCNT;        /* SIO control register */

  union /* These two registers share the same position */
  {
    short SIOMLT_SEND; /* SIO data (local of multi-player) */
    short SIODATA8;    /* SIO data (normal-8bit and uart mode) */
  } __attribute__ ((packed));  /* para evitar que fije el tamaño de la union a
                                  un múltiplo de 4 bytes (por defecto) */
  short filler1[4];
  short RCNT;          /* SIO mode select / general purpose data */
  short IR;            /* Infrared register (prototypes only) */
  short filler2[4];
  short JOYCNT;        /* SIO JOY bus control */
  short filler3[7];
  int   JOY_RECV;      /* SIO JOY bus receive data */
  int   JOY_TRANS;     /* SIO JOY bus transmit data */
  short JOYSTAT;       /* SIO JOY bus receive status */
} gba_serial_regs_struct;

extern volatile gba_serial_regs_struct gba_serial_regs;

/*****************************************************************************/

#endif /* serial_h */

