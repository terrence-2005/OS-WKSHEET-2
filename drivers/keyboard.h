#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#include "type.h"

#define KEYBOARD_MAX_ASCII 83

// Raw hardware functions
u8int keyboard_read_scan_code(void);
u8int keyboard_scan_code_to_ascii(u8int scan_code);

// Task 2: Buffer API functions
void keyboard_init_buffer();
void keyboard_push(char c);
char keyboard_getc();
void keyboard_readline(char* buffer, u32int size);

#endif /* INCLUDE_KEYBOARD_H */