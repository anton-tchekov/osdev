INCLUDE=\
	-I $(PREFIX)/algo/ \
	-I $(PREFIX)/alloc/ \
	-I $(PREFIX)/ctype/ \
	-I $(PREFIX)/event/ \
	-I $(PREFIX)/font/ \
	-I $(PREFIX)/format/ \
	-I $(PREFIX)/fs/ \
	-I $(PREFIX)/gfx/ \
	-I $(PREFIX)/gui/ \
	-I $(PREFIX)/keyboard/ \
	-I $(PREFIX)/panic/ \
	-I $(PREFIX)/rand/ \
	-I $(PREFIX)/serial/ \
	-I $(PREFIX)/std/ \
	-I $(PREFIX)/string/ \
	-I $(PREFIX)/syscall/ \
	-I $(PREFIX)/time/ \
	-I $(PREFIX)/vector/ \
	-I $(PREFIX)/../status/ \
	-I $(PREFIX)/../types/ \
	-I $(PREFIX)/../utf8/ \
	-I $(PREFIX)/../util/ \

SRC=\
	-I $(PREFIX)/algo/algo.c \
	-I $(PREFIX)/alloc/alloc.c \
	-I $(PREFIX)/ctype/ctype.c \
	-I $(PREFIX)/event/event.c \
	-I $(PREFIX)/font/font.c \
	-I $(PREFIX)/font/font_default.c \
	-I $(PREFIX)/font/arial.c \
	-I $(PREFIX)/font/fatty.c \
	-I $(PREFIX)/font/ubuntu_bold.c \
	-I $(PREFIX)/format/format.c \
	-I $(PREFIX)/fs/fs.c \
	-I $(PREFIX)/gfx/gfx.c \
	-I $(PREFIX)/gui/gui.c \
	-I $(PREFIX)/keyboard/keyboard.c \
	-I $(PREFIX)/panic/panic.c \
	-I $(PREFIX)/rand/rand.c \
	-I $(PREFIX)/serial/serial.c \
	-I $(PREFIX)/string/string.c \
	-I $(PREFIX)/time/time.c \
	-I $(PREFIX)/vector/vector.c \
	-I $(PREFIX)/../status/status.c \
	-I $(PREFIX)/../utf8/utf8.c \

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
	$(INCLUDE)

LDFLAGS=\
	-T$(PREFIX)/linker.ld