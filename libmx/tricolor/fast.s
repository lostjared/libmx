.data


.text


.globl swizzle
.type	swizzle, @function




swizzle:
	
	enter $0x0, $0x0
	movl 0x8(%ebp), %ecx
	movl 0xc(%ebp), %edx
	movl $0x0, %eax
set_loop:
	movl (%ecx,%eax,1), %ebx
	rorl $0x3, %ebx
	xorl $0x1c, %ebx
	movl %ebx, (%ecx,%eax,1)
	incl %eax
	cmpl %edx, %eax
	jne set_loop
	movl $0, %eax
end:
	addl $12, %esp
	leave
	ret 
