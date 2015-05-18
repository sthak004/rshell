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

##I/O Redirection && Piping
* piping, unfortunately, isn't implemented
* Due to a getline(cin, string) bug I was unable to fix in my rshell, you will only be able to call one command everytime you call bin/rshell. 
* when performing input redirection, `ls` cannot take any parameters otherwise the parameters will become files.
*   example: `ls -l` `>` `file.txt` will create `-l` as a file as well as `file.txt`. However, `file.txt` will not contain anything.  Same goes for the `>>` connector.
* `echo "hello > file2.txt` has the same bug as above where it will create `"hello"` and `file2.txt` as files, but file2.txt will not contain anything.
* `cat <<< "hello ` WITHOUT the closing quote will NOT mimic bash in that it will wait for the closing quote before outputting as bash does; rather, it will simply out whatever you type.
