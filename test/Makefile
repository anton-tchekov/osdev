TARGET=test
OPT=2

CFLAGS=\
	-march=rv32im \
	-mabi=ilp32 \
	-Wl,-verbose \
	-nostdlib \
	-ffreestanding \
	-O$(OPT) \
	-I. \
	-I ctype/ \
	-I fs/ \
	-I gfx/ \
	-I gui/ \
	-I keyboard/ \
	-I rand/ \
	-I syscall/ \
	-I std/ \
	-I status/

SRC=\
	main.c \
	std/std.c \
	ctype/ctype.c \
	fs/fs.c \
	gfx/gfx.c \
	gui/gui.c \
	keyboard/keyboard.c \
	rand/rand.c \
	status/status.c

all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) -T$(TARGET).ld
	riscv64-unknown-elf-objdump -d $(TARGET)
	riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin

clean:
	rm -f test

