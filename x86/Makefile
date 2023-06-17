all: clean build run

build:
	as --32 boot.s -o boot.o
	nasm -f elf -o interrupt.o interrupt.s
	gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
	gcc -m32 -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
	gcc -m32 -c stdlib.c -o stdlib.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

	gcc -m32 -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
	gcc -m32 -c isr.c -o isr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
	gcc -m32 -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
	gcc -m32 -c timer.c -o timer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

	gcc -m32 -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o interrupt.o kernel.o terminal.o stdlib.o idt.o isr.o gdt.o timer.o -fno-stack-protector
	./isgrub.sh
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run:
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f boot.o kernel.o
