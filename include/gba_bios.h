/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* GBA BIOS Function Library */
/*                                                                           */
/*****************************************************************************/

/*
 * We only include this file once
 */
#ifndef gba_gba_h
#define gba_gba_h

#include "gba.h"

/*****************************************************************************/

/*
 * Constants to create the bitmask of gba_register_ram_reset.
 */
#define ROM_RESTART		0
#define RAM_RESTART		1

#define RESET_EWRAM    (1 << 0) /* Do not use when WRAM is returned */
#define RESET_IWRAM    (1 << 1) /* Exclude the last 0x200 bytes */
#define RESET_PALETTE  (1 << 2)
#define RESET_VRAM     (1 << 3)
#define RESET_OAM      (1 << 4) /* Fill it with zeros: it does not hide the sprites */
#define RESET_SIO      (1 << 5) /* Switch to general-purpose mode */
#define RESET_SOUND    (1 << 6)
#define RESET_REMAINIG (1 << 7) /* Rest of records (less SIO and sound) */

#if	defined	( __thumb__ )
#define	swi_call(x)	 __asm ("SWI	  "#x"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	swi_call(x)	 __asm ("SWI	  "#x"	<< 16\n" :::"r0", "r1", "r2", "r3")
#endif

/*****************************************************************************/

/*
 * Function 0x00:
 * Soft reset.
 *  - Clears the IWRAM zone from 0x03007e00-0x03007fff
 *  - Initialize r0-r12 to zero
 *  - Initializes the stack pointers of the USER, SVC and IRQ modes
 *  - Switch to USER mode of execution
 *  - Jump to the address 0x08000000 or 0x02000000 depending on the content
 *    Of RETURN_ADDRESS_SEL (0x03007ffa):
 *      - 0x00: Jump to 0x08000000 (default)
 *      - other: Jump to the address 0x02000000
 */
extern void gba_soft_reset(int flags);

/*****************************************************************************/

/*
 * Function 0x01:
 * Register RAM reset.
 *  - Clears the I / O registers and memory zones specified in the
 *    Mask bits flags.
 *  - The area between 0x3007E00-0x3007FFF is changed.
 * @param flags: Mask formed with constants RESET_X
 * Bugs:
 * - SIODATA LSBs are always destroyed, even if bit 5 is not set
 * - Always set the screen blank because DISPCNT is initialized to 0x0080
 */
extern void gba_register_ram_reset(int flags);

/*****************************************************************************/

/*
 * Function 0x02:
 * Halt.
 *  - Changes the CPU to the low power mode, but the other circuits
 *    (Video, sound, series, etc.) continue to function.
 *  - Exits this way when an IRQ occurs (must be enabled)
 */
extern void gba_halt(void);

/*****************************************************************************/

/*
 * Function 0x03:
 * Stop.
 * - It changes the GBA to very low consumption mode. It stops the CPU, the clock,
 *   Sound, video, WIS clock, DMAs and timers
 * - You only get out of this state if a keyboard interrupt is received,
 *   The cartridge, or the serial port
 * - Preparation before calling the stop function:
 *     - Disable the video. If not, the image freezes,
 *       But battery is still running
 */
extern void gba_stop(void);

extern void gba_intr_wait(int clearFlag, int irq);
extern void gba_intr_wait_vblank(void);

/*****************************************************************************/

/*
 * Function 0x06:
 * Whole division
 * Returns the integer part of the division.
 * @param numerator Numerator (32 bit signed)
 * @param denominator Denominator (32 bits with sign)
 */
extern int gba_div(int numerator, int denominator);

/*****************************************************************************/

/*
 * Function 0x06:
 * Rest of the entire division.
 * Returns the rest of the division.
 * @param numerator Numerator (32 bit signed)
 * @param denominator Denominator (32 bits with sign)
 */
extern int gba_mod(int numerator, int denominator);

/*****************************************************************************/

/*
 * Function 0x06:
 * Whole division
 * Returns the absolute value of the integer part of the division.
 * @param numerator Numerator (32 bit signed)
 * @param denominator Denominator (32 bits with sign)
 */
extern int gba_abs_div(int numerator, int denominator);

extern int gba_arm_div(int denominator, int numerator);
extern u_int gba_sqrt(u_int val);
extern u_int gba_arc_tan(u_int val);
extern u_int gba_arc_tan2(u_int val1, u_int val2);
extern u_int gba_bios_checksum();

/*****************************************************************************/

/*
 * Function 0x08:
 * Square root.
 * @param val Value (32 bit unsigned)
 * @return The square root of val (16 bit unsigned). The result is whole.
 *         It is advisable to shift the value to the left 2 * N bits to get
 *         The result N bits shifted to the left.
 */
/*****************************************************************************/

/*
 * Function 0xff:
 * Prints a string of characters in the debugger console. Only works
 * In the Visualboy Advance simulator, if done in the console, leaves it
 * Hung up
 * @param str String pointer
 */
extern void gba_vba_print(const char *str);

/*****************************************************************************/

#endif /* gba_gba_h */
