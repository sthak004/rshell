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

int extractCommands(string &commandLine, char **cmds){
    const char* args = commandLine.c_str(); //the user input is converted to a const char* as we need it for execvp
    char* args_Mutatable = const_cast<char*>(args); //it needs to be mutable for us to tokenize it
    char *single_command;
    single_command = strtok(args_Mutatable, " ;&|"); //execute strtok with delimeters
    
    int numOfArgs = 0; //when you add one --> argv[1]


    while(single_command != NULL){
        //cout << single_command << endl;
        cmds[numOfArgs] = strdup(single_command); //this processes each command into the command array for execution
        /*cout << "at position " << numOfArgs << ": " << cmds[numOfArgs];
        cout << endl;*/
        single_command = strtok(NULL, " ;&|");
        numOfArgs++;
    }

    return numOfArgs;
}


int executeCommand(char **args){
    int pid = fork();

    if(pid == -1){
        perror("There was some error with fork()");
        exit(1);
    }
    else if(pid == 0){
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

    return 0; //if everything goes well, return 0 
}


int main(){
    string userInput; //command line in string format
    char *cmds[10000];
    do{ 
        displayPrompt(); //displays current user and hostname

        getline(cin, userInput); //get's input from user and stores it in a string

        //cout << userInput << endl; //NEED TO DELETE UPON COMPLETETION

        int numArgs = extractCommands(userInput, cmds); //retrieve number of arguments by parsing the string

        if(numArgs <= 0){continue;} //if there are no arguments, simply continue

        /*checks if first argument is exit and quits iff it's the only argument*/
        if( (strcmp(cmds[0], "exit") == 0) && (numArgs == 1) ) { break; }
        else{
            if(executeCommand(cmds) != 0){
                cout << "Error in executing commands" << endl;
            }
        }

        
        //break;
    }while(1);

    return 0;
}

