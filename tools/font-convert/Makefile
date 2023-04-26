all:
	gcc -I . \
	-Ifreetype/ \
	-I../../types/ \
	-I../../utf8/ \
	font-convert.c \
	../../utf8/utf8.c \
	-lfreetype \
	-g -o font-convert -Wall -Wextra -pedantic

clean:
	rm -f font-convert
