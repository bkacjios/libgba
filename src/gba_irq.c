/*

	libgba interrupt support routines

	Copyright 2003-2004 by Dave Murphy.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
	USA.

	Please report all bugs and problems through the bug tracker at
	"http://sourceforge.net/tracker/?group_id=114505&atid=668551".

*/

#include "gba_irq.h"
#include "gba_video.h"

struct gba_irq_handler_table gba_irq_table[IRQ_MAX];

void gba_irq_dummy(void) {};

void gba_irq_init() {
	int i;

	// Set all interrupts to dummy functions.
	for(i = 0; i < IRQ_MAX; i ++)
	{
		gba_irq_table[i].handler = gba_irq_dummy;
		gba_irq_table[i].mask = 0;
	}

	//IRQ_HANDLER = intr_main;
}

gba_irq_handler* gba_irq_set(short mask, gba_irq_handler function) {
	int i;

	for	(i=0;;i++) {
		if	(!gba_irq_table[i].mask || gba_irq_table[i].mask == mask) break;
	}

	if ( i >= IRQ_MAX) return NULL;

	gba_irq_table[i].handler	= function;
	gba_irq_table[i].mask		= mask;

	return &gba_irq_table[i].handler;

}

void gba_irq_enable(short mask) {
	gba_irq_regs.IME = IRQ_DISABLE;

	if (mask & IRQ_VBLANK) gba_video_regs.DISPSTAT |= VIDEO_VBLANK_INT;
	if (mask & IRQ_HBLANK) gba_video_regs.DISPSTAT |= VIDEO_HBLANK_INT;
	if (mask & IRQ_VCOUNT) gba_video_regs.DISPSTAT |= VIDEO_VCOUNTER_INT;
	gba_irq_regs.IE |= mask;
	
	gba_irq_regs.IME = IRQ_ENABLE;
}

void gba_irq_disable(short mask) {
	gba_irq_regs.IME = IRQ_DISABLE;

	if (mask & IRQ_VBLANK) gba_video_regs.DISPSTAT &= ~VIDEO_VBLANK_INT;
	if (mask & IRQ_HBLANK) gba_video_regs.DISPSTAT &= ~VIDEO_HBLANK_INT;
	if (mask & IRQ_VCOUNT) gba_video_regs.DISPSTAT &= ~VIDEO_VCOUNTER_INT;
	gba_irq_regs.IE &= ~mask;

	gba_irq_regs.IME = IRQ_ENABLE;
}