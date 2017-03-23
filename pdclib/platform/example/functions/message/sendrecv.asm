
INT_VECTOR_SYS_CALL equ 0x90
GET_TICKS_VECTOR equ 0
SEND_RCV_VECTOR equ 1

global sendrec ;  sendrec(int function, int src_dest, MESSAGE* msg)
sendrec:
	mov	eax, SEND_RCV_VECTOR
	mov	ebx, [esp + 4]	; function
	mov	ecx, [esp + 8]	; src_dest
	mov	edx, [esp + 12]	; p_msg
	int	INT_VECTOR_SYS_CALL
	ret