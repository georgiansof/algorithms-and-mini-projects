section .data
	;O_WRONLY equ 000001q ; write only
	dbg_msg db 'Debug'
	dbg_msg_len equ $-dbg_msg
	dbg_number dq 151.0
    rezultat dq 1.0
    exp dq 0.0
	prompt_msg db 'Write the x you want to evaluate e^x for ', 0xA
	prompt_msg_len equ $-prompt_msg
	err_msg db 'Invalid input'
	err_msg_len equ $-err_msg
	p dq 10.0
	sign_changer dq -1.0
	;file_name db 'number.txt'
	;file_name_len equ $-file_name
	;fd dq 0
	div10 dq 0.1
	qdigit dq 0.0
	unit dq 1.0
	eps dq 0.00001
	zero dq 0.0
	parte_intreaga dq 0
	parte_fractionara dq 0
	transf dq 1000000.0
	tms dq 0
	point db '.'
	linefeed db 0xA
	result_msg db 'The result is '
	result_msg_len equ $-result_msg

section .text
global _start

_start:
    push rbp
    mov rbp, rsp

	xor rbx,rbx
	mov [flags], rbx
	
	;mov rbx, __?float64?__(1.0)
	;mov [pow], rbx

	call show_prompt
	call read_number
	call exp_approx
	call show_number

	leave
	
	mov rax, 60
	mov rdi, 0
	syscall

exp_approx:
	push rbp
	mov rbp,rsp

	movsd xmm0, [rezultat]
	movsd xmm1, [unit]
	movsd xmm2, [unit]
	movsd xmm15, [eps]

	loop2:
		; rezultat += rezultat * exp / k
		mulsd xmm1, [exp]
		divsd xmm1, xmm2
		movsd xmm14, xmm1
		subsd xmm14, [eps]
		ucomisd xmm14, [zero]
		jbe exit_loop2

		addsd xmm0, xmm1
		addsd xmm2, [unit]
		jmp loop2

	exit_loop2:
	movsd [rezultat], xmm0

	leave
	ret

show_number:
	push rbp
	mov rbp,rsp

	movsd xmm0, [rezultat]
	cvttsd2si rbx, xmm0
	mov [parte_intreaga], rbx
	cvtsi2sd xmm1, rbx
	subsd xmm0, xmm1
	movsd [p], xmm0
	
	mulsd xmm0, [transf]
	xor rbx,rbx
	cvttsd2si rbx, xmm0
	mov [parte_fractionara], rbx

	mov rax, 1
	mov rdi, 1
	mov rsi, result_msg
	mov rdx, result_msg_len
	syscall

	xor rdx,rdx
	xor rax,rax
	xor r9,r9
	xor r8,r8
	mov rax, [parte_intreaga]
	mov r9, qword 10
	int_loop:
		div r9
		;mov [parte_intreaga], rax
		push rdx
		xor rdx,rdx
		inc r8
		mov [tms], r8
		cmp rax, qword 0
		je exit_int_loop
		jmp int_loop

	exit_int_loop:

	mov rcx, [tms]
	for_int_loop:
		pop rdx
		add rdx, '0'
		mov [qdigit], rdx

		mov rax, 1
		mov rdi, 1
		mov rsi, qdigit
		mov rdx, 1
		push rcx
		syscall
		pop rcx
		dec rcx
		jnz for_int_loop

	mov rax,1
	mov rdi,1
	mov rsi, point
	mov rdx, 1
	syscall

	xor rdx,rdx
	xor rax,rax
	xor r9,r9
	xor r8,r8
	mov rax, [parte_fractionara]
	mov r9, qword 10
	fract_loop:
		div r9
		;mov [parte_intreaga], rax
		push rdx
		xor rdx,rdx
		inc r8
		mov [tms], r8
		cmp rax, qword 0
		je exit_fract_loop
		jmp fract_loop

	exit_fract_loop:

	mov rcx, [tms]
	for_fract_loop:
		pop rdx
		add rdx, '0'
		mov [qdigit], rdx

		mov rax, 1
		mov rdi, 1
		mov rsi, qdigit
		mov rdx, 1
		push rcx
		syscall
		pop rcx
		dec rcx
		jnz for_fract_loop

	mov rax, 1
	mov rdi, 1
	mov rsi, linefeed
	mov rdx, 1
	syscall

	leave
	ret

show_prompt:
	push rbp
	mov rbp, rsp

	mov rax, 1 ; prompt messsage
	mov rdi, 1
	mov rsi, prompt_msg
	mov rdx, prompt_msg_len
	syscall

	leave
	ret

read_number:
	push rbp
	mov rbp, rsp

	loop1:
		mov rax, 0
		mov rdi, 0
		mov rsi, digit
		mov rdx, 1
		syscall

		cmp [digit], byte ' ' ; done
		je loop1

		cmp [digit], byte '-' ; done
		jne not_minus_sign
		; minus sign
		movzx rbx, byte [flags]
		test rbx, 0x2
		jnz error
		movzx rbx, byte [flags]
		OR rbx, qword 0x2
		mov [flags], rbx
		jmp loop1

		not_minus_sign:

		cmp [digit], byte '0'
		jl not_digit
		cmp [digit], byte '9' 
		jg not_digit
		; digit
		
		movzx rbx, byte [flags]
		test rbx, 0x1
		jnz decimal_part
		
		; integral part
		
		movsd xmm0, [exp]
		mulsd xmm0, [p]
		sub [digit], byte '0'
		
		movzx rbx, byte [digit]
		mov [qdigit], rbx
		cvtsi2sd xmm1, [qdigit]
 
		addsd xmm0, xmm1
		movsd [exp],xmm0

		jmp loop1

		decimal_part:

		movsd xmm0, [exp]
		sub [digit], byte '0'
		movzx rbx, byte [digit]
		mov [qdigit], rbx
		cvtsi2sd xmm1, [qdigit]
		;movsd xmm1, [digit]
		mulsd xmm1, [p]
		addsd xmm0, xmm1
		movsd [exp], xmm0

		movsd xmm2, [p]
		movsd xmm1, [div10]
		mulsd xmm2, xmm1
		movsd [p], xmm2
		jmp loop1

		not_digit:
		cmp [digit], byte '.' ; done
		jne not_decimal_mark
		; decimal_mark
		movzx rbx, byte [flags]
		test rbx, 0x1
		jnz error
		movzx rbx, byte [flags]
		OR rbx, 0x1
		mov [flags], rbx
		;mov rbx, __?float64?__(0.1)
		;mov [pow], rbx
		movq xmm0, [div10]
		movsd [p], xmm0
		jmp loop1

		not_decimal_mark: 
		cmp [digit], byte 0xA ; done
		je exit_loop
		; is endline
		; invalid input

		error:
		mov rax, 1
		mov rdi, 0
		mov rsi, err_msg
		mov rdx, err_msg_len
		syscall

		mov rax, 60
		mov rdi, 1
		syscall

	exit_loop:

	movzx rbx, byte [flags]
	test rbx, 0x2
	jz return

	movsd xmm0, [exp]
	movsd xmm1, [sign_changer]
	mulsd xmm0, xmm1  
	movsd [exp], xmm0 
	

	return:
	leave
	ret

;print_number_to_file:
;	push rbp
;	mov rbp, rsp
;
;	mov rax, 2
;	mov rdi, file_name
;	mov rsi, O_WRONLY
;	syscall
;	mov [fd], rax
;
;
;	mov rax, 1 
;	mov rdi, [fd]
;	mov rsi, exp
;	mov rdx, 8
;	syscall
;
;	leave
;	ret

show_debug_message:
	push rbp
	mov rbp, rsp

	mov rax, 1 
	mov rdi, 1
	mov rsi, dbg_msg
	mov rdx, dbg_msg_len
	syscall

	leave
	ret

section .bss
	digit resb 1
	flags resb 1 ; bit 0 -> decimal part activated, bit 1 -> sign switched
	;pow resq 1