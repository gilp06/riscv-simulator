.section .text
    .align 2
    .globl _start
    .globl done
_start:
    la sp, __stack_top   # set stack pointer to RAM top
    call main
done:
    j done
