.set IRQ_BASE, 0x20

.section .text

.extern _ZN5citos8hardware16InterruptManager15HandleInterruptEhj
.global _ZN5citos8hardware16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN5citos8hardware16InterruptManager19HandleException\num\()Ev
_ZN5citos8hardware16InterruptManager19HandleException\num\()Ev:
  movb $\num, (interruptnumber)
  jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN5citos8hardware16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN5citos8hardware16InterruptManager26HandleInterruptRequest\num\()Ev:
  movb $\num + IRQ_BASE, (interruptnumber)
  pushl $0
  jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:
  
  # store all registers and pointers
  #pusha
  #pushl %ds
  #pushl %es
  #pushl %fs
  #pushl %gs

  pushl %ebp
  pushl %edi
  pushl %esi

  pushl %edx
  pushl %ecx
  pushl %ebx
  pushl %eax

  pushl %esp
  push (interruptnumber)
  call _ZN5citos8hardware16InterruptManager15HandleInterruptEhj

  # restore esp to the result of the handleIntterupt method
  movl %eax, %esp

  popl %eax
  popl %ebx
  popl %ecx
  popl %edx

  popl %esi
  popl %edi
  popl %ebp
  #pop %gs
  #pop %fs
  #pop %es
  #pop %ds
  #popa

  add $4, %esp

_ZN5citos8hardware16InterruptManager22IgnoreInterruptRequestEv:
  # interrupt finished, return where you were
  iret

.data
  interruptnumber: .byte 0
  