#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/keyboard.h"

// =======================================================
// 1. STRING HELPER FUNCTIONS
// (We need these because we don't have <string.h>)
// =======================================================

/** strcmp: Returns 0 if strings match exactly */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

/** strncmp: Compare first n characters (useful for arguments) */
int strncmp(char s1[], char s2[], int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        if (s1[i] == '\0') return 0;
    }
    return 0;
}

/** strlen: Calculate length of string */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

// =======================================================
// 2. COMMAND HANDLERS
// =======================================================

void cmd_help() {
    fb_write("Available Commands:\n");
    fb_write("  help     - Show this message\n");
    fb_write("  clear    - Clear the screen\n");
    fb_write("  echo [t] - Print text [t] to screen\n");
}

void cmd_clear() {
    fb_clear();
    fb_move_cursor(0);
    fb_write("Welcome to My Mini OS!\n");
}

void cmd_echo(char* input) {
    // Command format: "echo some text"
    // We skip the first 5 chars ("echo ") to print the rest
    if (strlen(input) > 5) {
        fb_write(input + 5); 
        fb_write("\n");
    } else {
        fb_write("\n"); // Empty echo
    }
}

// Decides which function to call based on input
void execute_command(char* input) {
    if (input[0] == '\0') {
        return; // Empty input, do nothing
    }

    if (strcmp(input, "help") == 0) {
        cmd_help();
    } 
    else if (strcmp(input, "clear") == 0) {
        cmd_clear();
    }
    else if (strncmp(input, "echo ", 5) == 0) {
        cmd_echo(input);
    }
    else {
        fb_write("Unknown command: ");
        fb_write(input);
        fb_write("\nType 'help' for list of commands.\n");
    }
}

// =======================================================
// 3. KERNEL MAIN
// =======================================================

void kmain(void) {
    // Stability delay
    for (int i = 0; i < 1000000; i++) { asm volatile("nop"); }

    fb_clear();
    fb_write("Welcome to My Tiny OS!\n");
    fb_write("Task 3: Shell Ready.\n");

    // Initialize Drivers
    interrupts_install_idt();
    keyboard_init_buffer();

    // Buffer for storing user input
    char input_buffer[100];

    while(1) {
        fb_write("myos> ");
        
        // 1. Wait for user to type a line and press Enter
        keyboard_readline(input_buffer, 100);
        
        // 2. Process the command
        execute_command(input_buffer);
    }
}