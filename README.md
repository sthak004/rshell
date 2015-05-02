#Rshell - a bash terminal emulator
Rshell emulates all behaviors a normal bash shell would do

##Installation
Installing is faily simple. Type these commands into your terminal and you're good to go!
```
$ git clone https://github.com/sthak004/rshell.git

$ cd rshell

$ git checkout hw0

$ make

$ bin/rshell
```

#Bugs and Limitations
##Rshell
**only single commands* currently work, as in `ls-a` or `cat <filename>`, ect

**no* logic operator capability yet

*`cd` not supported

*code isn't clean to read

*lack of output formatting and syntax coloring

##ls
*`ls` works on the current directory

*`ls` does print in alphabetical order

*when `ls` tries to open a directory that is passed in, undefined behavior occurs. (ex. `ls src`)

*`ls` `-a`, `-l`, `-R`, all have undefined behavior which I couldn't figure out.

*flags are not implemented completely because of the previous error.
