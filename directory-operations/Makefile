all: getcwdpathconf getcwdpathmax printaccesstest shownames

getcwdpathconf: getcwdpathconf.c
	g++ -std=c++14 -Wall -Werror -o getcwdpathconf getcwdpathconf.c

getcwdpathmax: getcwdpathmax.c
	g++ -std=c++14 -Wall -Werror -o getcwdpathmax getcwdpathmax.c

printaccesstest: printaccess.c printaccesstest.c
	g++ -std=c++14 -Wall -Werror -o printaccesstest printaccesstest.c printaccess.c

shownames: shownames.c
	g++ -std=c++14 -Wall -Werror -o shownames shownames.c

clean:
	rm -f getcwdpathconf getcwdpathmax printaccesstest shownames