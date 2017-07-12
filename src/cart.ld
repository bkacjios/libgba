/*****************************************************************************/
/*                                                                           */
/* Sistemas operativos empotrados                                            */
/*                                                                           */
/* Linker script para la consola Gameboy Advance de Nintendo                 */
/*                                                                           */
/* Este script se ha escrito para programas que serán cargados con un        */
/* cartucho con memoria flash.                                               */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Punto de entrada                                                          */
/*****************************************************************************/

ENTRY(_ENTRY_)

SEARCH_DIR(.)

/*
 * Console memory map
 */
MEMORY
{
  bios    : org = 0x00000000, len = 0x00004000 /* BIOS - ROM         (16 KB) */
  ewram   : org = 0x02000000, len = 0x00040000 /* External Work RAM (256 KB) */

  iwram1  : org = 0x03000000, len = 0x00007f00 /* Internal Work RAM  (32 KB) */
  iwram2  : org = 0x03007f00, len = 0x00000100

  ioregs  : org = 0x04000000, len = 0x00000400 /* I/O Registers       (1 KB) */
  palette : org = 0x05000000, len = 0x00000400 /* Palette             (1 KB) */
  vram    : org = 0x06000000, len = 0x00180000 /* Video RAM          (96 KB) */
  oam     : org = 0x07000000, len = 0x00000400 /* Object Attributes   (1 KB) */
  rom     : org = 0x08000000, len = 0x02000000 /* Cartridge ROM      (32 MB) */
}

SECTIONS
{
  /*
   * I/O Registry
   */
  .ioregs : 
  {
    gba_ioregs        = .;
    gba_video_regs    = gba_ioregs;
    gba_sound_regs    = gba_ioregs + 0x0060;
    gba_dma_regs      = gba_ioregs + 0x00b0;
    gba_timer_regs    = gba_ioregs + 0x0100;
    gba_serial_regs   = gba_ioregs + 0x0120;
    gba_keypad_regs   = gba_ioregs + 0x0130;
    gba_irq_regs      = gba_ioregs + 0x0200;
    gba_gamepack_regs = gba_ioregs + 0x0204;
  } > ioregs

  /*
   * Paleta.
   */
  .palette :
  {
    gba_palette = .;
  } > palette
  
  /*
   * VRAM.
   */
  .vram :
  {
    gba_vram = .;
  } > vram
  
  /*
   * OAM.
   */
  .oam :
  {
    gba_oam = .;
  } > oam

  /*
   * El código se ejecutará desde la memoria rom del cartucho.
   */
  .text : 
  {
    . = ALIGN(4);
    *(.boot);          /* El código de arranque debe estar al principio */
    . = ALIGN(4);
    *(.text);          /* Código del programa */
    . = ALIGN(4);
    *(.rodata*);       /* Constantes en memoria (solo lectura) */
    . = ALIGN(4);
    *(.glue_7*);       /* Para las llamadas a código thumb */
    . = ALIGN(4);
  } > rom

  /*
   * El código para el procesamiento de interrupciones se copia a partir de una
   * dirección de carga LMA justo después del código (en la ROM), aunque se le
   * asignan direcciones virtuales en la memoria IWRAM, que es más rápida, para
   * que el procesamiento de interrupciones sea más eficiente. El programa de
   * arranque deberá copiarlos de la ROM a la IWRAM.
   */
  .intr :
  {
    . = ALIGN(4);
    __start_irq__ = . ;
    *(.intr);          /* Procesamiento de interrupciones */
    . = ALIGN(4);
    __end_irq__ = . ;
  } > iwram1 AT > rom

  __start_irq_rom__ = LOADADDR(.intr);

  /*
   * Los datos se copian en la ROM en las direcciones LMA siguientes, y también
   * se les asignan direcciones virtuales en la memoria IWRAM, ya que deben
   * poder modificarse durante la ejecución del programa. El programa de
   * arranque deberá copiarlos de la ROM a la IWRAM.
   */
  .data :
  {
    . = ALIGN(4);
    __start_data__ = . ;
    *(.data);          /* Datos inicializados */
    . = ALIGN(4);
    __end_data__ = . ;
  } > iwram1 AT > rom

  __start_data_rom__ = LOADADDR(.data);

  /*
   * Para las variables sin inicializar y la pila se reserva espacio en la
   * memoria iwram.
   */
  .bss :
  {
    . = ALIGN(4);
    __start_bss__ = . ;
    *(.bss);           /* Variables inicializadas a cero */
    *(COMMON);         /* Variables sin inicializar */
    . = ALIGN(4);
    __end_bss__ = . ;
  } > iwram1
  
  /*
   * Sección de la iwram gestionada por la consola
   */
  .system :
  {
    __bot_heap_irq__ = .;        /* Pila en el modo IRQ */
    . += 0x00a0;                   /* 160 bytes */
    __top_heap_irq__ = .;
    
    __bot_heap_svc__ = .;        /* Pila en el modo supervisor */
    . += 0x0040;                   /* 64 bytes */
    __top_heap_svc__ = .;
    
    . += 0x0010;                   /* Allocated area */
    
    gba_sound_buffer_address = .;  /* Dirección del buffer de sonido */
    . += 0x0004;                   /* 4 bytes */
    
    . += 0x0004;                   /* Allocated area */
    
    gba_irq_check_flag = .;       /* Interrupt check flag */
    . += 0x0002;                   /* 2 bytes */
    
    gba_return_address_sel_ = .;   /* Return address selector */
    . += 0x0001;                   /* 1 byte */
    
    . += 0x0001;                   /* Allocated area */
 
    gba_irq_handler_address = .;  /* Dirección del manejador de interrupciones */
    . += 0x0004;                   /* 4 bytes */
  } > iwram2

  /* Reservamos 1 KB para la pila del modo user */
  __top_heap_user__ = __bot_heap_irq__;
  __bot_heap_user__ = __top_heap_user__ - 0x400;
  
  end = . ;
}
