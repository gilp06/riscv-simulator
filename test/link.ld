OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
  ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 0x10000    /* 64KB */
  RAM (rw)  : ORIGIN = 0x10000000, LENGTH = 0x10000   /* 64KB */
  UART (rw) : ORIGIN = 0x100F0000, LENGTH = 0x100     /* 256B */
}

/* Provide stack size: 4KB */
STACK_SIZE = 0x1000;

SECTIONS
{
  /* Text section in ROM */
  .text : {
    *(.text*)
  } > ROM

  /* Read-only data in ROM */
  .rodata : {
    *(.rodata*)
  } > ROM

  /* Initialized data in RAM, loaded from ROM */
  .data : {
    *(.data*)
  } > RAM AT > ROM

  /* Uninitialized data in RAM */
  .bss : {
    *(.bss*)
    *(COMMON)
  } > RAM

  /* Reserve stack space at top of RAM */
  .stack (NOLOAD) : {
    . = ALIGN(16);
    __stack_start = .;
    . += STACK_SIZE;
    __stack_end = .;
  } > RAM

  /* UART MMIO region */
  .uart 0x100F0000 (NOLOAD) : ALIGN(4)
  {
    KEEP(*(.uart))
    __stdout_uart = .;
  } > UART

  /* Discard debug and comments */
  /DISCARD/ : {
    *(.comment)
    *(.note*)
  }
}

/* Define __stack_top for startup code */
PROVIDE(__stack_top = __stack_end);