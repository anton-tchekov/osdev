CC=gcc
TARGET=emu
CFLAGS=\
	-I lib/ \
	-I types/ \
	-I util/ \
	-I platform/ \
	-I emulator/ \
	-I status/ \
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
	status/status.c \

.PHONY: all cloc clean
all:
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

cloc:
	cloc . --exclude-ext=html,htm,css,js,tex,sty,svg,md \
		--exclude-dir=freetype,assets

clean:
	rm -f $(TARGET)
