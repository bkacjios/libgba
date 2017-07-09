/*****************************************************************************/
/*                                                                           */
/* Embedded Operating Systems */
/*                                                                           */
/* Basic functions library for handling interruptions in the GBA */
/*                                                                           */
/*****************************************************************************/

#include "gba_irq.h"

/*****************************************************************************/

#define IRQ_MAX    14

/*****************************************************************************/

/*
 * Main interrupt handler.
 * Identifies the source of the interrupt, calls the appropriate handler, and
 * Acknowledges his attention.
 * This version is very slow because it runs from the ROM. In startup there is
 * Another written much better assembler. There is also the seedy version without
 * Interrupt table ...
 */
void gba_irq_master_handler(void)
{
  int i = 0;

  gba_irq_disable_all();        /* We disable interruptions */

  for (i = 0 ; i < IRQ_MAX ; i++)
  {
    if(gba_irq_regs.IF & (1 << i)) /* We check what interrupt has been generated */
    {
      gba_irq_handler_table[i](); /* We called your manager */
      gba_irq_regs.IF |= (1 << i); /* We recognize your attention */
    }
  }

  gba_irq_enable_all();         /* Enable interrupts */
}

/*****************************************************************************/

/*
 * Enable interrupt management
 */
inline void gba_irq_enable_all()
{
  gba_irq_regs.IME = IRQ_ENABLE;
}

/*****************************************************************************/

/*
 * Disable interrupt management
 */
inline void gba_irq_disable_all()
{
  gba_irq_regs.IME = IRQ_DISABLE;
}

/*****************************************************************************/

/*
 * Function to enable the attention of concrete interrupts
 * @param masc Mask to select interrupts
 * @param handler Function to handle interrupts
 */
void gba_irq_enable(short masc, gba_irq_handler handler)
{
  int i;

  /* Copy the handle in the handle table */
  for (i = 0 ; i < IRQ_MAX ; i++)
  {
    if (masc & (1 << i))
      gba_irq_handler_table[i] = handler;
  }

  gba_irq_disable_all();
  gba_irq_regs.IE |= masc;
  gba_irq_enable_all();
}

/*****************************************************************************/

/*
 * Disable specific interrupts
 * @param masc Mask to select interrupts
 */
inline void gba_irq_disable(short masc)
{
  gba_irq_disable_all();
  gba_irq_regs.IE &= ~masc;
  gba_irq_enable_all();
}

/*****************************************************************************/

