CC=gcc
CFLAGS=-m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
LDFLAGS=-m32 -ffreestanding -O2 -nostdlib
TARGET=myos
BIN=$(TARGET).bin
ISO=$(TARGET).iso
OBJ=obj
SRC=src
ISODIR=isodir/
LINKER=linker.ld
ASM=nasm
ASMFLAGS=-f elf

all: clean build run

build:
	$(ASM) $(ASMFLAGS) -o $(OBJ)/boot.o $(SRC)/boot.s
	$(ASM) $(ASMFLAGS) -o $(OBJ)/interrupt.o $(SRC)/interrupt.s


	$(CC) $(CFLAGS) -c $(SRC)/kernel.c   -o $(OBJ)/kernel.o
	$(CC) $(CFLAGS) -c $(SRC)/terminal.c -o $(OBJ)/terminal.o
	$(CC) $(CFLAGS) -c $(SRC)/stdlib.c   -o $(OBJ)/stdlib.o
	$(CC) $(CFLAGS) -c $(SRC)/gdt.c      -o $(OBJ)/gdt.o
	$(CC) $(CFLAGS) -c $(SRC)/isr.c      -o $(OBJ)/isr.o
	$(CC) $(CFLAGS) -c $(SRC)/idt.c      -o $(OBJ)/idt.o
	$(CC) $(CFLAGS) -c $(SRC)/timer.c    -o $(OBJ)/timer.o
	$(CC) $(CFLAGS) -c $(SRC)/keyboard.c -o $(OBJ)/keyboard.o

	$(CC) -T $(LINKER) -o $(BIN) $(LDFLAGS) \
		$(OBJ)/boot.o \
		$(OBJ)/interrupt.o \
		$(OBJ)/keyboard.o \
		$(OBJ)/kernel.o \
		$(OBJ)/terminal.o \
		$(OBJ)/stdlib.o \
		$(OBJ)/idt.o \
		$(OBJ)/isr.o \
		$(OBJ)/gdt.o \
		$(OBJ)/timer.o \

	./isgrub.sh
	mkdir -p $(ISODIR)/boot/grub
	cp $(BIN) $(ISODIR)/boot/$(BIN)
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)

run:
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(OBJ)/*.o
