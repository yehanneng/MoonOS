global out_byte
;void out_byte(uint16_t port,uint8_t value);
out_byte:
    mov edx,[esp + 4] ;port
    mov al,[esp + 8] ;value
    out dx,al
    nop
    nop
    ret

;uint8_t in_byte(uint16_t port)
global in_byte
in_byte:
    mov edx,[esp+4]
    xor eax,eax
    in al,dx
    nop
    nop
    ret

global port_read ;uint16_t port_read(uint16_t port)
port_read:
    mov edx,[esp + 4]
    xor eax,eax
    in ax,dx
    nop
    nop
    ret

global load_gdt
load_gdt:
    mov eax,[esp + 4]
    sgdt [eax]
    nop
    nop
    ret

global set_gdt
; void set_gdt(const char* gdtr)
set_gdt:
    mov eax,[esp + 4]
    lgdt [eax]
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    nop
    nop
    jmp 0x08:rlab
rlab:
    ret

global set_cr3
;void set_cr3(unsigned int )
set_cr3:
    mov eax,[esp + 4]
    mov cr3,eax
    mov eax,cr0
    or  eax,0x80000000
    mov cr0,eax
    ret

global load_idt
load_idt:
    mov eax,[esp+4]
    lidt [eax]
    nop
    nop
    ret

global load_tss ;load_tss(int tss_select)
load_tss:
    xor eax,eax
    mov eax,[esp+4]
    ltr ax
    nop
    nop
    ret

INT_VECTOR_SYS_CALL equ 0x90
GET_TICKS_VECTOR equ 0
SEND_RCV_VECTOR equ 1

global get_ticks ;int get_ticks(void)
get_ticks:
    mov eax,GET_TICKS_VECTOR
    int INT_VECTOR_SYS_CALL
    ret

global sendrec ;  sendrec(int function, int src_dest, MESSAGE* msg)
sendrec:
	mov	eax, SEND_RCV_VECTOR
	mov	ebx, [esp + 4]	; function
	mov	ecx, [esp + 8]	; src_dest
	mov	edx, [esp + 12]	; p_msg
	int	INT_VECTOR_SYS_CALL
	ret
