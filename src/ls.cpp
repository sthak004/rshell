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


/*this function will get all the file descriptors or directors and put
 them into a vector of strings called file_O_dir*/

/*vector<string> getFiles_Dir (char **argv, pos_){
    string targets; //this is the rest of the targest after the flags
    vector<string> importantStuff;

    unsigned int pos = pos_;
    while(argv[pos] != '\0'){
        cout << " << argv[pos]; " << argv[pos] << endl;
        cout << endl;
        pos++;
    }
    cout << endl;



    return importantStuff;
}*/




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
    /*did the user type is ls only?*/
    else if(argc > 1 && argc <= 2 && argv[1] == ls){
        is_ls = true;
        if(is_ls) cout << "Only run ls on current directory" << endl;
        return 0;
    }


    if(argc > 1 && argv[1] == ls){ //if there is ls followed by a dir or file
        ;
    }
    else{
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
