all: getopt mycommand

getopt: getopt.c
	g++ -std=c++14 -Wall -Werror -o getopt getopt.c

mycommand: mycommand.c
	g++ -std=c++14 -Wall -Werror -o mycommand mycommand.c

clean:
	rm -f getopt mycommand

