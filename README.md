# Operating Systems: Worksheet 2 (Part 1)

This repo contains a minimal 32-bit OS kernel built from scratch. It boots via GRUB, sets up a stack for C execution, and implements a VGA framebuffer driver.

## Implementation Details

### Task 1: Booting & Assembly
* **Loader:** Wrote `loader.asm` with the required Multiboot magic number (`0x1BADB002`) and flags so the bootloader recognizes the kernel.
* **Linking:** Created `link.ld` to load the kernel at physical address `0x00100000` (1MB) to avoid conflicts with BIOS/GRUB memory.
* **Verification:** Verified the boot sequence by checking the QEMU log (`logQ.txt`) and confirming `EAX` held the value `0xCAFEBABE`.

### Task 2: C Integration
* **Stack Setup:** Since we don't have a standard library, I manually reserved 4KB of stack space in the `.bss` section and initialized the `esp` register in assembly before jumping to C.
* **Function Calls:** Implemented `kmain` alongside test functions (`sum_of_three`, `subtraction`, `multiplication`) to confirm that the stack frame and parameter passing were functioning correctly.

### Task 3: Framebuffer Driver
* **Memory Mapped I/O:** The driver writes directly to video memory at `0x000B8000`.
* **Volatile Casting:** I cast the memory pointer to `volatile unsigned short*`. This ensures the compiler doesn't optimize out the writes and handles the 16-bit cells (8-bit character + 8-bit color attribute) correctly.
* **Hardware Cursor:** Implemented `outb` assembly wrappers in `io.s` to talk to the VGA ports `0x3D4` (Command) and `0x3D5` (Data) for cursor updates.

---

## Execution & Screen Capture (Headless Server)

I developed this on the university `csctcloud` server, which does not have a graphical interface. Because of this, the standard `-curses` flag failed with a GTK initialization error.

**Workaround:**
To verify Task 3 output, I configured the Makefile to run QEMU in monitor mode (`-monitor stdio`) and used the internal `screendump` command to capture the state of the video memory.

**Output Proof:**
The image below shows the captured framebuffer state, confirming the driver works and prints "Welcome to My Tiny OS!".

![OS Screenshot](os_success.png)
![alt text](<Screenshot 2025-11-25 165419.png>)

---

## How to Run

To compile and boot the kernel:

```bash
make clean
make run