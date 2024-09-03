section .text 
global _start 
_start: 
mov rax, 1 
mov rdi, 1 
lea rsi, [rel lit_1] 
mov rdx, 13 
syscall 
mov rax, 1 
mov rdi, 1 
mov r9, 80 
add r9, 10 
mov [lit_1], r9 
lea rsi, [rel lit_1] 
mov rdx, 2 
syscall
mov rax, 1
mov rdi, 1
lea rsi,[rel text1]
mov rdx, 24
syscall
mov rax, 1
mov rdi, 1
lea rsi,[rel text2]
mov rdx, 24
syscall
mov rax, 60 
mov rdi, 0 
syscall 
section .data 
lit_1 db "Hello World!", 0x0A
text1 db "What is the first number" 

text2 db "What is the second number" 

result db '0', 0 

