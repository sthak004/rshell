#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace std;

typedef tokenizer<char_separator<char> > mytok;

void displayPrompt(){
    char *user = getlogin(); //I assign the login name to a user pointer
    if(user == NULL){
        perror("getlogin");
        exit(1);
    }

    char hostname[100]; //I allocate space for the hostname
    if(gethostname(hostname, sizeof(hostname)) == -1){ //checking for errors withostname
        perror("gethostname");
        exit(1);
    }

    cout << user << '@' << hostname << '$' << ' '; //here I output the user and hostanme to the screen.

}


void parseCommands(string &cmd){
    vector<char*> single_commands;


}


/*void executeCommand(char **argv){
    int pid = fork();

    if(pid == -1){
        perror("There was some error with fork()");
        exit(1);
    }
    else if(pid == 0){
        cout << "We are in the child process!";
        if(-1 == execvp(*argv, argv)){
            perror("There was an error with execvp");
            exit(1);
        }
    }
    else if(pid > 0){
        if(-1 == wait(0)){
            perror("There was an error with wait()");
        }
    }
}*/


int main(void){
        /*string cmd; //command line to be inputted from user


        while(cmd != "exit"){
            displayPrompt(); //display the username and hostname
            getline(cin, cmd);
            //parseCommands(cmd);
        }*/


        return 0;
}

//notes to try
//------------
// if(strcmp(argv[0], "exit") == 0){
//              exit(0);
//         }
//------------
//execute command needs work
