.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15HandleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
  movb $\num, (interruptnumber)
  jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
  movb $\num + IRQ_BASE, (interruptnumber)
  jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
  
  # store all registers and pointers
  pusha
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs

  pushl %esp
  push (interruptnumber)
  call _ZN16InterruptManager15HandleInterruptEhj

  # restore esp to the result of the handleIntterupt method
  movl %eax, %esp

  popl %gs
  popl %fs
  popl %es
  popl %ds
  popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:
  # interrupt finished, return where you were
  iret

.data
  interruptnumber: .byte 0
  