#!/bin/bash

nasm -f elf64 -s exp.asm -o exp.o
ld -m elf_x86_64 -s exp.o -o exp