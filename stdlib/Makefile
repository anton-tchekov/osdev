INCLUDE=\
	-I $(PREFIX)/algo/ \
	-I $(PREFIX)/disk/ \
	-I $(PREFIX)/alloc/ \
	-I $(PREFIX)/clipboard/ \
	-I $(PREFIX)/ctype/ \
	-I $(PREFIX)/event/ \
	-I $(PREFIX)/font/ \
	-I $(PREFIX)/format/ \
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
	-I $(PREFIX)/../shared/status/ \
	-I $(PREFIX)/../shared/types/ \
	-I $(PREFIX)/../shared/utf8/ \
	-I $(PREFIX)/../shared/util/ \
	-I $(PREFIX)/../shared/keyboard/ \
	-I $(PREFIX)/../shared/gfx-types/ \
	-I $(PREFIX)/../shared/event-types/ \

# -I $(PREFIX)/fs/ \

SRC=\
	$(PREFIX)/algo/algo.c \
	$(PREFIX)/disk/disk.c \
	$(PREFIX)/alloc/alloc.c \
	$(PREFIX)/clipboard/clipboard.c \
	$(PREFIX)/ctype/ctype.c \
	$(PREFIX)/event/event.c \
	$(PREFIX)/font/font.c \
	$(PREFIX)/font/font_default.c \
	$(PREFIX)/format/format.c \
	$(PREFIX)/gfx/gfx.c \
	$(PREFIX)/gui/gui.c \
	$(PREFIX)/keyboard/keyboard.c \
	$(PREFIX)/panic/panic.c \
	$(PREFIX)/rand/rand.c \
	$(PREFIX)/serial/serial.c \
	$(PREFIX)/string/string.c \
	$(PREFIX)/time/time.c \
	$(PREFIX)/vector/vector.c \
	$(PREFIX)/../shared/status/status.c \
	$(PREFIX)/../shared/utf8/utf8.c \

# $(PREFIX)/fs/fs.c \

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
