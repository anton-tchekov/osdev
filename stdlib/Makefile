TARGET=test
OPT=2

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
	-I ctype/ \
	-I fs/ \
	-I gfx/ \
	-I gui/ \
	-I keyboard/ \
	-I rand/ \
	-I syscall/ \
	-I std/ \
	-I status/ \
	-I ../utf8/ \
	-I font/ \
	-I format/ \
	-I alloc/ \
	-I serial/ \
	-I algo/ \
	-I string/ \
	-I time/ \
	-I ../types/ \

SRC=\
	main.c \
	std/std.c \
	ctype/ctype.c \
	string/string.c \
	fs/fs.c \
	gfx/gfx.c \
	gui/gui.c \
	keyboard/keyboard.c \
	rand/rand.c \
	../utf8/utf8.c \
	font/font.c \
	font/font_default.c \
	font/arial.c \
	font/fatty.c \
	format/format.c \
	alloc/alloc.c \
	serial/serial.c \
	algo/algo.c \
	status/status.c \
	time/time.c \

all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) -T$(TARGET).ld
	riscv64-unknown-elf-objdump -d $(TARGET)
	riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin

clean:
	rm -f test

