all: fork_basics fork_memory fork_parent_child getpid minimal_fork minimal_wait fork_output_test

fork_basics: fork_basics.c
	g++ -std=c++14 -Wall -Werror -o fork_basics fork_basics.c

fork_memory: fork_memory.c
	g++ -std=c++14 -Wall -Werror -o fork_memory fork_memory.c

fork_parent_child: fork_parent_child.c
	g++ -std=c++14 -Wall -Werror -o fork_parent_child fork_parent_child.c

getpid: getpid.c
	g++ -std=c++14 -Wall -Werror -o getpid getpid.c

minimal_fork: minimal_fork.c
	g++ -std=c++14 -Wall -Werror -o minimal_fork minimal_fork.c

minimal_wait: minimal_wait.c
	g++ -std=c++14 -Wall -Werror -o minimal_wait minimal_wait.c

fork_output_test: fork_output_test.c
	g++ -std=c++14 -Wall -Werror -o fork_output_test fork_output_test.c

clean:
	rm -f fork_basics fork_memory fork_parent_child getpid minimal_fork minimal_wait fork_output_test