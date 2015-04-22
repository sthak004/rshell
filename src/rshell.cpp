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
#include <algorithm>

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



int executeCommand(char **argv){
    int pid = fork();

    if(pid == -1){
        perror("There was some error with fork()");
        exit(1);
    }
    else if(pid == 0){
        if(execvp(argv[0], argv) == -1 ){
            perror("There was an error with execvp");
            exit(1);
        }
    }
    else if(pid > 0){
        if(-1 == wait(0)){
            perror("There was an error with wait");
            exit(1);
        }
    }

    return 0;
}



int extractCommands(string commandLine, char **cmds){
    const char* args = commandLine.c_str(); //the user input is converted to a const char* as we need it for execvp
    char* args_Mutatable = const_cast<char*>(args); //it needs to be mutable for us to tokenize it
    char *single_command;
    single_command = strtok(args_Mutatable, ";&|\t"); //execute strtok with delimeters
    
     
    int numOfArgs = 0; //when you add one --> argv[1]


    while(single_command != NULL){
        //cout << single_command << endl;
     
        cmds[numOfArgs] = strdup(single_command); //this processes each command into the command array for execution
        cout << "at position " << numOfArgs << ": " << cmds[numOfArgs];
        cout << endl;
        single_command = strtok(NULL, ";&|i\t");
        numOfArgs++;
    }

    cmds[numOfArgs] = NULL;
    return numOfArgs;
}


void comments(string &commandLine){
    int startingLocation = commandLine.find_first_of("#");
    if(startingLocation != -1){
        commandLine.erase(startingLocation);   
    }
}



bool noSpace(char* single){
    string temp(single);
    for(unsigned int i = 0; i < temp.size(); ++i){
        if(temp.at(i) == ' '){
            return false;   
        }
    }

    return true;
}



vector<string> getConnectors(string cmdLine){
    vector<string> connectors;

    for(unsigned int i = 0; i < cmdLine.size(); ++i){
        if(cmdLine.at(i) == ';'){
            connectors.push_back(";");
        }
        else if( (i+1) < cmdLine.size()){
            if(cmdLine.at(i) == '&' && cmdLine.at(i+1) == '&'){
                connectors.push_back("&&");
            }
            else if(cmdLine.at(i) == '|' && cmdLine.at(i+1) == '|'){
                connectors.push_back("||");
            }
        }
    }
    return connectors;
}


int main(){
    string userInput; //command line in string format 
    do{
        displayPrompt(); //display hostname

        getline(cin, userInput); // get input

        comments(userInput); // remove and commented input
        
        vector<string> testConnector = getConnectors(userInput); 
        for(unsigned i = 0; i < testConnector.size(); ++i){
            cout << testConnector.at(i) << ' ';
        }
        cout << endl;
    }while(1);
    return 0;
}
