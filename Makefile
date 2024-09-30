all: myfind

myfind: myfind.c
	gcc -std=c11 -Wall -Werror -o myfind myfind.c

clean:
	rm -f myfind

