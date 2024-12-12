	.file	"counter-nosync.c"
	.text
	.globl	counter
	.bss
	.align 8
	.type	counter, @object
	.size	counter, 8
counter:
	.zero	8
	.section	.rodata
	.align 8
.LC0:
	.string	"set_cpu: pthread_setaffinity failed for cpu %d\n"
.LC1:
	.string	"set_cpu: set cpu %d\n"
	.text
	.globl	set_cpu
	.type	set_cpu, @function
set_cpu:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$192, %rsp
	movl	%edi, -180(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	call	pthread_self@PLT
	movq	%rax, -160(%rbp)
	leaq	-144(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movl	-180(%rbp), %eax
	cltq
	movq	%rax, -152(%rbp)
	cmpq	$1023, -152(%rbp)
	ja	.L3
	movq	-152(%rbp), %rax
	shrq	$6, %rax
	leaq	0(,%rax,8), %rdx
	leaq	-144(%rbp), %rcx
	addq	%rcx, %rdx
	movq	(%rdx), %rdx
	movq	-152(%rbp), %rcx
	andl	$63, %ecx
	movl	$1, %esi
	salq	%cl, %rsi
	movq	%rsi, %rcx
	leaq	0(,%rax,8), %rsi
	leaq	-144(%rbp), %rax
	addq	%rsi, %rax
	orq	%rcx, %rdx
	movq	%rdx, (%rax)
.L3:
	leaq	-144(%rbp), %rdx
	movq	-160(%rbp), %rax
	movl	$128, %esi
	movq	%rax, %rdi
	call	pthread_setaffinity_np@PLT
	movl	%eax, -164(%rbp)
	cmpl	$0, -164(%rbp)
	je	.L4
	movl	-180(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L1
.L4:
	movl	-180(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L1:
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	set_cpu, .-set_cpu
	.globl	incr_var
	.type	incr_var, @function
incr_var:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L8
.L9:
	movq	counter(%rip), %rax
	addq	$1, %rax
	movq	%rax, counter(%rip)
	addl	$1, -4(%rbp)
.L8:
	cmpl	$99999999, -4(%rbp)
	jle	.L9
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	incr_var, .-incr_var
	.section	.rodata
	.align 8
.LC2:
	.string	"mythread1: args: '%s'. pid %d; ppid %d; tid %d\n"
	.text
	.globl	mythread1
	.type	mythread1, @function
mythread1:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -24(%rbp)
	call	gettid@PLT
	movl	%eax, %r12d
	call	getppid@PLT
	movl	%eax, %ebx
	call	getpid@PLT
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%r12d, %r8d
	movl	%ebx, %ecx
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	set_cpu
	movl	$0, %eax
	call	incr_var
	movl	$0, %eax
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	mythread1, .-mythread1
	.section	.rodata
	.align 8
.LC3:
	.string	"mythread2: args: '%s'. pid %d; ppid %d; tid %d\n"
	.text
	.globl	mythread2
	.type	mythread2, @function
mythread2:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -24(%rbp)
	call	gettid@PLT
	movl	%eax, %r12d
	call	getppid@PLT
	movl	%eax, %ebx
	call	getpid@PLT
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%r12d, %r8d
	movl	%ebx, %ecx
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	set_cpu
	movl	$0, -28(%rbp)
	jmp	.L13
.L14:
	movq	counter(%rip), %rax
	addq	$1, %rax
	movq	%rax, counter(%rip)
	addl	$1, -28(%rbp)
.L13:
	cmpl	$99999999, -28(%rbp)
	jle	.L14
	movl	$0, %eax
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	mythread2, .-mythread2
	.section	.rodata
.LC4:
	.string	"thread 1"
	.align 8
.LC5:
	.string	"main: 1 pthread_create() failed: %s\n"
.LC6:
	.string	"thread 2"
	.align 8
.LC7:
	.string	"main: pthread_join() failed: %s\n"
.LC8:
	.string	"main: counter = %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-32(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movl	$16, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	leaq	-32(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rcx
	leaq	mythread1(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	je	.L17
	movl	-52(%rbp), %eax
	movl	%eax, %edi
	call	strerror@PLT
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L23
.L17:
	leaq	-32(%rbp), %rax
	leaq	.LC6(%rip), %rdx
	movl	$16, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	leaq	-32(%rbp), %rdx
	leaq	-40(%rbp), %rax
	movq	%rdx, %rcx
	leaq	mythread2(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	je	.L19
	movl	-52(%rbp), %eax
	movl	%eax, %edi
	call	strerror@PLT
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L23
.L19:
	movq	-48(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	je	.L20
	movl	-52(%rbp), %eax
	movl	%eax, %edi
	call	strerror@PLT
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L23
.L20:
	movq	-40(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	je	.L21
	movl	-52(%rbp), %eax
	movl	%eax, %edi
	call	strerror@PLT
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L23
.L21:
	movq	counter(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	counter(%rip), %rax
	cmpq	$200000000, %rax
	je	.L22
	movl	$1, %eax
	jmp	.L23
.L22:
	movl	$0, %eax
.L23:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L24
	call	__stack_chk_fail@PLT
.L24:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
