all:
	gcc -I . \
	-Ifreetype/ \
	-I../../shared/types/ \
	-I../../shared/utf8/ \
	font-convert.c \
	../../shared/utf8/utf8.c \
	-lfreetype \
	-g -o font-convert -Wall -Wextra -pedantic

clean:
	rm -f font-convert
