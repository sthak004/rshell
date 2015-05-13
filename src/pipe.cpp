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
/* parmeter1: file you want to open*/
/* parmeter2: command you want to run*/
/* parmeter3: list of all arguments (needed for execvp) */
void inputRedirection(char* command, char* file, char **argv){
    /* pid gets the value of fork*/
    int pid = fork();

    /* based on the value of fork, perform the following operations */
    if(pid == -1){ //fork straight up failed you...
        perror("fork() failed");
        exit(1);
    }
    else if(pid == 0){ //child process = good *thumbs up*
        /* flush standard out so that, if the forked child writes to standard
         * error because of a problem, there will be no extraneous duplicated
         * output.*/
        fflush(STDIN_FILENO);

        /* open the file passed in */
        int fd0 = open(file, O_RDONLY);
        if(fd0 == -1){
            perror(file);
            exit(1);
        }

        /* duplicate the file descriptor */
        /* use STDIN_FILENO as standard input*/
        dup2(fd0, STDIN_FILENO);

        /* close the file descriptor*/
        close(fd0);


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

/* function5: perform logic - IO/pipe */
void logic(vector<char*> &parts, char **argv){
    /* loop through vector and determine each case */
    for(unsigned int i = 0; i < parts.size(); i++){
        if(*(parts.at(i)) == '<'){
            /* check is ' < ' is not the first or last token*/
            if(i > 0 && i < parts.size() - 1){
                /* 1.perform input redirection assuming "i" is in a
                 * position*/

                /* 2.you pass in parts.at(i-1) because that SHOULD
                 * be the command preceding the ' < ' sign*/

                /* 3.you pass in parts.at(i+1) because you getting information
                 * from the file AFTER the ' < ' sign */
                inputRedirection(parts.at(i-1), parts.at(i+1), argv);
            }
        }
    }
    return;
}


/* function5: input redirection */

int main(int argc, char** argv){
    /* user input */
    string input;

    /* get user input */
    getline(cin, input);

    /* extract components */
    vector<char*> components;
    components = getParts(input);

    /*perform appropriate logic*/
    logic(components, argv);
    return 0;
}
