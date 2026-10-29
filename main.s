.data
msg: .asciz "KRIM\n"

.text
.globl krim

krim:
mov $1, %eax
mov $1, %edi
lea msg(%rip), %rsi
mov $0, %edx
for:
inc %edx
cmpb $0, msg(%edx)
jne for
syscall

mov $60, %eax
mov $0, %rdi
syscall

.section .note.GNU-stack,"",@progbit
