main: ; comment
    mov r0 10      ; comment
    mov r0 r1
    mov r0 [0x10]
    mov r0 0x01
    sub r0 r1
    add r0 r1
    xor r0 r1
    and r0 r1
    or  r0 r1
    cmp r0 r1
    nop
    jmp 2
    jmp main
    hlt    
