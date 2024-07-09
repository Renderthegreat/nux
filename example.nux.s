.section .data
a: .long 0
.section .text
    movl $9, %eax
    movl %eax, %ebx
    movl %eax, a
.section .data
b: .long 0
.section .data
c: .long 0
.section .data
w: .asciz ""
.section .text
    .section .data
temp_83: .asciz "world"
    .section .text
    movl $temp_83, %eax
    movl %eax, w
.global main
main:
.section .data
x: .long 0
.section .text
    movl a, %eax
    movl %eax, %ebx
    movl %eax, x
.section .data
broken: .asciz ""
.section .text
    .section .data
temp_86: .asciz "hello"
    .section .text
    movl $temp_86, %eax
    movl %eax, broken
    ret #Final return on function main
