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
#include <cstddef>
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


/*this function will get all the file descriptors or directories and put
 them into a vector of strings called file_O_dir*/

//anything that is not 'ls' or '-<flags>' is considered a file or dir

vector<string> getFiles_Dirs (char **argv){
    string targets; //this is the rest of the targest after the flags

    string ls = "ls";
    char dash = '-';

    vector<string> importantStuff;

    unsigned int pos = 1;
    while(argv[pos] != '\0'){
        string temp = argv[pos]; //keep the current argv part in a string

        if(argv[pos] == ls){
            pos++; //increment position to get next arg
            continue; //continue to next iteration
        }

        if(argv[pos][0] == dash){
            pos++; //increment position to get next arg
            continue; //continue to next iteration
        }

        importantStuff.push_back(temp); //if both cases fail, you add it!

        pos++; //increment to get next arg
    }


    return importantStuff;
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

    //vector of files & directories
    vector<string> files_dirs;
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
    /*below are some cases for ls*/
    else if(argc > 1  && argv[1] == ls){ //is there a ls?
        is_ls = true;
        if(is_ls) cout << "run ls depending on the size of dir vector" << endl;

        files_dirs = getFiles_Dirs(argv); //get directories or files

        //where there any directories or files that were called?
        if( !(files_dirs.size() > 0) ){
            //call ls on the current working directory
        }
    }
    else{ //is there a ls with flags and maybe files or dir
        for(int x = 0; x < argc; ++x){
            if(*(argv[x]) == dash){ //grabs flags
                flags_ = argv[x]; //put the cstring in a string
                flags_.erase(flags_.begin()); //erase the '-' at the beginning

                //go through each flag_ and compare with the vector of flags
                for(unsigned int i = 0 ; i < flags_.size(); ++i){
                    for(unsigned int j = 0; j < flags.size(); ++j){
                        if(flags_.at(i) == flags.at(j)){
                            if(flags.at(j) == 'a'){
                                isFlag_a = true;
                                if(isFlag_a) cout << "We have a!" << endl;
                            }
                            else if(flags.at(j) == 'l'){
                                isFlag_l = true;
                                if(isFlag_l) cout << "We have l!" << endl;
                            }
                            else if(flags.at(j) == 'R'){
                                isFlag_R = true;
                                if(isFlag_R) cout << "We have R!" << endl;
                            }
                        }
                    }
                }
            }
        }
    }

    //files_dirs = getFiles_Dir(argv, );
//  --------------------------------------------------------------------

    return 0;
}
