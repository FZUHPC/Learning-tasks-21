	.file	"conotext.c"
	.text
	.globl	context_switch
	.type	context_switch, @function
context_switch:
.LFB0:
	.cfi_startproc
	.cfi_def_cfa_offset 16
	pushq  	%rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movq 	%rsp, %rbp
	subq	$120, %rsp
	movq	%rsp, (%rdi)
	movq	%rax, -8(%rsp)
	movq	%rbx, -16(%rsp)
	movq	%rcx, -24(%rsp)
	movq	%rdx, -32(%rsp)
	movq	%rdi, -40(%rsp)
	movq	%rsi, -48(%rsp)
	movq	%rbp, -56(%rsp)
	movq	%r8 , -64(%rsp)
	movq	%r9 , -72(%rsp)
	movq	%r10, -80(%rsp)
	movq	%r11, -88(%rsp)
	movq	%r12, -96(%rsp)
	movq	%r13, -104(%rsp)
	movq	%r14, -112(%rsp)
	movq	%r15, -120(%rsp)

	movq 	(%rsi), %rsp
	movq	-8(%rsp), %rax
	movq	-16(%rsp), %rbx
	movq	-24(%rsp), %rcx
	movq	-32(%rsp), %rdx
	movq	-40(%rsp), %rdi
	movq	-48(%rsp), %rsi
	movq	-56(%rsp), %rbp
	movq	-64(%rsp), %r8 
	movq	-72(%rsp), %r9 
	movq	-80(%rsp), %r10
	movq	-88(%rsp), %r11
	movq	-96(%rsp), %r12
	movq	-104(%rsp), %r13
	movq	-112(%rsp), %r14
	movq	-120(%rsp), %r15
	
	leave
	nop
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	context_switch, .-context_switch
	.globl	context_init
	.type	context_init, @function
context_init:
.LFB1:
	.cfi_startproc
	.cfi_def_cfa_offset 16
	pushq 	%rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movq 	%rsp, %rbp
	movq    (%rdi), %rsp
	subq	$120,  %rsp
	movq	$0, -8(%rsp)	
	movq	$0, -16(%rsp)	
	movq	$0, -24(%rsp)	
	movq	$0, -32(%rsp)	
	movq	$0, -40(%rsp)	
	movq	$0, -48(%rsp)	
	
	pushq	(%rdi)
	popq	-56(%rsp)	
	
	movq	$0, -64(%rsp)
	movq	$0, -72(%rsp)
	movq	$0, -80(%rsp)
	movq	$0, -88(%rsp)
	movq	$0, -96(%rsp)
	movq	$0, -104(%rsp)
	movq	$0, -112(%rsp)
	movq	$0, -120(%rsp)
	movq 	%rsp, (%rdi)

	leave
	nop
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	context_init, .-context_init
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
	