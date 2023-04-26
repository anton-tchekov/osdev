all:
	gcc main.c -I types/ -o emu -Wall -Wextra -pedantic

clean:
	rm -f emu
