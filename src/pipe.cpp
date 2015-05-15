#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

/* BUGS:
 * 1. out of range error when ending with a connector '<' or '>' */


//ONCE YOU ARE DONE, MERGE THIS FILE WITH RSHELL AND DELETE TARGET IN MAKEFILE

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
        int fd0 = open(files.at(i), O_RDONLY);
        if(fd0 == -1){
            perror(files.at(i));
            exit(1);
        }
        /* duplicate the file descriptor */
        dup2(fd0, STDIN_FILENO);

        /* close the file descriptor */
        close(fd0);
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
        int fd1 = open(files.at(i), O_WRONLY | O_TRUNC | O_CREAT,
                                    S_IRGRP | S_IWGRP | S_IWUSR);

        if(fd1 == -1){
            perror(files.at(i));
            exit(1);
        }

        /* duplicate file descriptor */
        dup2(fd1, STDOUT_FILENO);

        /* close the file descriptor */
        close(fd1);
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
                        vector<char*> connectors, char** argv){
    /* flush both STDIN and STDOUT */
    fflush(STDIN_FILENO);
    fflush(stdout);
    vector<char*> infiles;
    vector<char*> outfiles;
    for(unsigned int i = 0; i < files.size(); i++){
        if(*(connectors.at(i)) == '<'){
            infiles.push_back(files.at(i));
        }
        else if(*(connectors.at(i)) == '>'){
            outfiles.push_back(files.at(i));
        }
    }

    for(unsigned int x = 0; x < infiles.size(); x++){
        int in = open(infiles.at(x), O_RDONLY);
        if(in == -1){
            perror("open");
            exit(1);
        }
        dup2(in, STDIN_FILENO);
        close(in);
    }

    for(unsigned int z = 0; z < outfiles.size(); z++){
        int out = open(outfiles.at(z), O_WRONLY | O_TRUNC | O_CREAT,
                                      S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        if(out == -1){
            perror("open");
            exit(1);
        }
        dup2(out, STDOUT_FILENO);
        close(out);
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
        /* if you find a '<' push it onto connectors vector*/
        if(*(commands.at(i)) == '<'){
            connectors.push_back(commands.at(i));
        }
        /* if you find a '>' push it onto the connectors vector*/
        else if(*(commands.at(i)) == '>'){
            connectors.push_back(commands.at(i));
        }
        /* if you find a '|' push it onto the connectors vector*/
        else if(*(commands.at(i)) == '|'){
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
        inputRedirection(command, words, argv);
        return;
    }
    /* if the vector contain's all '<' signs, then only call
     * inputRedirection once. */
    /* in other words, if you ever see a '>' or '|' sign then stop*/
    else if(connectors.size() > 1 && *(connectors.at(0)) == '<'){
        for(unsigned int i = 0; i < connectors.size(); i++){
           if(*(connectors.at(i)) == '>' || *(connectors.at(i)) == '|'){
               in_out_redirection(command, words, connectors, argv);
               return;
           }
        }
        /* the vector contains all '<' sign so call inputRedirection once!*/
        inputRedirection(command, words, argv);
    }
    /* if there is only one '>' connector, then only execute
     * outputRedirection once*/
    else if(connectors.size() == 1 && *(connectors.at(0)) == '>'){
        outputRedirection(command, words, argv);
        return;
    }
    /* if the vector contain's all '>' signs, then only call
     * outputRedirection once. */
    else if(connectors.size() > 1 && *(connectors.at(0)) == '>'){
        for(unsigned int i = 0; i < connectors.size(); i++){
           if(*(connectors.at(i)) == '<' || *(connectors.at(i)) == '|'){
               //specialcase();
               return;
           }

           /* the vector contains all '>' signs so call outputRedirection
            * once! */
           outputRedirection(command, words, argv);
        }
    }
    return;
}



int main(int argc, char** argv){
    /* user input */
    string input;

    /* get user input */
    getline(cin, input);


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
