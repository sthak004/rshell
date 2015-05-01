#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

#define printVec(x) for(unsigned int i = 0; i < x.size(); i++) \
                        cout << x.at(i) << " "; \
                    cout << endl;



//function to print out the contents of argv
void printARGV(char **argv){
    int i = 0;
    while(argv[i] != NULL){
        cout << "Pos " << i << ": " << argv[i] << endl;
        i++;
    }
    cout << endl;
}

int main(int argc, char**argv){

    //test and case variables
    string flags_;
    string ls = "ls";
    char dash = '-';
//  --------------------------------------------------------------------

    //vectors of flags -a, -l, -R
    vector<char> flags;
    flags.push_back('a');
    flags.push_back('l');
    flags.push_back('R');

    //vector of directories
    vector<string> dir;
//  --------------------------------------------------------------------

    bool isFlag_a = false; //bool to determine if the flag is -a
    bool isFlag_l = false; //bool to determine if the flag is -l
    bool isFlag_R = false; //bool to determine if the flag is -R
    bool is_ls = false;    //bool to determine if the input ONLY ls
//  --------------------------------------------------------------------

    if(argc <= 1){ //assumes the user didn't type anything
        return 0;
    }
//  --------------------------------------------------------------------

    //What's in char **argv?
    printARGV(argv);
//  --------------------------------------------------------------------

    /*if you type anything other than ls or ls with flags,
      output an error and quit the program*/
    if(argc > 1 && argc <= 2 && argv[1] != ls){
        cout << "Error: no form of ls" << endl;
        exit(1);
    }

    /*did the user type is ls only?*/
    else if(argc > 1 && argc <= 2 && argv[1] == ls){
        is_ls = true;
        if(is_ls) cout << "Only run ls" << endl;
        return 0;
    }
//  --------------------------------------------------------------------

    /*supposed to determine which flags are called and which directories,
      if any, are listed */

    //grabs the flags
    if(*(argv[2]) == dash){
        flags_ = argv[2]; //put the cstring in a string
        flags_.erase(flags_.begin()); //erase the '-' at the beginning
    }

    //go through each flag_ and compare with the vector of flags
    for(unsigned int i = 0 ; i < flags_.size(); ++i){
        for(unsigned int j = 0; j < flags.size(); ++j){
            if(flags_.at(i) == flags.at(j)){
                if(flags.at(j) == 'a'){
                    isFlag_a = true;
                    if(isFlag_a) cout << "BINGO! We have a!" << endl;
                }
                else if(flags.at(j) == 'l'){
                    isFlag_l = true;
                    if(isFlag_l) cout << "HOORAY! We have l" << endl;
                }
                else if(flags.at(j) == 'R'){
                    isFlag_R = true;
                    if(isFlag_R) cout << "BOOOYA! We have R" << endl;
                }
            }
        }
    }


    /*for(int i = 0; i < argc; i++){
        for(unsigned int j = 0; j < flags.size(); j++){

            if(argv[2][i+1] == flags.at(j)){ //[2] is to start at flags
                                             //[i+1] is to ignore '-'
                if(flags.at(j) == 'a'){
                    isFlag_a = true;
                    if(isFlag_a) cout << "BINGO! We have -a!" << endl;
                }
                else if(flags.at(j) == 'l'){
                    isFlag_l = true;
                    if(isFlag_l) cout << "HOORAY! We have -l" << endl;
                }
                else if(flags.at(j) == 'R'){
                    isFlag_R = true;
                    if(isFlag_R) cout << "BOOOYA! We have -R" << endl;
                }
            }
        }
    }*/
//  --------------------------------------------------------------------

    return 0;
}
