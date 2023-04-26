.PHONY: all cloc clean

all:
	gcc main.c -I types/ -o emu -Wall -Wextra -pedantic

cloc:
	cloc . --exclude-ext=html,htm,css,js,tex,sty,svg,md --exclude-dir=freetype

clean:
	rm -f emu
