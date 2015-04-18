FLAGS=-Wall -Werror -ansi -pedantic
STD=-std=c++11

all:
	if [ ! -d bin ]; then mkdir bin; fi
	g++ $(STD) $(FLAGS) src/rshell.cpp -o bin/rshell

rshell:
	if [ ! -d bin ]; then mkdir bin; fi
	g++ $(STD) $(FLAGS) src/rshell.cpp -o bin/rshell
clean:
	rm -rf bin
