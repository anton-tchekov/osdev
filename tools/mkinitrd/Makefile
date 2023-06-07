all:
	gcc main.c ../../shared/util/util.c -o mkinitrd -Wall -Wextra \
		-I ../../shared/types/ \
		-I ../../shared/atfs/ \
		-I ../../shared/util/

clean:
	rm -rf mkinitrd
