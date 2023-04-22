all:
	gcc main.c -o emu -Wall -Wextra -pedantic

clean:
	rm -f emu
