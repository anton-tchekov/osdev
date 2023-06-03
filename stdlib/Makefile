INCLUDE=\
	-I $(PREFIX)/algo/ \
	-I $(PREFIX)/alloc/ \
	-I $(PREFIX)/clipboard/ \
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
	-I $(PREFIX)/../keyboard/ \
	-I $(PREFIX)/../gfx-types/ \
	-I $(PREFIX)/../event-types/ \

SRC=\
	$(PREFIX)/algo/algo.c \
	$(PREFIX)/alloc/alloc.c \
	$(PREFIX)/clipboard/clipboard.c \
	$(PREFIX)/ctype/ctype.c \
	$(PREFIX)/event/event.c \
	$(PREFIX)/font/font.c \
	$(PREFIX)/font/font_default.c \
	$(PREFIX)/font/arial.c \
	$(PREFIX)/font/fatty.c \
	$(PREFIX)/font/ubuntu_bold.c \
	$(PREFIX)/font/ubuntu_regular.c \
	$(PREFIX)/format/format.c \
	$(PREFIX)/fs/fs.c \
	$(PREFIX)/gfx/gfx.c \
	$(PREFIX)/gui/gui.c \
	$(PREFIX)/keyboard/keyboard.c \
	$(PREFIX)/panic/panic.c \
	$(PREFIX)/rand/rand.c \
	$(PREFIX)/serial/serial.c \
	$(PREFIX)/string/string.c \
	$(PREFIX)/time/time.c \
	$(PREFIX)/vector/vector.c \
	$(PREFIX)/../status/status.c \
	$(PREFIX)/../utf8/utf8.c \

CFLAGS=\
	-Wall \
	-Wextra \
	-pedantic \
	$(INCLUDE) \
	-march=rv32im \
	-mabi=ilp32 \
	-Wl,-verbose \
	-nostdlib \
	-ffreestanding \
	-O$(OPT) \
	-DSTDLIB

LDFLAGS=\
	-T$(PREFIX)/linker.ld
