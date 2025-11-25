#include "../drivers/framebuffer.h"

// =======================================================
// Task 2: Function Definitions
// =======================================================

int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

int subtraction(int a, int b) {
    return a - b;
}

int multiplication(int arg1, int arg2) {
    return arg1 * arg2;
}

// =======================================================
// Task 2 & 3: Kernel Entry Point
// =======================================================

void kmain(void) {
    // CRITICAL FIX: Add this loop back. It prevents the "silent crash"
    // by letting the hardware stabilize before we touch it.
    for (int i = 0; i < 1000000; i++) {
        asm volatile("nop");
    }

    // 1. Clear the screen
    fb_clear(); 

    // 2. Move cursor to start
    fb_move_cursor(0);

    // 3. Print output
    fb_write("Welcome to My Tiny OS!\n");
    fb_write("Task 2 and 3 fully initialized.\n");
    
    // 4. Execute Task 2 functions
    int sum = sum_of_three(1, 2, 3);
    int sub = subtraction(10, 4);
    int mul = multiplication(2, 5);
    
    // Silence warnings
    (void)sum;
    (void)sub;
    (void)mul;
    
    // 5. Halt loop
    while(1) {
        asm volatile("hlt");
    }
}