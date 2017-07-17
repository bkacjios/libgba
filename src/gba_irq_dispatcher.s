/*

	libgba interrupt dispatcher routines

	Copyright 2003-2007 by Dave Murphy.

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

@---------------------------------------------------------------------------------
	.section	.iwram,"ax",%progbits
	.extern	gba_irq_table
	.code 32

	.global	gba_irq_main
@---------------------------------------------------------------------------------
gba_irq_main:
@---------------------------------------------------------------------------------
	mov	r3, #0x4000000		@ REG_BASE
	ldr	r2, [r3,#0x200]		@ Read	REG_IE

	ldr	r1, [r3, #0x208]	@ r1 = IME
	str	r3, [r3, #0x208]	@ disable IME
	mrs	r0, spsr
	stmfd	sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}

	and	r1, r2,	r2, lsr #16	@ r1 =	IE & IF

	ldrh	r2, [r3, #-8]		@\mix up with BIOS irq flags at 3007FF8h,
	orr	r2, r2, r1		@ aka mirrored at 3FFFFF8h, this is required
	strh	r2, [r3, #-8]		@/when using the (VBlank)IntrWait functions

	ldr	r2,=gba_irq_table
	add	r3,r3,#0x200

@---------------------------------------------------------------------------------
gba_find_irq:
@---------------------------------------------------------------------------------
	ldr	r0, [r2, #4]		@ Interrupt mask
	cmp	r0,#0
	beq	gba_irq_no_handler
	ands	r0, r0, r1
	bne	gba_jump_irq
	add	r2, r2, #8
	b	gba_find_irq

@---------------------------------------------------------------------------------
gba_irq_no_handler:
@---------------------------------------------------------------------------------
	strh	r1, [r3, #0x02]		@ IF Clear
	ldmfd	sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]	@ restore REG_IME
	mov	pc,lr

@---------------------------------------------------------------------------------
gba_jump_irq:
@---------------------------------------------------------------------------------
	ldr	r2, [r2]		@ user IRQ handler address
	cmp	r2, #0
	beq	gba_irq_no_handler

@---------------------------------------------------------------------------------
gba_irq_got_handler:
@---------------------------------------------------------------------------------

	mrs	r1, cpsr
	bic	r1, r1, #0xdf		@ \__
	orr	r1, r1, #0x1f		@ /  --> Enable IRQ & FIQ. Set CPU mode to System.
	msr	cpsr,r1

	strh	r0, [r3, #0x02]		@ IF Clear
	
	push	{lr}
	adr	lr, gba_irq_ret
	bx	r2

@---------------------------------------------------------------------------------
gba_irq_ret:
@---------------------------------------------------------------------------------
	pop	{lr}
	mov	r3, #0x4000000		@ REG_BASE
	str	r3, [r3, #0x208]	@ disable IME

	mrs	r3, cpsr
	bic	r3, r3, #0xdf		@ \__
	orr	r3, r3, #0x92		@ /  --> Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
	msr	cpsr, r3

	ldmfd   sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]	@ restore REG_IME
	msr	spsr, r0		@ restore spsr
	mov	pc,lr

	.pool
	.end
