CC=gcc
TARGET=emu
CFLAGS=\
	-I shared/types/ \
	-I shared/util/ \
	-I platform/ \
	-I platform/simulator/ \
	-I platform/shared/keyboard \
	-I emulator/ \
	-I shared/status/ \
	-I shared/keyboard/ \
	-I shared/event-types/ \
	-I shared/gfx-types/ \
	-Wall -Wextra \
	-Werror=uninitialized \
	-Werror=return-type \
	-Werror=implicit-function-declaration \
	-pedantic -std=c99 \
	-g \
	-l SDL2 \
	-DLINUX \

LDFLAGS=
SRC=platform/simulator/platform.c \
	emulator/emulator.c \
	platform/shared/keyboard/german.c \

.PHONY: all cloc clean
all:
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

cloc:
	cloc . --exclude-ext=html,htm,css,js,tex,sty,svg,md \
		--exclude-dir=freetype,assets

clean:
	rm -f $(TARGET)
