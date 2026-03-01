/* * STM32F407VGT6 Linker Script - Production Grade
 * * CORE: ARM Cortex-M4
 * FLASH: 1024 KB (1 MB) @ 0x08000000
 * SRAM:  128 KB        @ 0x20000000
 * CCM:   64 KB         @ 0x10000000 (Not DMA accessible)
 */

/* 1. Entry Point */
ENTRY(Reset_Handler)

/* 2. Stack & Heap Safety Definitions */
/* * These enforce a minimum reservation. If your program eats 
 * too much RAM, the linker will throw an error instead of 
 * letting you crash at runtime.
 */
_Min_Heap_Size  = 0x200;  /* 512 bytes */
_Min_Stack_Size = 0x400;  /* 1024 bytes */

/* 3. Memory Definitions */
MEMORY
{
    /* * RX = Read/Execute (Flash)
     * RWX = Read/Write/Execute (RAM)
     */
    CCMRAM (xrw)  : ORIGIN = 0x10000000, LENGTH = 64K
    SRAM   (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH  (rx)   : ORIGIN = 0x08000000, LENGTH = 1024K
}

/* 4. Sections */
SECTIONS
{
    /* The stack starts at the end of SRAM */
    _estack = ORIGIN(SRAM) + LENGTH(SRAM);

    /* * .text: Code and Read-Only Data 
     * Stored in FLASH, Executed from FLASH
     */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Vector Table must be at 0x08000000 */
        . = ALIGN(4);
        *(.text)             /* Main Code */
        *(.text*)
        *(.glue_7)           /* Glue arm to thumb code */
        *(.glue_7t)          /* Glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;          /* End of code in Flash */
    } > FLASH

    /* * .rodata: Constants (const int x = 5;)
     */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* * .data: Initialized Variables (int x = 10;)
     * LMA (Load Memory Address) = FLASH
     * VMA (Virtual Memory Address) = SRAM
     * * The Startup Code copies this from _sidata to _sdata
     */
    _sidata = LOADADDR(.data); 

    .data : 
    {
        . = ALIGN(4);
        _sdata = .;        /* Start of data in RAM */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* End of data in RAM */
    } > SRAM AT> FLASH

    /* * .bss: Uninitialized Variables (int x;)
     * These must be zeroed out by startup code.
     */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* Start of BSS */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* End of BSS */
        __bss_end__ = _ebss;
    } > SRAM

    /* * .user_heap_stack: 
     * Checks if there is enough RAM left for Heap and Stack.
     */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } > SRAM
    
    /* Remove standard libraries info if not used to save space */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}