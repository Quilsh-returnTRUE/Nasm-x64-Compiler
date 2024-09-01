#!/bin/bash

g++ -std=c++20 main.cpp -o target/main

target/main "$@"

nasm -f elf64 ex/ex.asm

ld ex/ex.o -o ex/ex

rm ex/ex.o

echo "----------" 

ex/ex "$@"