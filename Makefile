CC=gcc
TARGET=emu
CFLAGS=\
	-I lib/ \
	-I types/ \
	-I platform/ \
	-I emulator/ \
	-Wall -Wextra \
	-Werror=uninitialized \
	-Werror=return-type \
	-Werror=implicit-function-declaration \
	-pedantic -std=c99 \
	-g \
	-l SDL2

LDFLAGS=
SRC=platform/simulator/platform.c \
	emulator/emulator.c \

.PHONY: all cloc clean
all:
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

cloc:
	cloc . --exclude-ext=html,htm,css,js,tex,sty,svg,md --exclude-dir=freetype

clean:
	rm -f $(TARGET)
