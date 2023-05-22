extern printf
extern scanf

section .data
    rezultat dq 0
    pstr db 'bla', 0
section .bss

section .text
global _start

_start:
    push rbp
    mov rbp, rsp
    push pstr
    call printf

    mov rsp, rbp
    pop rbp
    ret