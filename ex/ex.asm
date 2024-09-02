section .text 
global _start 
_start: 
mov rax, 1 
mov rdi, 1 
lea rsi, [rel lit_1] 
mov rdx, 13 
syscall 
mov rax, 60 
mov rdi, 0 
syscall 
section .data 
lit_1 db "Hello World!", 0x0A
