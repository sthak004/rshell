#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

/* BUGS:
 * 1. out of range error when ending with a connector '<' or '>'
 * 2. ls > output < input doesn't work the way bash does
 * 3. echo hello > file.txt doesn't work the way bash does. it creates
 *    two files called "hello" and "file.txt"
 * 4. cat <<< "hello (with one quote) doesn't work as bash does
 * 5. g++ main.cpp (num)>err doesn't work when > and err are next to each other*/

using namespace std;

/* bools for setting file descriptors */
bool is_fd_in = false , is_fd_out = false, is_fd_err = false;

/* function1: prints out any type */
#define printVec(x) for(unsigned int i = 0; i < x.size(); i++) \
                        cout << x.at(i) << ' '; \
                    cout << endl;


/* function2: prints out any regular variable */
template<typename T>
void printReg(T &sample){
    cout << sample << endl;
    return;
}

/* function3: extract componenets in a vector using strtok */
vector<char*> getParts(string &wut){
    /* vector of each piece to return*/
    vector<char*> brokenParts;

    /* convert to const char* to allow for parameter match*/
    const char* wut_cstring = wut.c_str();

    /* cast it to allow char* paramter cabability*/
    char* wut_cstring_2 = const_cast<char*>(wut_cstring);

    /* each individual command will be stored here temporarily*/
    char* token;

    /* extract with delimeters*/
    token = strtok(wut_cstring_2, " ");

    /* counter for list of argument parameters*/
    int pos = 0;

    /*extract until end of line*/
    while(token != NULL){
        /* output each token */
        //cout << "token" << pos << ": " << token << endl;

        /* push each token onto vector */
        brokenParts.push_back(token);

        /* keep tokenizing*/
        token = strtok(NULL, " ");

        /*increment counter */
        pos++;
    }

    /* return vector of char* */
    return brokenParts;
}

/* function4: performs input redirection */
/* parmeter1: command that you will run on the files*/
/* parmeter2: the vector of files to open and redirect input on */
/* parmeter3: list of all arguments (needed for execvp) */
void inputRedirection(char* command, vector<char*> files, char **argv){
    /* flush standard in so that, if the forked child writes to standard
     * error because of a problem, there will be no extraneous duplicated
     * output.*/
    fflush(STDIN_FILENO);

    /*open up all the files in the vector of files */
    for(unsigned int i = 0; i < files.size(); i++){
        int fd0;
        if( (fd0 = open(files.at(i), O_RDONLY)) == -1){
            perror(files.at(i));
            exit(1);
        }
        /* duplicate the file descriptor */
        if(dup2(fd0, STDIN_FILENO) < 0){
            perror("dup2");
            exit(1);
        }

        /* close the file descriptor */
        if(close(fd0) < 0){
            perror("close");
            exit(1);
        }
    }


    /* pid gets the value of fork*/
    int pid = fork();

    /* based on the value of fork, perform the following operations */
    if(pid == -1){ //fork straight up failed you...
        perror("fork() failed");
        exit(1);
    }
    else if(pid == 0){ //child process = good *thumbs up*
        /* run the command passed in */
        if( execvp(command, argv) == -1){
            perror("error running execvp");
            exit(1);
        }
    }
    else if(pid > 0){ //we have to wait for the child process to die
        if ( -1 == wait(0)){
            perror("there was an error with wait");
        }
    }
    return;
}

/* function5: performs output redirection */
/* parmeter1: command that you will run on the files*/
/* parmeter2: the vector of files to open and redirect output on */
/* parmeter3: list of all arguments (needed for execvp) */

void outputRedirection(char* command, vector<char*> files, char** argv){
    /* flush standard out so that, if the forked child writes to standard
     * error because of a problem, there will be no extraneous duplicated
     * output.*/
    fflush(stdout);

     /*open up all the files in the vector of files */
    for(unsigned int i = 0; i < files.size(); i++){
        int fd1;
        if ((fd1 = open(files.at(i), O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0){
            perror("open");
            exit(1);
        }

        /* duplicate file descriptor */
        if (dup2(fd1, STDOUT_FILENO) < 0){
            perror("dup2");
            exit(1);
        }

        /* close the file descriptor */
        if( (close(fd1)) < 0){
            perror("close");
            exit(1);
        }
    }


    /* pid gets the value of fork*/
    int pid = fork();

    /* based on the value of fork, perform the following operations */
    if(pid == -1){ //fork straight up failed you...
        perror("fork() failed");
        exit(1);
    }
    else if(pid == 0){ //child process = good *thumbs up*
        /* run the command passed in */
        if( execvp(command, argv) == -1){
            perror("error running execvp");
            exit(1);
        }
    }
    else if(pid > 0){ //we have to wait for the child process to die
        if ( -1 == wait(0)){
            perror("there was an error with wait");
        }
    }
    return;
}

/* function6: in_out_redirection */
/* this function perform input and output redirection together
 * if '<' and '>' are spotted on the same line */
/* parmeter1: command that you will run on the files*/
/* parmeter2: the vector of files to open and redirect output on */
/* parmeter3: list of all arguments (needed for execvp) */

void in_out_redirection(char* command, vector<char*> files,
                        vector<char*> connectors, char** argv, bool special, bool s_o,
                        bool fdin, bool fdout, bool fderr){
    /* flush both STDIN and STDOUT */
    //fflush(STDIN_FILENO);
    //fflush(stdout);
    if(s_o){
        /* if you have the <<< operator, then you know that the second and ONLY
         * parameter is a c-string and thus you must print it to standard out */
        string c_string;
        for(unsigned int i = 0; i < files.size(); i++){
            c_string += files.at(i);
            c_string += ' ';
        }

        unsigned count = 0;
        for(unsigned int j = 0; j < c_string.size(); j++){
            if( c_string.at(j) == '\"'){
                count++;
            }
        }

        if( (count % 2) == 0){
            /* however, you must remove the quotes first... */
            c_string.erase(remove(c_string.begin(), c_string.end(), '\"'), c_string.end());
            cout << c_string << endl;
            return;
        }

    }
    else{
        vector<char*> infiles;
        vector<char*> outfiles;

        for(unsigned int i = 0; i < files.size(); i++){
            string temp_connector;
            temp_connector.assign(connectors.at(i));

            /* need the length to differentiate between a '>' and ">>" */
            unsigned int connector_length = temp_connector.size();

            if(*(connectors.at(i)) == '<'){
                infiles.push_back(files.at(i));
            }
            else if(connector_length == 2 && (temp_connector[0] == '>' && temp_connector[1] == '>')){

                outfiles.push_back(files.at(i));
            }
            else if(*(connectors.at(i)) == '>'){
                outfiles.push_back(files.at(i));
            }
        }

        for(unsigned int x = 0; x < infiles.size(); x++){
            int in;
            if( (in = open(infiles.at(x), O_RDONLY)) == -1){
                perror("open");
                exit(1);
            }

            if( (dup2(in, STDIN_FILENO)) < 0){
                perror("dup2");
                exit(1);
            }

            if( close(in) < 0 ){
                perror("close");
                exit(1);
            }
        }
        if(special == true){
            for(unsigned int z = 0; z < outfiles.size(); z++){
                int out;
                if( (out = open(outfiles.at(z), O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1){
                    perror("open");
                    exit(1);
                }

                if( (dup2(out, STDOUT_FILENO)) < 0){
                    perror("dup2");
                    exit(1);
                }

                if( close(out) < 0){
                    perror("close");
                    exit(1);
                }
            }
        }
        else{
            for(unsigned int z = 0; z < outfiles.size(); z++){
                int out;
                if( (out = open(outfiles.at(z), O_WRONLY | O_CREAT, 0644)) < 0){
                    perror("open");
                    exit(1);
                }

                if( (dup2(out, STDOUT_FILENO)) < 0){
                    perror("dup2");
                    exit(1);
                }


                if( close(out) < 0){
                    perror("close");
                    exit(1);
                }
            }
        }
    }

    /* pid gets the value of fork*/
    int pid = fork();

    /* based on the value of fork, perform the following operations */
    if(pid == -1){ //fork straight up failed you...
        perror("fork() failed");
        exit(1);
    }
    else if(pid == 0){ //child process = good *thumbs up*
        /* run the command passed in */
        if( execvp(command, argv) == -1){
            perror("error running execvp");
            exit(1);
        }
    }
    else if(pid > 0){ //we have to wait for the child process to die
        if ( -1 == wait(0)){
            perror("there was an error with wait");
        }
    }

    return;
}

/* function7: extracts connectors */
/* this function extracts the connectors and stores them in one vector
 * and the rest of the files and/or commands into another vector*/
/* parameter1: the vector of ALL the words and connectors
 * parameter2: the vectors of words we will extract from the first parameter*/
vector<char*> extractConnectors(vector<char*> &commands, vector<char*> &words){
    /* create a vector for connectors*/
    vector<char*> connectors;

    /* loop through the commands vectors and push back all connectors onto
     * the connectors vector */
    for(unsigned int i = 0; i < commands.size(); i++){
        /* create a temporary string to check for a ">>" connector */
        string temp_connector;
        temp_connector.assign(commands.at(i));

        /* need the length to differentiate between a '>' and ">>" */
        unsigned int connector_length = temp_connector.size();

        /* if you find a '<' push it onto connectors vector*/
        if(connector_length == 1 && *(commands.at(i)) == '<'){
            connectors.push_back(commands.at(i));
        }
        /* if you find a '>' push it onto the connectors vector*/
        else if(connector_length == 1 && *(commands.at(i)) == '>'){
            connectors.push_back(commands.at(i));
        }
        else if(connector_length == 2 && temp_connector[1] == '>'){
            //char connector[1] = {'>'};
            //char* connector_c = (char*)connector;

            if(temp_connector[0] == '0' && temp_connector[1] == '>'){
                //is_fd_in = true;
                //connectors.push_back(connector_c);
                ;
            }
            else if(temp_connector[0] == '1' && temp_connector[1] == '>'){
                //char one[1] = {'1'};
                //char* one_c = (char*)one;
                ;
            }
            else if(temp_connector[0] == '2' && temp_connector[1] == '>'){
                is_fd_err = true;
                connectors.push_back(commands.at(i));
            }
            else if(temp_connector[0] == '>' && temp_connector[1] == '>'){
                //cout << "the special character exists" << endl;
                connectors.push_back(commands.at(i));
            }
        }
        else if(connector_length == 3 && temp_connector[0] == '<' &&
                                         temp_connector[1] == '<' &&
                                         temp_connector[2] == '<'){
            connectors.push_back(commands.at(i));
        }
        /* if you find a '|' push it onto the connectors vector*/
        else if(connector_length == 1 && *(commands.at(i)) == '|'){
            connectors.push_back(commands.at(i));
        }
        /* if you fail to find any of the connectors above, then push the file
         * or command onto the words vector*/
        else{
            words.push_back(commands.at(i));
        }
    }

    return connectors;
}


/* function8: perform logic - IO/pipe */
void logic(vector<char*> &connectors, vector<char*> &words, char **argv){
    /* bool to signify it is a ">>" sign */
    bool appended = false;

    /* bool for the "<<<" operator */
    bool string_out = false;

    /* we assume that the first input is a command and the rest of the
     * componenets in the vector will be files that are either created
     * or will be created */
    char* command = words.at(0);

    /* erase the first element in the vector<char*> words as it is the
     * command and we should not parse through that */
    words.erase(words.begin());


    /* 1. if there is only connector then only execute inputRedirection
     * once*/
    if(connectors.size() == 1 && *(connectors.at(0)) == '<'){
        string temp_connector;
        temp_connector.assign(connectors.at(0));

        unsigned int connector_length = temp_connector.size();

        if(connector_length > 1 && temp_connector[0] == '<' && temp_connector[1] == '<'
                                && temp_connector[2] == '<'){
            /* the bool that signifies the ">>" connector */
            string_out = true;
            in_out_redirection(command, words, connectors, argv, appended, string_out,
                               is_fd_in, is_fd_out, is_fd_err);
            return;
        }
        inputRedirection(command, words, argv);
        return;
    }
    /* if the vector contain's all '<' signs, then only call
     * inputRedirection once. */
    /* in other words, if you ever see a '>' or '|' sign then stop*/
    else if(connectors.size() > 1 && *(connectors.at(0)) == '<'){
        for(unsigned int i = 0; i < connectors.size(); i++){
           if(*(connectors.at(i)) == '>' || *(connectors.at(i)) == '|'){
               in_out_redirection(command, words, connectors, argv, appended, string_out,
                                  is_fd_in, is_fd_out, is_fd_err);
               return;
           }
        }
        /* the vector contains all '<' sign so call inputRedirection once!*/
        inputRedirection(command, words, argv);
    }
    /* if there is only one '>' connector, then only execute
     * outputRedirection once*/
    else if(connectors.size() == 1 && ((*(connectors.at(0)) == '>') ||
                                       (*(connectors.at(0)) == '2'))){
        string temp_connector;
        temp_connector.assign(connectors.at(0));

        unsigned int connector_length = temp_connector.size();

        if(connector_length > 1 && temp_connector[0] == '>' && temp_connector[1] == '>'){
            /* the bool that signifies the ">>" connector */
            appended = true;
            in_out_redirection(command, words, connectors, argv, appended, string_out,
                               is_fd_in, is_fd_out, is_fd_err);
            return;
        }
        else if(connector_length > 1 && temp_connector[0] == '2' && temp_connector[1] == '>'){
            in_out_redirection(command, words, connectors, argv, appended, string_out, is_fd_in, is_fd_out, is_fd_err);
            return;
        }
        outputRedirection(command, words, argv);
        return;
    }
    /* if the vector contain's all '>' signs, then only call
     * outputRedirection once. */
    else if(connectors.size() > 1 && *(connectors.at(0)) == '>'){
        for(unsigned int i = 0; i < connectors.size(); i++){
           string temp_connector;
           temp_connector.assign(connectors.at(i));

           unsigned int connector_length = temp_connector.size();
           if(connector_length > 1 && temp_connector[0] == '>' && temp_connector[1] == '>'){
               in_out_redirection(command, words, connectors, argv, appended, string_out,                                   is_fd_in, is_fd_out, is_fd_err);
               return;
           }
           if(*(connectors.at(i)) == '<' || *(connectors.at(i)) == '|'){
               in_out_redirection(command, words, connectors, argv, appended, string_out,                                   is_fd_in, is_fd_out, is_fd_err);
               return;
           }

           /* the vector contains all '>' signs so call outputRedirection
            * once! */
           outputRedirection(command, words, argv);
        }
    }
    return;
}


//rshell begins here
//------------------------------------------------------------------------------------------------

void displayPrompt(){
    char* user;
    if( (user = getlogin()) == NULL){
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


int main(int argc, char** argv){
    /*string userInput; //command line in string format
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

    }while(1);*/


/* rshell stuff above. piping and io stuff below.*/



    /*while(getline(cin, input) != "exit"){*/

        /* get user input */
        //getline(cin, input);

        /*comments(input);

        if(input == ""){
            continue;
        }*/
        /*if(strcmp(input.c_str(), "exit") == 0){
            break;
        }*/

        /* extract components */
        //components = getParts(input);

        /* 1. separate connectors from files and/or commands */
        /* 2. create a vector for the files and/or commands to parse through*/
        /* 3. create a connectors vector that the extractConnects should return */

        /* call extractconnectors command to retrieve all the connectors*/
        //connectors = extractConnectors(components, words);

        /*perform appropriate logic*/
        //logic(connectors, words, argv);
        //cin.ignore();
    //}*/

    /* user input */
    string input;

    /* get user input */
    getline(cin, input);

    comments(input);

    if(input == ""){
        return 0;
    }

    /* extract components */
    vector<char*> components;
    components = getParts(input);

    /* 1. separate connectors from files and/or commands */
    /* 2. create a vector for the files and/or commands to parse through*/
    vector<char*> words;
    /* 3. create a connectors vector that the extractConnects should return */
    vector<char*> connectors;

    /* call extractconnectors command to retrieve all the connectors*/
    connectors = extractConnectors(components, words);

    /*perform appropriate logic*/
    logic(connectors, words, argv);
    return 0;
}
