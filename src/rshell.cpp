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
#include <limits>

using namespace std;

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

void extractCommands(string &commandLine){
    const char* args = commandLine.c_str();
    char* args_Mutatable = const_cast<char*>(args);
    char *single_command;
    single_command = strtok(args_Mutatable, " ;&|");
    while(single_command != NULL){
        cout << single_command << endl;
        single_command = strtok(NULL, " ;&|");
    }

}



int main(){
    string userInput; //command line in string format

    while(1){
        displayPrompt(); //displays current user and hostname
        getline(cin, userInput); //get's input from user and stores it in a string
        cout << userInput << endl;
        extractCommands(userInput);
        break;
    }
    return 0;
}


/*void executeCommand(char **args){
    int pid = fork();

    if(pid == -1){
        perror("There was some error with fork()");
        exit(1);
    }
    else if(pid == 0){
        cout << "We are in the child process!";
        if(-1 == execvp(args[0], args)){
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

