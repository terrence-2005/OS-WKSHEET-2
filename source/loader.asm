global loader
extern kmain          ; <--- ADD OR ENSURE THIS LINE IS PRESENT

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                ; (magic number + checksum + flags should equal 0)

KERNEL_STACK_SIZE equ 4096      ; size of stack in bytes

section .bss
align 4                         ; align at 4 bytes
kernel_stack:
    resb KERNEL_STACK_SIZE      ; reserve stack for the kernel

section .text                   ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

loader:                              ; the loader label (defined as entry point in linker script)
    mov eax, 0xCAFEBABE             ; 1. Keep this for Task 1 verification
    
     ; ** NEW: SET UP THE STACK **
     mov esp, kernel_stack + KERNEL_STACK_SIZE 
    
     ; ** NEW: CALL C **
     call kmain                      ; Transfer control to your C kernel
    
 .loop:                              ; If kmain ever returns, loop forever to prevent crash
     jmp .loop                       ; loop forever



