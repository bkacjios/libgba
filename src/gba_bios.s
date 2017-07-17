@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Embedded Operating Systems                                                  @
@                                                                             @
@ GBA BIOS Function Library                                                   @
@                                                                             @
@ The calls to the BIOS in this file are made in ARM mode                     @
@ (By moving the 16-bit function number to the left)                          @
@                                                                             @
@ Most of the time, it is preferable to use the asm                           @
@ Execute assembler embedded in C. For example:                               @
@                                                                             @
@ asm volatile("mov r0, %3       \n\t"                                        @
@              "mov r1, %4       \n\t"                                        @
@              "swi (0x06 << 16) \n\t" /* ARM mode, in thumb 0x06 mode */     @
@              "mov %0, r0       \n\t"                                        @
@              "mov %1, r1       \n\t"                                        @
@              "mov %2, r3       \n\t"                                        @
@              : "=&r" (*quot), "=&r" (*remain), "=&r" (*abs_quot)            @
@              : "r" (num), "r" (denom)                                       @
@              : "r0", "r1", "r3")/;                                          @
@                                                                             @
@ The problem is that depending on the mode (thumb, or ARM, two               @
@ implementations would be necessary, in thumb swi ny in ARM swi n<< 6, to    @
@ call the function n of the bios. To avoid this problem, and taking          @
@ advantage of the AAPCS (ARM Architecture Procedure Call Standard)           @
@ introduces the first four input arguments in r0-r3 (the rest in the stack)  @
@ and the first two output arguments in r0-r1, it is easier to implement      @
@ bios calls directly in assembler And define a .h file so that they can      @
@ be called from C.                                                           @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ At the moment we are going to implement the functions in ARM mode.          @
@ The thumb mode saves code, but with each call a mode change is not          @
@ appropriate for critical ARM (interrupt) encoded functions. In addition it  @
@ would have to be compiled with the -mthumb-interwork option                 @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.code 32
.section .text

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x00:                                                              @
@ Soft reset.                                                                 @
@  - Clears the IWRAM zone from 0x03007e00-0x03007fff                         @
@  - Initialize r0-r12 to zero                                                @
@  - Initializes the stack pointers of the USER, SVC and IRQ modes            @
@  - Switch to USER mode of execution                                         @
@  - Jump to the address 0x08000000 or 0x02000000 depending on the content    @
@    Of RETURN_ADDRESS_SEL (0x03007ffa):                                      @
@      * 0x00: Jump to 0x08000000 (default)                                   @
@      * Other: Jump to address 0x02000000                                    @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_soft_reset(void);
@
.global gba_soft_reset
gba_soft_reset:
	swi      (0x00 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x01:                                                              @
@ Register RAM reset.                                                         @
@  - Clears the I / O registers and memory zones specified in the             @
@    Mask bits flags.                                                         @
@  - The area between 0x3007E00-0x3007FFF is changed.                         @
@                                                                             @
@ Settings:                                                                   @
@  - r0: Flags indicating what is reset. Meaning of each bit:                 @
@     0 Clears 256K of EWRAM (Do not use when WRAM is returned)               @
@     1 Clean 32K of IWRAM (exclye the last 0x200 bytes)                      @
@     2 Cleans the palette memory                                             @
@     3 Clean the VRAM                                                        @
@     4 Clear the OAM (fill it with zeros -> do not hide the sprites !!)      @
@     5 Clears SIO records (switch to general-purpose mode)                   @
@     6 Cleans sound records                                                  @
@     7 Clears all other records (except SIO and sound)                       @
@                                                                             @
@ Bugs:                                                                       @
@ - SIODATA LSBs are always destroyed, even if bit 5 is not set               @
@ - Always set the screen blank because DISPCNT is initialized to 0x0080      @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_register_ram_reset(int flags);
@
.global gba_register_ram_reset
gba_register_ram_reset:
	swi      (0x01 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x02:                                                              @
@ Halt.                                                                       @
@  - Changes the CPU to the low power mode, but the other circuits            @
@    (Video, sound, series, etc.) continue to function.                       @
@  - Exits this way when an IRQ occurs (must be enabled)                      @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_halt(void);
@
.global gba_halt
gba_halt:
	swi      (0x02 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x03:                                                              @
@ Stop.                                                                       @
@                                                                             @
@ - It changes the GBA to very low consumption mode. It stops the CPU,        @
@ the clock, sound, video, WIS clock, DMAs and timers                         @
@ - You only get out of this state if a keyboard interrupt is received,       @
@ The cartridge, or the serial port                                           @
@ - Preparation before calling the stop function:                             @
@     * Disable video. If not, the image freezes,                             @
@       But battery is still running                                          @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_stop(void);
@
.global gba_stop
gba_stop:
	swi      (0x03 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x04:                                                              @
@ Interrupt wait.                                                             @
@                                                                             @
@ - It changes the GBA to very low consumption mode. It stops the CPU,        @
@ the clock, sound, video, WIS clock, DMAs and timers                         @
@ - You only get out of this state if a keyboard interrupt is received,       @
@ The cartridge, or the serial port                                           @
@ - Preparation before calling the stop function:                             @
@     * Disable video. If not, the image freezes,                             @
@       But battery is still running                                          @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_intr_wait(int clearFlag, int irq);
@
.global gba_intr_wait
gba_intr_wait:
	swi      (0x04 << 16)
	bx       lr

@
@ extern void gba_intr_wait_vblank(void);
@
.global gba_intr_wait_vblank
gba_intr_wait_vblank:
	swi      (0x05 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x06:                                                              @
@ Whole division                                                              @
@                                                                             @
@ Settings:                                                                   @
@  - r0: Numerator (32 bits with sign)                                        @
@  - r1: Denominator (32 bits with sign)                                      @
@                                                                             @
@ Return:                                                                     @
@  - r0: Whole part of the division (signed)                                  @
@  - r1: Rest of division (with sign)                                         @
@  - r3: Absolute value of the whole part of the division (signed)            @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern int gba_div(int numerator, int denominator);
@
.global gba_div
gba_div:
	swi      (0x06 << 16)
	bx       lr

@
@ extern int gba_mod(int numerator, int denominator);
@
.global gba_mod
gba_mod:
	swi      (0x06 << 16)
	mov      r0, r1
	bx       lr

@
@ extern int gba_abs_div(int numerator, int denominator);
@
.global gba_abs_div
gba_abs_div:
	swi      (0x06 << 16)
	mov      r0, r3
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x07:                                                              @
@ Whole division (ARM format).                                                @
@                                                                             @
@ Settings:                                                                   @
@  - r0: Denominator (32 bits with sign)                                      @
@  - r1: Numerator (32 bits with sign)                                        @
@                                                                             @
@ Return:                                                                     @
@  - r0: Whole part of the division (signed)                                  @
@  - r1: Rest of division (with sign)                                         @
@  - r3: Absolute value of the whole part of the division (signed)            @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern int gba_arm_div(int denominator, int numerator);
@
.global gba_arm_div
gba_arm_div:
	swi      (0x07 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0x08:                                                              @
@ Square root.                                                                @
@                                                                             @
@ Settings:                                                                   @
@  - r0: Value (32 bit unsigned)                                              @
@                                                                             @
@ Return:                                                                     @
@  - r0: Result (16 bit unsigned)                                             @
@                                                                             @
@ Notes:                                                                      @
@  - The result is whole. It is advisable to move the value to the left       @
@    2 * N bits to get the result N bits shifted to the left                  @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern u_int gba_sqrt(u_int val);
@
.global gba_sqrt
gba_sqrt:
	swi      (0x08 << 16)
	bx       lr

@
@ extern u_int gba_arc_tan(u_int val);
@
.global gba_arc_tan
gba_arc_tan:
	swi      (0x09 << 16)
	bx       lr

@
@ extern u_int gba_arc_tan2(u_int val);
@
.global gba_arc_tan2
gba_arc_tan2:
	swi      (0x0A << 16)
	bx       lr

@
@ extern u_int gba_cpu_set(const void *source,  void *dest, u32 mode);
@
.global gba_cpu_set
gba_cpu_set:
	swi      (0x0B << 16)
	bx       lr

@
@ extern u_int gba_cpu_fast_set(const void *source,  void *dest, u32 mode);
@
.global gba_cpu_fast_set
gba_cpu_fast_set:
	swi      (0x0C << 16)
	bx       lr

@
@ extern u_int gba_bios_checksum(void);
@
.global gba_bios_checksum
gba_bios_checksum:
	swi      (0x0D << 16)
	bx       lr

.global  gba_multi_boot
gba_multi_boot:
	swi      (0x25 << 16)
	bx       lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Function 0xff:                                                              @
@ Prints a string of characters in the debugger console.                      @
@                                                                             @
@ Settings:                                                                   @
@  - r0: Pointer to string                                                    @
@                                                                             @
@ Notes:                                                                      @
@  - Only works in the Visualboy Advance simulator, if it is performed in the @
@    console, let it hang                                                     @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@
@ extern void gba_vba_print(const char *str);
@
.global gba_vba_print
gba_vba_print:
	swi      (0xff << 16)
	bx       lr
