# Mini OS: Kernel Development (Worksheet 2)

This project is a minimal 32-bit operating system kernel built from scratch. It started with a basic bootloader and framebuffer driver (Part 1) and evolved to handle hardware interrupts, keyboard input, and an interactive shell (Part 2).

The development was done on a headless Linux server (`csctcloud`), which required some specific workarounds for graphical output.

---

## Part 1: Booting & C Integration

The first goal was just to get the machine to boot and run C code.

* **Bootloader:** I wrote `loader.asm` to set up the Multiboot headers so GRUB would recognize my binary as a kernel.
* **Stack Setup:** Since C needs a stack to run, and there's no OS to give me one, I had to manually reserve 4KB of memory in the BSS section and point the `esp` register to it before calling `kmain`.
* **Driver:** I built a simple VGA driver using memory-mapped I/O. Writing characters to `0x000B8000` puts them on the screen. I used `volatile` pointers to make sure the compiler didn't optimize these writes away.

**Verification:**
To prove the boot worked without a screen, I checked the QEMU logs. Finding `EAX=CAFEBABE` confirmed my assembly loader ran correctly.

![OS Boot Success](os_success.png)

---

## Part 2: Inputs & Interrupts

This was the tricky part—making the hardware actually talk to the CPU.

### 1. Handling Interrupts
I had to remap the **Programmable Interrupt Controller (PIC)** because the default hardware interrupts conflict with CPU exceptions. I mapped them to offset `0x20` and set up an **Interrupt Descriptor Table (IDT)** to catch keyboard signals (IRQ 1).

### 2. The Keyboard Driver
The keyboard doesn't send ASCII; it sends scan codes. I wrote a driver (`keyboard.c`) to translate these codes into characters. I also added an `inb` assembly function to read byte data directly from the hardware ports.

### 3. Buffering Input
Typing is fast, but processing is slow. To fix this, I implemented a **Circular Buffer**.
* When a key is pressed, the interrupt handler immediately pushes the character into the buffer.
* When the kernel is ready, it pulls characters out. This prevents missed keys and allows for smooth typing.

### 4. The Shell
Finally, I built a basic command shell. Since I didn't have `<string.h>`, I wrote my own helper functions (`strcmp`, `strlen`) to parse input.
Supported commands:
* `help`: Lists commands.
* `clear`: Wipes the screen.
* `echo [text]`: Prints text back to you.

---

## Debugging & Challenges

Developing this on a remote server wasn't smooth sailing. Here are the main issues I hit and how I fixed them.

### The Headless Server Problem
The biggest issue was that the university server (`csctcloud`) has no display. The standard `-curses` flag for QEMU crashed with a GTK error.
**Fix:** I ran QEMU in **Monitor Mode** (`-monitor stdio`) and used the `screendump` command to save the video memory to an image file. This allowed me to verify the output without seeing a window.

### The "Green Block" Glitch
At first, typing caused ugly green blocks to overwrite the screen.
**Fix:** It turned out the reference code I used had some hardcoded debug logic that printed raw hex codes to index 0. I rewrote the interrupt handler to remove this and use my own `fb_write` function instead.

### Linker Errors
I kept getting "undefined reference" errors.
**Fix:** I realized I hadn't updated the Makefile to include the new driver files (`keyboard.o`, `pic.o`, etc.). Updating the object list fixed the build.

---

## Evidence of Success

Here is the final result: a working shell running on the bare-metal kernel. You can see the `help` command and the `echo` command working as expected.

![Shell Success](shell_success.ppm)

---

## How to Run

To compile and boot the system:

```bash
# 1. Clean up old files
make clean

# 2. Build and run (starts QEMU monitor)
make run