#!/bin/bash

cd "/home/bradmarr/dev/MyBasic/"

g++ -std=c++20 main.cpp -o target/main

target/main "$@"

nasm -f elf64 ex/ex.asm

ld ex/ex.o -o ex/ex

echo "----------" 

ex/ex "$@"