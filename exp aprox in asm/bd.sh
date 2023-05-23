#!/bin/bash

nasm -f elf64 -g -F dwarf exp.asm -l exp.lst
gcc -g -o exp exp.o -no-pie