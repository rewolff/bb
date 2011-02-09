	.file	"bb_mon.c"
.globl interval
	.data
	.align 4
	.type	interval, @object
	.size	interval, 4
interval:
	.long	100000
	.section	.rodata
.LC0:
	.string	"usage error.\n"
	.text
.globl usage
	.type	usage, @function
usage:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$13, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC0, (%esp)
	call	fwrite
	movl	$1, (%esp)
	call	exit
	.size	usage, .-usage
	.section	.rodata
.LC1:
	.string	"hello2"
.LC2:
	.string	"hello3"
.LC3:
	.string	"%d\n"
.LC4:
	.string	"hello4"
	.text
.globl mon_byte
	.type	mon_byte, @function
mon_byte:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$-1, -4(%ebp)
	movl	$.LC1, (%esp)
	call	puts
.L5:
	movl	$.LC2, (%esp)
	call	puts
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	cmpl	-4(%ebp), %eax
	je	.L4
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	movl	%eax, 4(%esp)
	movl	$.LC3, (%esp)
	call	printf
	movl	$.LC4, (%esp)
	call	puts
	leave
	ret
.L4:
	movl	interval, %eax
	movl	%eax, (%esp)
	call	usleep
	jmp	.L5
	.size	mon_byte, .-mon_byte
.globl mon_short
	.type	mon_short, @function
mon_short:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$-1, -4(%ebp)
.L9:
	movl	8(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-4(%ebp), %eax
	je	.L8
	movl	8(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	movl	%eax, 4(%esp)
	movl	$.LC3, (%esp)
	call	printf
	leave
	ret
.L8:
	movl	interval, %eax
	movl	%eax, (%esp)
	call	usleep
	jmp	.L9
	.size	mon_short, .-mon_short
.globl mon_int
	.type	mon_int, @function
mon_int:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$-1, -4(%ebp)
.L13:
	movl	-4(%ebp), %eax
	movl	8(%ebp), %edx
	movl	(%edx), %edx
	cmpl	%edx, %eax
	je	.L12
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	$.LC3, (%esp)
	call	printf
	leave
	ret
.L12:
	movl	interval, %eax
	movl	%eax, (%esp)
	call	usleep
	jmp	.L13
	.size	mon_int, .-mon_int
	.section	.rodata
.LC6:
	.string	"%f\n"
	.text
.globl mon_float
	.type	mon_float, @function
mon_float:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	$0xbf800000, %eax
	movl	%eax, -4(%ebp)
.L18:
	movl	8(%ebp), %eax
	flds	(%eax)
	flds	-4(%ebp)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	.L22
	je	.L21
.L22:
.L20:
	movl	8(%ebp), %eax
	flds	(%eax)
	fstpl	4(%esp)
	movl	$.LC6, (%esp)
	call	printf
	leave
	ret
.L21:
	movl	interval, %eax
	movl	%eax, (%esp)
	call	usleep
	jmp	.L18
	.size	mon_float, .-mon_float
	.section	.rodata
.LC8:
	.string	"hello"
.LC9:
	.string	"hello5"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$32, %esp
	movl	%ecx, %ebx
	call	bb_init
	movl	4(%ebx), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	jne	.L24
	call	usage
.L24:
	movl	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	bb_get_type
	movl	%eax, -12(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	bb_get_ptr
	movl	%eax, -16(%ebp)
	movl	$.LC8, (%esp)
	call	puts
	movl	-12(%ebp), %eax
	movl	%eax, -28(%ebp)
	cmpl	$1, -28(%ebp)
	je	.L27
	cmpl	$1, -28(%ebp)
	jg	.L30
	cmpl	$0, -28(%ebp)
	je	.L26
	jmp	.L25
.L30:
	cmpl	$2, -28(%ebp)
	je	.L28
	cmpl	$3, -28(%ebp)
	je	.L29
	jmp	.L25
.L26:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	mon_byte
	jmp	.L25
.L27:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	mon_short
	jmp	.L25
.L28:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	mon_int
	jmp	.L25
.L29:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	mon_float
.L25:
	movl	$.LC9, (%esp)
	call	puts
	movl	$0, (%esp)
	call	exit
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
