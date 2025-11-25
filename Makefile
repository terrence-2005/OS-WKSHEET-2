OBJECTS = source/loader.o source/kmain.o source/io.o \
          drivers/framebuffer.o drivers/pic.o drivers/keyboard.o \
          drivers/interrupts.o drivers/interrupt_handlers.o drivers/interrupt_asm.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T source/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
	-b boot/grub/stage2_eltorito                \
	-no-emul-boot                               \
	-boot-load-size 4                           \
	-A os                                       \
	-input-charset utf8                         \
	-quiet                                      \
	-boot-info-table                            \
	-o os.iso                                   \
	iso

run: os.iso
	# Run QEMU with no display, but with a command monitor in the terminal
	qemu-system-i386 -display none -monitor stdio -boot d -cdrom os.iso -m 32

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf source/*.o drivers/*.o kernel.elf os.iso logQ.txt