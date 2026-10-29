/*

_001: ENABLE RAW MODE
_002: EXIT

*/

.bss
raw: .skip 36
orig: .skip 36
buf: .skip 1024*1024
inbuf: .skip 32

.text
.globl krim

_001:
mov $16, %eax
mov $0, %edi
mov $0x5401, %esi
lea orig(%rip), %rdx
syscall

lea orig(%rip), %rsi
lea raw(%rip), %rdi
mov $36, %ecx
rep movsb

andl $~0xb, raw+12(%rip)
movb $1, raw+22(%rip)
movb $0, raw+23(%rip)

mov $16, %eax
mov $0, %edi
mov $0x5402, %esi
mov raw(%rip), %rdx
syscall
ret

_002:
mov $16, %eax
mov $0, %edi
mov $0x5402, %esi
lea orig(%rip), %rdx
syscall

mov $60, %eax
mov $0, %rdi
syscall

krim:
call _001



jmp _002


.section .note.GNU-stack,"",@progbit
