global outb
global inb

; outb: preserves all caller-save registers (EAX, EBX, ECX, EDX) as good practice.
; This is critical for stable kernel execution.
outb:
    push ebx        ; Save EBX
    push ecx        ; Save ECX
    push edx        ; Save EDX, though DX is used later

    mov dx, [esp + 16]  ; Load 'port' (first argument) into DX. (Offset adjusts for 3 pushes + return address)
    mov al, [esp + 20]  ; Load 'data' (second argument) into AL - FIXED FROM esp+16
    out dx, al          ; Execute OUT instruction

    pop edx         ; Restore EDX
    pop ecx         ; Restore ECX
    pop ebx         ; Restore EBX
    ret             ; Return

; inb:
inb:
    push ebx
    push ecx
    push edx

    mov dx, [esp + 16]  ; Load 'port' into DX
    in  al, dx          ; Execute IN instruction

    pop edx
    pop ecx
    pop ebx
    ret
