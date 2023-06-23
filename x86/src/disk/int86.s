int86:
	push ebp
	mov ebp, esp
	pushad
	cli
	sidt (saved_idtr)
	lidt (rmidt)
	mov ebx, int_op
	movb 8(%ebp), %al
	movb %al, 1(%ebx)

	ljmp $0x30,$0f
0:
	.code16
	# disable protection
	mov %cr0, %eax
	and $0xfffe, %ax
	mov %eax, %cr0
	# load cs <- 0
	ljmp $0,$0f
0:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	nop
	mov saved_esp, esp
	mov saved_ebp, ebp
	mov 12(%ebp), %esp
	popad
	popfw
	pop es
	pop ds
	mov esp, saved_esp
	mov esp, 0x7be0

	# call 16bit interrupt
int_op:	int $0
	# BIOS call might have enabled interrupts, cli for good measure
	cli

	# save all registers that we'll clobber before having the
	# chance to populate the int86regs structure
	mov %eax, saved_eax
	mov %ds, saved_ds
	mov %es, saved_es
	pushfw
	pop %ax
	mov %ax, saved_flags

	# re-enable protection
	mov %cr0, %eax
	or $1, %ax
	mov %eax, %cr0
	# long jump to load code selector for 32bit code (1)
	ljmp $0x8,$0f
0:
	.code32
	# set data selector (2) to all segment regs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	nop

	# point the esp to our regs struct, to fill it with pusha/pushf
	mov saved_ebp, %ebp
	mov 12(%ebp), %esp
	add $38, %esp
	mov saved_ds, %ax
	pushw %ax
	mov saved_es, %ax
	pushw %ax
	mov saved_flags, %ax
	pushw %ax
	mov saved_eax, %eax
	pushal
	mov saved_esp, %esp

	# restore 32bit interrupt descriptor table
	lidt (saved_idtr)
	sti
	popal
	pop %ebp
	ret
