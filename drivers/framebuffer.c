#include "../source/io.h"

/* The I/O ports */
#define FB_COMMAND_PORT      0x3D4
#define FB_DATA_PORT         0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Global cursor position variable - MOVED TO TOP */
int cursor_pos = 0;

/** fb_write_cell:
 * Writes a character with the given foreground and background to position i
 * in the framebuffer.
 *
 * @param i  The location in the framebuffer
 * @param c  The character
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);  // FIXED: bg and fg were swapped
}

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

/** fb_clear:
 * Clears the screen with a black background.
 */
void fb_clear() {
    for(int i = 0; i < 80 * 25; i++) {
        fb_write_cell(i, ' ', 15, 0); // Write empty space, white text on black
    }
    cursor_pos = 0;      // Reset cursor position
    fb_move_cursor(0);   // Move hardware cursor to start
}

/** fb_write:
 * Writes a string to the screen.
 */
void fb_write(char *buf) {
    unsigned int i = 0;
    while (buf[i] != '\0') {
        if(buf[i] == '\n') {
            // Handle newline by moving cursor to start of next row
            cursor_pos = (cursor_pos / 80 + 1) * 80;
        } else {
            fb_write_cell(cursor_pos, buf[i], 15, 0); // White on black
            cursor_pos++;
        }
        
        // Bounds check to prevent buffer overflow
        if(cursor_pos >= 80 * 25) {
            cursor_pos = 0; // Wrap to start
        }
        
        fb_move_cursor(cursor_pos); // Update hardware cursor
        i++;
    }
}