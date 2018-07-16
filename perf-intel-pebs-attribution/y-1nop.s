	.file	"y.c"
	.globl	SIZE
	.data
	.align 8
	.type	SIZE, @object
	.size	SIZE, 8
SIZE:
	.quad	10000000
	.text
	.globl	f1
	.type	f1, @function
f1:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$1, -4(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L2
.L3:
	negl	-4(%rbp)
	nop
	movq	-16(%rbp), %rax
	nop
	leaq	0(,%rax,4), %rdx
	nop
	movq	-24(%rbp), %rax
	nop
	addq	%rax, %rdx
	nop
	movl	-4(%rbp), %eax
	nop
	movl	%eax, (%rdx)
	nop
	addq	$1, -16(%rbp)
	nop
.L2:
	movq	-16(%rbp), %rax
	nop
	cmpq	-32(%rbp), %rax
	nop
	jb	.L3
	nop
	popq	%rbp
	nop
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	f1, .-f1
	.globl	f2
	.type	f2, @function
f2:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$1, -8(%rbp)
	jmp	.L5
.L6:
	movq	-8(%rbp), %rax
	nop
	leaq	0(,%rax,4), %rdx
	nop
	movq	-32(%rbp), %rax
	nop
	addq	%rdx, %rax
	nop
	movq	-8(%rbp), %rdx
	nop
	leaq	0(,%rdx,4), %rcx
	nop
	movq	-24(%rbp), %rdx
	nop
	addq	%rcx, %rdx
	nop
	movl	(%rax), %eax
	nop
	movl	%eax, (%rdx)
	nop
	addq	$1, -8(%rbp)
	nop
.L5:
	movq	-40(%rbp), %rax
	nop
	subq	$1, %rax
	nop
	cmpq	%rax, -8(%rbp)
	nop
	jb	.L6
	nop
	popq	%rbp
	nop
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	f2, .-f2
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	SIZE(%rip), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -8(%rbp)
	movq	SIZE(%rip), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -16(%rbp)
	movq	SIZE(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	f1
	movq	SIZE(%rip), %rdx
	movq	-16(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	f2
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.ident	"GCC: (GNU) 7.1.0"
	.section	.note.GNU-stack,"",@progbits
