@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Embedded Operating Systems                                                  @
@                                                                             @
@ Second example: The "Hello World" in the GBA in C                           @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Constants                                                                   @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Execution Modes and Status Bits                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.set ARM_MODE_USER,            0x10
.set ARM_MODE_FIQ,             0x11
.set ARM_MODE_IRQ,             0x12
.set ARM_MODE_SVC,             0x13
.set ARM_MODE_ABORT,           0x17
.set ARM_MODE_UNDEF,           0x1b
.set ARM_MODE_SYS,             0x1f

.set ARM_I_BIT,                0x80
.set ARM_F_BIT,                0x40
.set ARM_T_BIT,                0x20

@ heap filler (to detect overflows)
.set HEAP_FILLER,     0xcadacafe

@ Interrupt Flags
.set IRQ_VBLANK,       (1 << 0)
.set IRQ_HBLANK,       (1 << 1)
.set IRQ_VCOUNT,       (1 << 2)
.set IRQ_TIMER0,       (1 << 3)
.set IRQ_TIMER1,       (1 << 4)
.set IRQ_TIMER2,       (1 << 5)
.set IRQ_TIMER3,       (1 << 6)
.set IRQ_SERIAL,       (1 << 7)
.set IRQ_DMA0,         (1 << 8)
.set IRQ_DMA1,         (1 << 9)
.set IRQ_DMA2,         (1 << 10)
.set IRQ_DMA3,         (1 << 11)
.set IRQ_KEYPAD,       (1 << 12)
.set IRQ_CART,         (1 << 13)


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ The 32-bit instruction repertoire                                           @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ We created a new section to tell the linker script
@ Put it at the beginning of the ROM
.section .boot

@ The boot code must be 32-bit
.code 32

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Entry point                                                                 @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.globl _ENTRY_
_ENTRY_:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ We include the cartridge header                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ The first instruction is a jump that jumps the header
	b	start_rom

	@ Nintendo Logo (156 bytes)
	.fill   156, 1, 0

	@ Game Title (12 characters in capital letters)
	.byte   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte   0x00, 0x00, 0x00, 0x00

	@ Game Code (4 characters in capital letters)
	.byte   0x00, 0x00, 0x00, 0x00

	@ Maker code (2 characters in capital letters, eg "01" -> Nintendo)
	.byte   0x30, 0x31

	@ Fixed value (should be 0x96)
	.byte   0x96

	@ Main Unit Code (Identifies the required HW. Should be 0x00)
	.byte   0x00

	@ Device Type
	.byte   0x00

	@ Reserved Area (7 Bytes to 0)
	.byte   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	@ Software Version Number
	.byte   0x00

	@ CRC Check (Checksum of the header)
	@ It should be calculated as:
	@ Chk = 0: for i = 0A0h to 0BCh: chk = chk- [i]: next: chk = (chk-19h) and 0FFh
	.byte   0xf0

	@ Reserved Area (2 Bytes to 0)
	.byte   0x00, 0x00

	.align

start_rom:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ We copied the interrupt processing code to the IWRAM.                       @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__start_irq__
	ldr     r2, =__end_irq__
	ldr     r3, =__start_irq_rom__
	bl      copy

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Initialize the heap                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__bot_heap_user__
	ldr     r2, =__top_heap_svc__
	ldr     r3, =HEAP_FILLER
	bl      initialize

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Initialize the stack pointers for each mode                                 @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ Switch to IRQ mode
	@ We disable IRQ and FIQ interrupts in this mode
	msr     CPSR_c, #(ARM_MODE_IRQ | ARM_I_BIT | ARM_F_BIT)

	@ Initialize the stack pointer
	ldr     sp, =__top_heap_irq__

	@ Switch to supervisor mode
	@ We disable IRQ and FIQ interrupts in this mode
	msr     CPSR_c, #(ARM_MODE_SVC | ARM_I_BIT | ARM_F_BIT)

	@ Initialize the stack pointer
	ldr     sp, =__top_heap_svc__

	@ We change to user mode (which will stay from now)
	@ We disable FIQ interrupts (not used in GBA)
	msr     CPSR_c, #(ARM_MODE_USER | ARM_F_BIT)

	@ Initialize the stack pointer
	ldr     sp, =__top_heap_user__

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ We prepare the interrupt support                                            @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ When an interruption occurs, the GBA jumps to the address
	@ that is stored in IRQ_HANDLER (0x03007ffc), so store the
	@ interrupt attention routine (defined below)
	ldr     r1, =gba_irq_handler_address
	ldr     r0, =gba_irq_main
	str     r0, [r1]

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ We copy the initialized variables from their LMA address to the VMA.        @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__start_data__
	ldr     r2, =__end_data__
	ldr     r3, =__start_data_rom__
	bl      copy

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Set the RAM reserved for uninitialized variables to 0.                      @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__start_bss__
	ldr     r2, =__end_bss__
	ldr     r3, =0
	bl      initialize

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Start the code in C                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r0, =main         @ We call the main function
	bx      r0                @ We call the main function
	b       .                 @ We hang the system if main returns

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Auxiliary Functions                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ We put them in the code section
	.section .text

	@ Currently we use 32-bit code
	.code 32

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Routine to copy code or data from ROM to RAM                                @
@ Settings:                                                                   @
@   r1: Pointer to initial position in RAM                                    @
@   r2: Pointer to the final position in RAM                                  @
@   r3: Pointer to initial position in ROM                                    @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	copy:
		cmp     r1, r2
		bge     do_copy
		ldrb    r4, [r3], #+1
		strb    r4, [r1], #+1
		b       copy

	do_copy:
		mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Routine to initialize a RAM zone to a fixed value                           @
@ Settings:                                                                   @
@   r1: Pointer to initial position in RAM                                    @
@   r2: Pointer to the final position in RAM                                  @
@   r3: Value for initialization                                              @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	initialize:
		cmp     r1, r2
		strne   r3, [r1], #+4
		bne     initialize

		mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Interrupt Processing                                                        @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ We create a new section (.intr), which we will locate in the
@ linker script in the iwram memory so that its processing is fast
	.section	.iwram,"ax",%progbits
	.extern	gba_irq_table

@ The code must be 32-bit
	.code 32

gba_irq_main:
	mov	r3, #0x4000000		@ REG_BASE
	ldr	r2, [r3,#0x200]		@ Read REG_IE

	ldr	r1, [r3, #0x208]	@ R1 = IME
	str	r3, [r3, #0x208]	@ Disable IME
	mrs	r0, spsr
	stmfd	sp!, {r0-r1,r3,lr}	@ {Spsr, IME, REG_BASE, lr_irq}

	and	r1, r2,	r2, lsr #16	@ R1 = IE & IF

	ldrh	r2, [r3, #-8]		@\mix up with BIOS irq flags at 3007FF8h,
	orr	r2, r2, r1				@ aka mirrored at 3FFFFF8h, this is required
	strh	r2, [r3, #-8]		@/when using the (VBlank)IntrWait functions

	ldr	r2,=gba_irq_table
	add	r3,r3,#0x200

gba_find_irq:
	ldr	r0, [r2, #4]		@ Interrupt mask
	cmp	r0,#0
	beq	gba_irq_no_handler
	ands	r0, r0, r1
	bne	jump_intr
	add	r2, r2, #8
	b	gba_find_irq

gba_irq_no_handler:
	strh	r1, [r3, #0x02]		@ IF Clear
	ldmfd	sp!, {r0-r1,r3,lr}	@ {Spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]	@ Restore REG_IME
	mov	pc,lr

jump_intr:
	ldr	r2, [r2]		@ User IRQ handler address
	cmp	r2, #0
	beq	gba_irq_no_handler

gba_irq_got_handler:
	mrs	r1, cpsr
	bic	r1, r1, #0xdf		@ \__
	orr	r1, r1, #0x1f		@ /  --> Enable IRQ & FIQ. Set CPU mode to System.
	msr	cpsr,r1

	strh	r0, [r3, #0x02]		@ IF Clear
	
	push	{lr}
	adr	lr, gba_irq_ret
	bx	r2

gba_irq_ret:
	pop	{lr}
	mov	r3, #0x4000000		@ REG_BASE
	str	r3, [r3, #0x208]	@ Disable IME

	mrs	r3, cpsr
	bic	r3, r3, #0xdf		@ \__
	orr	r3, r3, #0x92		@ /  --> Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
	msr	cpsr, r3

	ldmfd   sp!, {r0-r1,r3,lr}	@ {Spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]		@ Restore REG_IME
	msr	spsr, r0				@ Restore spsr
	mov	pc,lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ End @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
.end
