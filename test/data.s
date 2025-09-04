    .section .text
    .global _start
    .extern __stdout_uart

_start:
    la a0, message          # a0 = pointer to message string
    la a1, __stdout_uart    # a1 = UART MMIO address

loop:
    lbu a2, 0(a0)          # load byte from message
    beqz a2, done          # if byte == 0 (null terminator), jump to done
    sb a2, 0(a1)           # store byte to UART (send character)
    addi a0, a0, 1         # increment message pointer
    j loop                 # jump back to loop

done:
    j done                 # infinite loop to end program

    .section .rodata
message:
    .ascii "Test message!!!!!!!\n"
    .byte 0                # null terminator

.section .uart, "aw"    # allocatable, writable
__stdout_uart:
    .space 4                # reserve 4 bytes (or whatever size)