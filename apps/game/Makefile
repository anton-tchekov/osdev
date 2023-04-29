TARGET=test
OPT=2

PREFIX=../../stdlib

CFLAGS=\
	-Wall \
	-Wextra \
	-pedantic \
	-march=rv32im \
	-mabi=ilp32 \
	-Wl,-verbose \
	-nostdlib \
	-ffreestanding \
	-O$(OPT) \
	-I $(PREFIX)/ctype/ \
	-I $(PREFIX)/fs/ \
	-I $(PREFIX)/gfx/ \
	-I $(PREFIX)/gui/ \
	-I $(PREFIX)/keyboard/ \
	-I $(PREFIX)/rand/ \
	-I $(PREFIX)/syscall/ \
	-I $(PREFIX)/std/ \
	-I $(PREFIX)/../utf8/ \
	-I $(PREFIX)/font/ \
	-I $(PREFIX)/format/ \
	-I $(PREFIX)/alloc/ \
	-I $(PREFIX)/serial/ \
	-I $(PREFIX)/algo/ \
	-I $(PREFIX)/string/ \
	-I $(PREFIX)/time/ \
	-I ../../types/ \
	-I ../../util/ \
	-I ../../status/ \
	-I .

SRC=\
	main.c \
	$(PREFIX)/std/std.c \
	$(PREFIX)/ctype/ctype.c \
	$(PREFIX)/string/string.c \
	$(PREFIX)/fs/fs.c \
	$(PREFIX)/gfx/gfx.c \
	$(PREFIX)/gui/gui.c \
	$(PREFIX)/keyboard/keyboard.c \
	$(PREFIX)/rand/rand.c \
	$(PREFIX)/../utf8/utf8.c \
	$(PREFIX)/font/font.c \
	$(PREFIX)/font/font_default.c \
	$(PREFIX)/font/arial.c \
	$(PREFIX)/font/fatty.c \
	$(PREFIX)/font/ubuntu_bold.c \
	$(PREFIX)/format/format.c \
	$(PREFIX)/alloc/alloc.c \
	$(PREFIX)/serial/serial.c \
	$(PREFIX)/algo/algo.c \
	$(PREFIX)/../status/status.c \
	$(PREFIX)/time/time.c \

all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) -T$(PREFIX)/$(TARGET).ld
	riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin

# riscv64-unknown-elf-objdump -d $(TARGET)


clean:
	rm -f test

