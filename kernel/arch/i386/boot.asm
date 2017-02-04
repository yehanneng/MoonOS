
%define MULTIBOOT_HEADER_MAGIC  0x1BADB002
%define MULTIBOOT_HEADER_FLAGS	0x00000003
%define CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[section .multiboot]
align 4
dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd CHECKSUM

global stack_top
[section .bootstrap_stack]
stack_bottom:
times 16384 db 0
stack_top:

[section .text]
global _start
extern kernel_early,_init,kernel_main
_start:
    mov esp,stack_top
    push ebx
    call kernel_early
    add esp,4
    call _init

    call kernel_main

    cli
.Lhang:
    hlt
    jmp .Lhang
