section .text 
global _start 
_start: 
mov rax, 1 
mov rdi, 1 
lea rsi, [rel lit1] 
mov rdx, 8 
syscall 
mov rax, 60 
mov rdi, 3 
syscall 
section .data 
lit1 db "testing", 0x0A
