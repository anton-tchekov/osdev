CC=gcc
CFLAGS=-m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector \
	-I src/ \
	-I src/gui/ \
	-I src/drivers/ \
	-I src/stdlib/ \
	-I src/keyboard/ \
	-I src/graphics/ \

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

SRCS=$(shell find src/ -name '*.c')
OBJS=$(join $(addprefix obj/, $(dir $(SRCS))), $(notdir $(SRCS:.c=.o)))

all: clean build

obj/src/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJS)
	$(ASM) $(ASMFLAGS) -o $(OBJ)/boot.o        $(SRC)/drivers/boot.s
	$(ASM) $(ASMFLAGS) -o $(OBJ)/interrupt.o   $(SRC)/drivers/interrupt.s

	$(CC) -T $(LINKER) -o $(BIN) $(LDFLAGS) $(OBJS) \
		$(OBJ)/boot.o \
		$(OBJ)/interrupt.o

	./isgrub.sh
	mkdir -p $(ISODIR)/boot/grub
	cp $(BIN) $(ISODIR)/boot/$(BIN)
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)

run:
	qemu-system-i386 -vga std -cdrom $(ISO)

clean:
	find $(OBJ)/ -name "*.o" -type f -delete
