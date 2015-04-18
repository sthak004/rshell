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

/*this parseCommands function will take a command line input and tokenize
 * each command */ 

int parseCommands(char commandLine[]){
    char delimeters[4]; //characters that we want to ignore when tokenizing
    char *args; //the individual arguments
    int numOfArgs; //the number of arguments

    strcpy(delimeters, " .;:||&&"); //set the delimeters

    args = strtok(commandLine, delimeters); //tokenize all the arguments in pointer of arguments called args
    
    numOfArgs = 1; //when we tokenize the first part, the count is 1

    //this while loop condition signifies that we continue tokenizing until we have reached the end of our character array which is terminated by a null terminating character
    while(args != '\0'){
        args = strtok(NULL, delimeters); //gets the next argument
        numOfArgs++; //increments the counter of arguments
    }

    return numOfArgs; //return the number of arguments
}


int main(void){
    char commandLine[100000000]; //the user input command line (char array)
    string commandLine_S; //the user input command line (string)

    strcpy(commandLine, commandLine_S.c_str()); //this will take the string and put in in the char array

    //parseCommands(commandLine);

    while(1){
        displayPrompt(); // display current user and hostname
        getline(cin, commandLine_S); //get user input
    }


    return 0;
}













//notes to try
//------------
// if(strcmp(argv[0], "exit") == 0){
//              exit(0);
//         }
//------------
//execute command needs work
//----------------------------------------
/*string cmd; //command line to be inputted from user
        while(cmd != "exit"){
            displayPrompt(); //display the username and hostname
            getline(cin, cmd);
            //parseCommands(cmd);
        }
*/
//----------------------------------------------------------------
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
