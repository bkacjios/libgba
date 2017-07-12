@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                                             @
@ Sistemas operativos empotrados                                              @
@                                                                             @
@ Segundo ejemplo: El "hola mundo" en la GBA en C                             @
@                                                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Constantes                                                                  @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Modos de ejecución y bits de estado                                         @
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

@ Relleno para las pilas (para detectar desbordamientos)
.set RELLENO_PILA,     0xcadacafe

@ Flags de interrupción
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
@ Se usará el repertorio de instrucciones de 32 bits                          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ Creamos una nueva sección para indicarle al script del linker
@ que la coloque al principio de la ROM
.section .boot

@ El código de arranque debe ser de 32 bits
.code 32

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Punto de entrada                                                            @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.globl _ENTRY_
_ENTRY_:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Incluimos la cabecera del cartucho                                          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ La primera instrucción es un salto que se salta la cabecera
	b	start_rom

	@ Nintendo Logo (156 bytes)
	.fill   156, 1, 0

	@ Game Title (12 caracteres en mayúsculas)
	.byte   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte   0x00, 0x00, 0x00, 0x00

	@ Game Code (4 caracteres en mayúsculas)
	.byte   0x00, 0x00, 0x00, 0x00

	@ Maker code (2 caracteres en mayúsculas, ej. "01" -> Nintendo)
	.byte   0x30, 0x31

	@ Fixed value (debe se 0x96)
	.byte   0x96

	@ Main Unit Code (Identifica el HW requerido. Debería ser 0x00)
	.byte   0x00

	@ Device Type (Tipo de cartucho)
	.byte   0x00

	@ Reserved Area (7 Bytes a 0)
	.byte   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	@ Software Version Number (Número de versión del juego)
	.byte   0x00

	@ Complement Check (Checksum de la cabecera)
	@ Se debe calcular como:
	@ chk=0:for i=0A0h to 0BCh:chk=chk-[i]:next:chk=(chk-19h) and 0FFh
	.byte   0xf0

	@ Reserved Area (2 Bytes a 0)
	.byte   0x00, 0x00

	.align

start_rom:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copiamos el código de procesamiento de interrupciones a la IWRAM.           @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__inicio_intr__
	ldr     r2, =__fin_intr__
	ldr     r3, =__inicio_intr_rom__
	bl      copiar

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Inicializamos las pilas                                                     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__fondo_pila_user__
	ldr     r2, =__tope_pila_svc__
	ldr     r3, =RELLENO_PILA
	bl      inicializar

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Inicializamos los punteros de pila para cada modo                           @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ Cambiamos al modo IRQ
	@ Deshabilitamos las interrupciones IRQ y FIQ en este modo
	msr     CPSR_c, #(ARM_MODE_IRQ | ARM_I_BIT | ARM_F_BIT)

	@ Inicializamos el puntero de pila
	ldr     sp, =__tope_pila_irq__


	@ Cambiamos al modo supervisor
	@ Deshabilitamos las interrupciones IRQ y FIQ en este modo
	msr     CPSR_c, #(ARM_MODE_SVC | ARM_I_BIT | ARM_F_BIT)

	@ Inicializamos el puntero de pila
	ldr     sp, =__tope_pila_svc__


	@ Cambiamos al modo usuario (el que se quedará partir de ahora)
	@ Deshabilitamos las interrupciones FIQ (no se usan en la GBA)
	msr     CPSR_c, #(ARM_MODE_USER | ARM_F_BIT)

	@ Inicializamos el puntero de pila
	ldr     sp, =__tope_pila_user__

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Preparamos el soporte de las interrupciones                                 @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ Cuando se produzca uan interrupción, la GBA salta a la dirección
	@ que esté almacenada en IRQ_HANDLER (0x03007ffc), por tanto,
	@ almacenanos en esta dirección la rutina de atención a las
	@ interrupciones (definida más abajo)
	ldr     r1, =gba_intr_handler_address
	ldr     r0, =gba_irq_main
	str     r0, [r1]

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copiamos las variables inicializadas de su dirección LMA a la VMA.          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__inicio_datos__
	ldr     r2, =__fin_datos__
	ldr     r3, =__inicio_datos_rom__
	bl      copiar

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Fijamos a 0 la memoria RAM reservada para las variables sin inicializar.    @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r1, =__inicio_bss__
	ldr     r2, =__fin_bss__
	ldr     r3, =0
	bl      inicializar

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Empieza el código en C                                                      @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ldr     r0, =main         @ Llamamos a la funcion main
	bx      r0                @ Llamamos a la funcion main
	b       .                 @ Colgamos el sistema si main retorna

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Funciones auxiliares                                                        @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	@ Las colocamos en la sección de código
	.section .text

	@ De momento usamos código de 32 bits
	.code 32

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Rutina para copiar código o datos de la memoria ROM a la RAM                @
@ Parámetros:                                                                 @
@   r1: Puntero a la posición inicial en la memoria RAM                       @
@   r2: Puntero a la posición final en la memoria RAM                         @
@   r3: Puntero a la posición inicial en la memoria ROM                       @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	copiar:
		cmp     r1, r2
		bge     copia_hecha
		ldrb    r4, [r3], #+1
		strb    r4, [r1], #+1
		b       copiar

	copia_hecha:
		mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Rutina para inicializar una zona de memoria RAM a un valor fijo             @
@ Parámetros:                                                                 @
@   r1: Puntero a la posición inicial en la memoria RAM                       @
@   r2: Puntero a la posición final en la memoria RAM                         @
@   r3: Valor para la inicialización                                          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	inicializar:
		cmp     r1, r2
		strne   r3, [r1], #+4
		bne     inicializar

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
	ldr	r2, [r3,#0x200]		@ Read	REG_IE

	ldr	r1, [r3, #0x208]	@ r1 = IME
	str	r3, [r3, #0x208]	@ disable IME
	mrs	r0, spsr
	stmfd	sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}

	and	r1, r2,	r2, lsr #16	@ r1 =	IE & IF

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
	ldmfd	sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]	@ restore REG_IME
	mov	pc,lr

jump_intr:
	ldr	r2, [r2]		@ user IRQ handler address
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
	str	r3, [r3, #0x208]	@ disable IME

	mrs	r3, cpsr
	bic	r3, r3, #0xdf		@ \__
	orr	r3, r3, #0x92		@ /  --> Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
	msr	cpsr, r3

	ldmfd   sp!, {r0-r1,r3,lr}	@ {spsr, IME, REG_BASE, lr_irq}
	str	r1, [r3, #0x208]		@ restore REG_IME
	msr	spsr, r0				@ restore spsr
	mov	pc,lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ End                                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
.end
