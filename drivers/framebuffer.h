#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/** fb_write_cell:
 * Writes a character with the given foreground and background to position i.
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position.
 */
void fb_move_cursor(unsigned short pos);

/** fb_clear:
 * Clears the screen with a black background.
 */
void fb_clear();

/** fb_write:
 * Writes a string to the screen.
 */
void fb_write(char *buf);

#endif /* FRAMEBUFFER_H */