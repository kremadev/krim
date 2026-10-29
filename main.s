.data
msg: .asciz "KRIM\n"

.text
.globl krim

krim:
mov $1, %eax
mov $1, %edi
lea msg(%rip), %esi
mov $0, %edx
for:
inc %edx
cmpb msg(%edx), 0
jne for
syscall

mov $60, %eax
mov $0, %rdi
syscall

.section .note.GNU-stack,"",@progbit
