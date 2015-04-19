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



void splitSpaces(char* argument, char **pendingArgs, int pos){
    string temp(argument);
    //int count = 1; //used for debugging
    for(unsigned int i = 0; i < temp.size(); ++i){
        if(temp.at(i) == ' '){
            int j = i;
            while(temp.at(j) == ' '){
                j++;
            }
            string part1 = temp.substr(0, i);
            //cout << "PART" << count << ": "  << part1 << endl;
            pendingArgs[pos] = const_cast<char*>(part1.c_str());
            string part2 = temp.substr(j, temp.size());
            //cout << "PART" << count++ << ": " << part2  << endl;
            pendingArgs[pos] = const_cast<char*>(part2.c_str());
        }
    }
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



int extractCommands(string &commandLine, char **cmds){
    const char* args = commandLine.c_str(); //the user input is converted to a const char* as we need it for execvp
    char* args_Mutatable = const_cast<char*>(args); //it needs to be mutable for us to tokenize it
    char *single_command;
    single_command = strtok(args_Mutatable, ";&|"); //execute strtok with delimeters
    
    int numOfArgs = 0; //when you add one --> argv[1]


    while(single_command != NULL){
        //cout << single_command << endl;
        cmds[numOfArgs] = strdup(single_command); //this processes each command into the command array for execution
        cout << "at position " << numOfArgs << ": " << cmds[numOfArgs];
        cout << endl;
        single_command = strtok(NULL, ";&|");
        numOfArgs++;
    }

    cmds[numOfArgs] = NULL;
    return numOfArgs;
}


vector<string>  extractConnectors(string inputLine){
    vector<string> cnctors;
    for(unsigned int i = 0; i < inputLine.size(); ++i){
        if(inputLine.at(i)  == ';'){
            cnctors.push_back(";");   
        }
        if((inputLine.at(i) == '&' && inputLine.at(i+1) == '&')){
            cnctors.push_back("&&");
        }
        if((inputLine.at(i) == '|' && inputLine.at(i+1) == '|')){
            cnctors.push_back("||");
        }
    }
    return cnctors;
}



void run(vector<string> &connectors, char **cmds){
    char *pendingArgs[10000]; //arguments waiting to be executed depending on connector 
    /*for(unsigned int i = 0; i < connectors.size(); ++i){
        if(connectors.at(i) == ";" && hasSpace(cmds[i])){
            cout << "BEFORE: " << cmds[i] << endl;
            pendingArgs[i] = cmds[i];
            cout << "AFTER: " << pendingArgs[i] << endl;
            executeCommand(pendingArgs);
        }
    }*/
    int i = 0;
    splitSpaces(cmds[0], pendingArgs, i);
}


int main(){
    string userInput; //command line in string format
    char *cmds[10000];
    do{ 
        displayPrompt(); //displays current user and hostname

        getline(cin, userInput); //get's input from user and stores it in a string
 
        /*gets all the delimeters and stores into a vector*/
        vector<string> connectors = extractConnectors(userInput);
        cout << "CONNECTORS: ";
        for(unsigned i = 0; i < connectors.size(); ++i){
            cout << connectors.at(i) << ' ';
        }
        cout << endl;


        //cout << userInput << endl; //NEED TO DELETE UPON COMPLETETION


        int numArgs = extractCommands(userInput, cmds); //retrieve number of arguments by parsing the string


        if(numArgs <= 0){continue;} //if there are no arguments, simply continue to the next iteration

        /*checks if first argument is exit and quits iff it's the only argument*/
        if( (strcmp(cmds[0], "exit") == 0) && (numArgs == 1) ) { break; }

        
        //run(connectors, cmds);

        if(executeCommand(cmds) != 0){
            cout << "Error in executing commands" << endl;
        }

    }while(1);
      

    return 0;
}

