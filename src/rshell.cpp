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


/*this function will execute the command in the vector of char**/
int executeCommand(vector<char*> rmSpcCmds){
    char *argv[10000]; //will store the command

    for(unsigned int i = 0; i < rmSpcCmds.size(); ++i){
        argv[i] = rmSpcCmds.at(i);
    }

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
        /*cout << "at position " << numOfArgs << ": " << cmds[numOfArgs];
        cout << endl;*/
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

/*this function will take the vectors of commands 
and remove all spaces and take each component and place
it in the vector of char*'s for execution*/
vector<char*> removeSpaces(vector<char*> singles){
    vector<char*> candycorn;
    
    /*this should take a SINGLE COMMAND and remove and spaces or tabs
      and put it in the candycorn vector*/ 

    char* token = strtok(singles.at(0), " \t");
    while(token != NULL){
        candycorn.push_back(token);

        token = strtok(NULL, " \t");

    }
    return candycorn;
}

template<typename T>
void printVector(vector<T> &temp){
   for(unsigned int i = 0; i < temp.size(); ++i){
       cout << "Pos " << i << ": " << temp.at(i) << endl; 
   }
}

void performLogic(vector<string> links, char **args){

    vector<char *> nextCommand; //this is the current command that will be exeucuted

    //for single commands that contain NO logic
    if(links.size() == 0){
        //add command to vector
        nextCommand.push_back(args[0]);

        //if the command has no space, then do not remove spaces ex. pwd
        if(noSpace(args[0])){
            printVector(nextCommand);
            executeCommand(nextCommand);
            //ERASE EVERYTHING IN VECTOR
            nextCommand.erase(nextCommand.begin(), nextCommand.end());
            return;
        }
        
        //OTHERWISE, we still have to remove spaces
        vector<char*> PASSTOEXEC = removeSpaces(nextCommand);
        executeCommand(PASSTOEXEC);
        //ERASE EVERYTHING IN VECTOR
        nextCommand.erase(nextCommand.begin(), nextCommand.end());
        return;
    }
}


int main(){
    string userInput; //command line in string format 
    char *special[10000]; //holds all the commands

    do{
        displayPrompt(); //display hostname

        getline(cin, userInput); // get input

        comments(userInput); // remove and commented input
        
        vector<string> connectors = getConnectors(userInput); 

        int numArgs = extractCommands(userInput, special); 

        if(numArgs <= 0){continue;} //if there are no arguments, continue to next iteration

        if ((strcmp(special[0], "exit") == 0) && (numArgs == 1)) {break;}

        performLogic(connectors, special);

    }while(1);
    return 0;
}
