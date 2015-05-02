#include <iostream>
#include <algorithm>
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


/*Comparision function for two char* */
/*needed for sorting*/
bool compareTwo(const char* s1, const char* s2){
    return strcasecmp(s1, s2) < 0;
}

bool compareTwo_(string a, string b){
    string str1( a );
    string str2( b );
    transform( str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform( str2.begin(), str2.end(), str2.begin(), ::tolower);
    return (str1 < str2);
}

/*this function takes in a directory and outputs its contents*/

/*string magic = directory */

vector<char*> open_direct(string magic){
    //const char* magic_2 = magic.c_str(); //need to do some more magic
    const char* magic_2 = magic.c_str();
    vector<char*> filenames; //to store the filenames so we can sort

    DIR *dirp;
    if(NULL == (dirp = opendir(magic_2))){
        perror("There was an error with opendir(). ");
        exit(1);
    }
    struct dirent *filespecs;
    errno = 0;
    while(NULL != (filespecs = readdir(dirp))){
        filenames.push_back(filespecs->d_name);
        //cout << filespecs->d_name << " ";
    }

    if(errno != 0){
        perror("There was an error with readdir(). ");
        exit(1);
    }
    cout << endl;
    if(-1 == closedir(dirp)){
        perror("There was an error with closedir(). ");
        exit(1);
    }


    return filenames;
}

void ls_multiple(vector<string> &file_dir){
    for(unsigned int i = 0; i < file_dir.size(); ++i){
        unsigned int j;

        //CHECK THIS STATEMENT!!!
        vector<char*> contents_ = open_direct(file_dir.at(i));

        /*remove any hidden files (files that begin with a '.')*/
        char dot = '.';
        unsigned int totalsize = contents_.size();
        for(j = 0; j < totalsize; ++j){
            if(*(contents_.at(j)) == dot){
                contents_.erase(contents_.begin() + j); //remove that file
                totalsize--; //after you erase you -1 from size
                j = 0; //after you -1 from size, you reset i
            }
        }

        //sort the files within directory
        sort(contents_.begin(), contents_.end(), compareTwo);

        cout << file_dir.at(j) << ":" << endl; //formatting..

        //print the files to standard out
        printVec(contents_);

        cout << endl; //formatting..
    }
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
    //printARGV(argv);
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
        if(is_ls) ;

        files_dirs = getFiles_Dirs(argv); //get directories or files

        //sort dictionary files so they can be executed in order
        sort(files_dirs.begin(), files_dirs.end(), compareTwo_);

        //where there any directories or files that were called?
        if( !(files_dirs.size() > 0) ){
            /*call ls on the current working directory*/

            //get the current working directory
            char buf[1024];
            if(!getcwd(buf,1024)){
                perror("problem with getcwd");
            }
            string cwd(buf); //convert cstring to string for function

            //get the vector of the contents in the current directory
            vector<char*> contents = open_direct(cwd.c_str());

            /*remove any hidden files (files that begin with a '.')*/
            char dot = '.';
            unsigned int totalsize = contents.size();
            for(unsigned int i = 0; i < totalsize; ++i){
                if(*(contents.at(i)) == dot){
                    contents.erase(contents.begin() + i); //remove that file
                    totalsize--; //after you erase you -1 from size
                    i = 0; //after you -1 from size, you reset i
                }
            }

            //sort the files within directory
            sort(contents.begin(), contents.end(), compareTwo);

            //print the files to standard out
            printVec(contents);
        }
        else{
            printVec(files_dirs);
            ls_multiple(files_dirs);
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
