; Test Program for 8080 Emulator

; Data storage areas
org 0x0100
mem_a: db 0    ; Memory location to test STA/LDA
mem_b: dw 0    ; Memory location to test SHLD/LHLD

; Start of test code
org 0x0200

start:
    ; Test STA and LDA
    mvi a, 0x55        ; Load accumulator with 0x55
    sta mem_a          ; Store A at address mem_a (0x0100)
    mvi a, 0x00        ; Clear A
    lda mem_a          ; Load A from address mem_a (0x0100)
    cpi 0x55           ; Compare A with 0x55 (should be equal)
    jnz error          ; Jump to error if not equal

    ; Test SHLD and LHLD
    mvi h, 0x01        ; Load H with 0x01
    mvi l, 0x02        ; Load L with 0x02 (HL = 0x0102)
    shld mem_b         ; Store HL at address mem_b (0x0101)
    xra a              ; Clear A (A = 0x00)
    lhld mem_b         ; Load HL from address mem_b (0x0101)
    mov a, h           ; Move H to A
    cpi 0x01           ; Compare A with 0x01 (should be equal)
    jnz error          ; Jump to error if not equal
    mov a, l           ; Move L to A
    cpi 0x02           ; Compare A with 0x02 (should be equal)
    jnz error          ; Jump to error if not equal

    ; Test LDAX B
    mvi b, 0x01        ; Load B with 0x01 (high byte of address)
    mvi c, 0x00        ; Load C with 0x00 (low byte of address)
    mvi a, 0xAA        ; Load A with 0xAA
    sta mem_a          ; Store A at address mem_a (0x0100)
    mvi a, 0x00        ; Clear A
    ldax b             ; Load A from address pointed to by BC (0x0100)
    cpi 0xAA           ; Compare A with 0xAA (should be equal)
    jnz error          ; Jump to error if not equal

    ; Test CNZ
    xra a              ; Clear A (A = 0x00, Zero flag set)
    jnz error          ; Jump to error if Zero flag is not set
    mvi a, 0x01        ; Load A with 0x01 (Zero flag cleared)
    jnz success        ; Jump to success if Zero flag is not set

error:
    hlt                ; Halt on error

success:
    nop                ; Success - continue execution (NOP as a placeholder)
    hlt                ; Halt on success

end:
