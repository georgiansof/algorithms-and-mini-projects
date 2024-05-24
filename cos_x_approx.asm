section .data
    msj db 'Introduceti x pentru a afla cos(x): ', 0
    fmt db '%lf', 0
    final_prompt db 'cos(x): %lf', 0xA, 0
    x dq 0.0
    rezultat dq 1.0
    unit dq 1.0
    two dq 2.0
    negative_unit dq -1.0
    zero dq 0.0
    eps dq 0.00001

section .text
    global main

    extern printf
    extern scanf

main:
    push rbp
    mov rbp, rsp

    xor rbx, rbx

    call get_number
    call cos_approx
    call show_number

    leave
    mov rax, 60
    mov rdi, 0
    syscall

cos_approx:
    push rbp
    mov rbp, rsp

    movsd xmm0, [unit]
    movsd xmm6, [x]
    movsd [rezultat], xmm6

    mulsd xmm6, xmm6 ; xmm6 = x^2
    movsd xmm2, [unit] ; k = 1
    movsd xmm1, xmm6 ; *= x^2
    movsd xmm3, xmm2
    mulsd xmm3, [two]
    divsd xmm1, xmm3 ; / 2!

    mulsd xmm1, [negative_unit]
    addsd xmm0, xmm1
    movsd [rezultat], xmm0
    addsd xmm2, [unit]

.loop:
    mulsd xmm1, xmm6
    movsd xmm3, xmm2 ; = k
    mulsd xmm3, [two]
    divsd xmm1, xmm3 ; /2*k
    subsd xmm3, [unit]
    divsd xmm1, xmm3 ; /(2*k - 1)
    mulsd xmm1, [negative_unit] ; 

    addsd xmm0, xmm1 ; rezultat += factor
    movsd [rezultat], xmm0

    movsd xmm5, xmm1
    
    ucomisd xmm5, [zero]
    ja .abs
        mulsd xmm5, [negative_unit]
    .abs:
    ucomisd xmm5, [eps]
    jbe .finish
    
    addsd xmm2, [unit]
    jmp .loop

.finish:
    leave
    ret

show_number:
    push rbp
    mov rbp, rsp

    mov rdi, final_prompt
    mov rsi, rezultat
    movsd xmm0, [rezultat]
    call printf

    leave
    ret

get_number:
    push rbp
    mov rbp, rsp

    xor rax, rax
    mov rdi, msj
    call printf

    mov rax, 1
    mov rdi, fmt
    mov rsi, x
    call scanf


    leave
    ret