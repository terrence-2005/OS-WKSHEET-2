#include "../source/io.h"
#include "keyboard.h"
#include "framebuffer.h"

#define KEYBOARD_DATA_PORT 0x60
#define BUFFER_SIZE 256

// Circular Buffer Storage
static char key_buffer[BUFFER_SIZE];
static volatile int write_ptr = 0;
static volatile int read_ptr = 0;

// Initialize buffer indices
void keyboard_init_buffer() {
    write_ptr = 0;
    read_ptr = 0;
}

// Push character into buffer (Called by Interrupt Handler)
void keyboard_push(char c) {
    int next_write = (write_ptr + 1) % BUFFER_SIZE;
    
    // If buffer is full, discard input
    if (next_write == read_ptr) {
        return; 
    }
    
    key_buffer[write_ptr] = c;
    write_ptr = next_write;
    
    // Echo to screen immediately so user sees what they type
    if (c != '\n') { // Don't print newline yet, let kmain handle formatting
        char str[2] = { c, '\0' };
        fb_write(str);
    }
}

// Get character from buffer (Blocking - Waits for input)
char keyboard_getc() {
    // Wait loop: spin until buffer has data
    while (read_ptr == write_ptr) {
        asm volatile("nop"); 
    }
    
    char c = key_buffer[read_ptr];
    read_ptr = (read_ptr + 1) % BUFFER_SIZE;
    return c;
}

// Read a full line of text
void keyboard_readline(char* buffer, u32int size) {
    u32int i = 0;
    while (i < size - 1) {
        char c = keyboard_getc();
        
        if (c == '\n') {
            fb_write("\n"); // Move cursor to next line on screen
            break;
        }
        
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0'; // Null terminate string
}

// --- Hardware Logic ---

u8int keyboard_read_scan_code(void) {
    return inb(KEYBOARD_DATA_PORT);
}

u8int keyboard_scan_code_to_ascii(u8int scan_code) {
    if (scan_code & 0x80) return 0; // Ignore releases

    switch(scan_code) {
        case 0x02: return '1'; case 0x03: return '2'; case 0x04: return '3';
        case 0x05: return '4'; case 0x06: return '5'; case 0x07: return '6';
        case 0x08: return '7'; case 0x09: return '8'; case 0x0A: return '9';
        case 0x0B: return '0'; case 0x0C: return '-'; case 0x0D: return '=';
        case 0x0E: return '\b'; // Backspace
        case 0x10: return 'q'; case 0x11: return 'w'; case 0x12: return 'e';
        case 0x13: return 'r'; case 0x14: return 't'; case 0x15: return 'y';
        case 0x16: return 'u'; case 0x17: return 'i'; case 0x18: return 'o';
        case 0x19: return 'p'; case 0x1A: return '['; case 0x1B: return ']';
        case 0x1C: return '\n'; // Enter
        case 0x1E: return 'a'; case 0x1F: return 's'; case 0x20: return 'd';
        case 0x21: return 'f'; case 0x22: return 'g'; case 0x23: return 'h';
        case 0x24: return 'j'; case 0x25: return 'k'; case 0x26: return 'l';
        case 0x27: return ';'; case 0x28: return '\''; case 0x2B: return '\\';
        case 0x2C: return 'z'; case 0x2D: return 'x'; case 0x2E: return 'c';
        case 0x2F: return 'v'; case 0x30: return 'b'; case 0x31: return 'n';
        case 0x32: return 'm'; case 0x33: return ','; case 0x34: return '.';
        case 0x35: return '/'; case 0x39: return ' ';
        default: return 0;
    }
}