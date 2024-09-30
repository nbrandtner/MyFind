all: myfind

myfind: myfind.c
	g++ -std=c++14 -Wall -Werror -o myfind myfind.c

clean:
	rm -f myfind

