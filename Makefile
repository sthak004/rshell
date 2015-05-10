FLAGS=-Wall -Werror -ansi -pedantic
STD= --std=c++11

all: rshell ls

rshell:
	if [ ! -d bin ]; then mkdir bin; fi
	g++ -g $(STD) $(FLAGS) src/rshell.cpp -o bin/rshell
ls:
	if [ ! -d bin ]; then mkdir bin; fi
	g++ -g $(STD) $(FLAGS) src/ls.cpp -o bin/ls
pipe:
	if [ ! -d bin ]; then mkdir bin; fi
	g++ -g $(STD) $(FLAGS) src/pipe.cpp -o bin/pipe

clean:
	rm -rf bin
