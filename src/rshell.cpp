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


/*void splitSpaces(char* argument, char **pendingArgs, int pos){
    string temp(argument);
    string first_word, second_word;

    for(unsigned int i = 0; i < temp.size(); ++i){
        if( !(isspace(temp.at(i))) && (second_word.size() == 0) ){
            first_word += temp.at(i);
        }
        else if( (isspace(temp.at(i)))){
            if( !(isspace(temp.at(i+1))) ){
                second_word += temp.at(i+1);
                ++i;   
            }
        }
        else if( !(isspace(temp.at(i))) ){
            second_word += temp.at(i);
        }
    }

    cout << "THE FIRST PIECE: " << first_word << endl;
    pendingArgs[pos] = const_cast<char*>(first_word.c_str());
    
    pos++;
    
    cout << "THE SECOND PIECE: " << second_word << endl;
    pendingArgs[pos] = const_cast<char*>(second_word.c_str());

    pos++;
    pendingArgs[pos] = NULL; //YOUR ARGUMENT ARRAY MUST END IN A NULL PTR
    return;
}*/




void splitSpaces(char* argument, char **pendingArgs, int pos){
    char *token = strtok(argument, " \t");
    while(token != NULL){
        //cout << token << endl;

        pendingArgs[pos] = strdup(token);

        token = strtok(NULL, " \t");
        pos++;
    }

    pendingArgs[pos] = NULL;
    return;
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
        //cout << "at position " << numOfArgs << ": " << cmds[numOfArgs];
        //cout << endl;
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

bool noSpace(char* single){
    string temp(single);
    for(unsigned int i = 0; i < temp.size(); ++i){
        if(temp.at(i) == ' '){
            return false;   
        }
    }

    return true;
}

void run(vector<string> &connectors, char **cmds){
    char *pendingArgs[10000]; //arguments waiting to be executed depending on connector 
    if(connectors.size() == 0){
        splitSpaces(cmds[0], pendingArgs, 0);
        if(executeCommand(pendingArgs) != 0){
            cout << "Error calling execvp";
            return;
        }
    }
    else if(connectors.size() == 0 && sizeof(cmds[0]) >= 2){
            cout << "PASSES FIRST TEST" << endl;
            splitSpaces(cmds[0], pendingArgs, 0);
            if(executeCommand(pendingArgs) != 0){
                cout << "Error calling execvp";
                return;
            }
    }

    for(unsigned int i = 0; i < connectors.size(); i++){
        if(connectors.at(i) == ";"){
            ;
        }
    }
}

int main(){
    string userInput; //command line in string format
    char *cmds[10000];
    do{ 
        displayPrompt(); //displays current user and hostname

        getline(cin, userInput); //get's input from user and stores it in a string

        //CHECK FOR COMMENTS BEFORE ANYTHING
        comments(userInput);
 
        /*gets all the delimeters and stores into a vector*/
        vector<string> connectors = extractConnectors(userInput);
        //cout << userInput << endl; //NEED TO DELETE UPON COMPLETETION

        int numArgs = extractCommands(userInput, cmds); //retrieve number of arguments by parsing the string

        if(numArgs <= 0){continue;} //if there are no arguments, simply continue to the next iteration

        /*checks if first argument is exit and quits iff it's the only argument*/
        if( (strcmp(cmds[0], "exit") == 0) && (numArgs == 1) ) { break; }
        
        run(connectors, cmds);

        /*if(executeCommand(cmds) != 0){
            cout << "Error in executing commands" << endl;
        }*/

        //exeRshell(tempInput, cmds);
    }while(1);
      

    return 0;
}
