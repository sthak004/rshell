#include <iostream>
#include <string.h>
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
        cout << "token" << pos << ": " << token << endl;

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


/* function4: parse user input */

int main(int argc, char** argv){
    /* user input */
    string input;

    /* get user input */
    getline(cin, input);

    /* extract components */
    vector<char*> onepieceatatime;
    onepieceatatime = getParts(input);

    printVec(onepieceatatime);
    return 0;
}
