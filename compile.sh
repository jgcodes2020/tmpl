#!/bin/bash

g++ $1 @compile_flags.txt -o main || exit
g++ $1 @compile_flags.txt -S -masm=intel -fverbose-asm
./main